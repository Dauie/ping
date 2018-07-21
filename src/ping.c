#include "../incl/ping.h"



static u_int16_t 		update_checksum(t_echo *echo, u_int8_t *packet)
{
	return (checksum((packet + IPV4_HDRLEN),
				(ICMP_HDRLEN + sizeof(echo->time) + echo->datalen)));
}

static void				init_icmp_header_request(t_mgr *mgr, struct icmp *icmp)
{
	icmp->icmp_type = ICMP_ECHO;
	icmp->icmp_code = 0;
	icmp->icmp_hun.ih_idseq.icd_id = htons(mgr->pid);
	icmp->icmp_hun.ih_idseq.icd_seq = htons(mgr->seq);
}

static void				init_ip_header(t_mgr *mgr, struct ip *ip, t_echo *echo)
{
	ip->ip_hl = IPV4_HDRLEN / sizeof(uint32_t);
	ip->ip_v = 4;
	ip->ip_tos = 0;
	ip->ip_len = htons(IPV4_HDRLEN + ICMP_HDRLEN + sizeof(echo->time) +
							echo->datalen);
	ip->ip_id = htons(0);
	ip->ip_off = htons(0);
	ip->ip_ttl = 64;
	ip->ip_p = IPPROTO_ICMP;
	if (inet_pton(AF_INET, mgr->saddr, &(ip->ip_src.s_addr)) <= 0)
	{
		dprintf(STDERR_FILENO, "Error inet_pton() 1. %s\n", strerror(errno));
		exit(FAILURE);
	}
	if (inet_pton(AF_INET, mgr->daddr, &(ip->ip_dst.s_addr)) <= 0)
	{
		dprintf(STDERR_FILENO, "Error inet_pton() 2. %s\n", strerror(errno));
		exit(FAILURE);
	}
}

void					prep_sockaddr(struct sockaddr_in *sin, t_echo *echo)
{
	ft_memset(sin, 0, sizeof (struct sockaddr_in));
	sin->sin_family = AF_INET;
	sin->sin_addr.s_addr = echo->ip.ip_dst.s_addr;
}

void 					fill_packet(u_int8_t *packet, t_echo *echo)
{
	echo->icmp.icmp_cksum = 0;
	ft_memcpy(packet, &echo->ip, IPV4_HDRLEN);
	ft_memcpy((packet + IPV4_HDRLEN), &echo->icmp, ICMP_HDRLEN);
	gettimeofday(&echo->time, NULL);
	ft_memcpy((packet + IPV4_HDRLEN + ICMP_HDRLEN), &echo->time, sizeof(echo->time));
	ft_memcpy((packet + IPV4_HDRLEN + ICMP_HDRLEN + sizeof(echo->time)), echo->data, echo->datalen);
	echo->icmp.icmp_cksum = update_checksum(echo, packet);
	ft_memcpy(packet + IPV4_HDRLEN, &echo->icmp, ICMP_HDRLEN);
}

int 					send_ping(t_mgr *mgr, t_echo *echo)
{
	u_int8_t		packet[IP_MAXPACKET];

	ft_memset(packet, 0, IP_MAXPACKET);
	fill_packet(packet, echo);
	if (sendto(mgr->sock, packet, (IPV4_HDRLEN + ICMP_HDRLEN +
								   sizeof(echo->time) + echo->datalen), 0,
			   (struct sockaddr *)&mgr->sin, sizeof(struct sockaddr)) < 0)
	{
		dprintf(STDERR_FILENO, "Error sendto(). %s\n", strerror(errno));
		exit(FAILURE);
	}
	return (SUCCESS);
}

struct msghdr		*init_msghdr()
{
	struct iovec	*iov;
	struct msghdr	*resp;
	u_int8_t		*addrbuff;
	u_int8_t		*resp_data;

	if (!(resp = ft_memalloc(sizeof(struct msghdr))))
		return (NULL);
	if (!(addrbuff = ft_memalloc(sizeof(IPV4_ADDR_LEN))))
		return (NULL);
	if (!(resp_data = ft_memalloc(256)))
		return (NULL);
	if (!(iov = ft_memalloc(sizeof(struct iovec))))
		return (NULL);
	iov->iov_base = resp_data;
	iov->iov_len = 256;
	ft_memset(resp, 0, sizeof(struct msghdr));
	resp->msg_name = addrbuff;
	resp->msg_namelen = IPV4_ADDR_LEN;
	resp->msg_iov = iov;
	resp->msg_iovlen = 1;
	resp->msg_control = resp_data;
	resp->msg_controllen = sizeof(resp_data);
	return (resp);
}

int 				handel_response(struct msghdr *resp, struct timeval *now, ssize_t rbyte)
{
	struct icmp		*icmp;
	struct timeval	*then;
	struct in_addr	*src;
	double			timediff;
	char 			addr[IPV4_ADDR_LEN];
	u_short			seq;
	char 			ttl;

	icmp = (struct icmp *)&((u_int8_t *)resp->msg_control)[IPV4_HDRLEN];
	then = (struct timeval *)&((u_int8_t *)resp->msg_control)[IPV4_HDRLEN + ICMP_HDRLEN];
	if (icmp->icmp_type == TYPE_ECHO_RPLY)
	{
		timediff = (now->tv_sec + (1.0 / 1000000) * now->tv_usec) -
				(then->tv_sec + (1.0 / 1000000) * then->tv_usec);
		seq = ntohs((u_short)((struct icmp *)&((u_int8_t *)resp->msg_control)[IPV4_HDRLEN])->icmp_hun.ih_idseq.icd_seq);
		ttl = ((struct ip *)resp->msg_control)->ip_ttl;
		src = &((struct ip *)resp->msg_control)->ip_src;
		inet_ntop(AF_INET, src, addr, IPV4_ADDR_LEN);

		printf("%zu bytes from %s: icmp_seq=%u ttl=%i time=%f ms\n",
			   rbyte, addr, seq, (int)ttl, timediff);
	}
	return (SUCCESS);
}

void				recv_ping(t_mgr *mgr, struct timeval *now)
{
	struct msghdr	*resp;
	ssize_t 		rbyte;

	resp = init_msghdr();
	if ((rbyte = recvmsg(mgr->sock, resp, MSG_DONTWAIT)) < 0)
	{
		if (errno == EAGAIN)
			return ;
		dprintf(STDERR_FILENO, "Error recvmsg().%s\n", strerror(errno));
		exit(FAILURE);
	}
	else
	{
		alarm(0);
		handel_response(resp, now, rbyte);
	}
	if (g_toflg == TRUE)
	{
		printf("Request timeout for icmp_seq %zu\n", mgr->seq - 1);
		g_toflg = FALSE;
	}
}

int 					ping_loop(t_mgr *mgr, t_echo *echo)
{
	struct timeval	then;
	struct timeval	now;

	gettimeofday(&then, NULL);
	signal(SIGALRM, alarm_handel_timeout);
	while (mgr->count)
	{
		gettimeofday(&now, NULL);
		if ((now.tv_sec + (1.0 / 1000000) * now.tv_usec) -
			(then.tv_sec + (1.0 / 1000000) * then.tv_usec) > 1.0)
		{
			send_ping(mgr, echo);
			alarm(1);
			if (mgr->flags.count == TRUE)
				mgr->count -= 1;
			echo->icmp.icmp_hun.ih_idseq.icd_seq = ntohs(++mgr->seq);
			gettimeofday(&then, NULL);
			recv_ping(mgr, &now);
		}
	}
	return (SUCCESS);
}

int						ping(t_mgr *mgr)
{
	ft_strcpy(mgr->echo.data, "                !\"#$%&'()*+,-./01234567");
	mgr->echo.datalen = (u_short)ft_strlen(mgr->echo.data);
	init_ip_header(mgr, &mgr->echo.ip, &mgr->echo);
	init_icmp_header_request(mgr, &mgr->echo.icmp);
	prep_sockaddr(&mgr->sin, &mgr->echo);
	printf("PING %s (%s) %zu(%zu) bytes of data.\n",
		mgr->domain, mgr->daddr, mgr->echo.datalen + sizeof(struct timeval),
			IPV4_HDRLEN + ICMP_HDRLEN + mgr->echo.datalen + sizeof(struct timeval));
	ping_loop(mgr, &mgr->echo);
	return (SUCCESS);
}
