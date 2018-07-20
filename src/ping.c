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

int 					ping_loop(t_mgr *mgr, t_echo *echo, struct sockaddr_in *sin)
{
	struct timeval	then;
	struct timeval	now;
	u_int8_t		packet[IP_MAXPACKET];
	struct msghdr	resp;
	struct cmsghdr	*cmsg;
	u_int8_t		ctrlbuff[256];
	ssize_t 		rbyte;

	resp.msg_name = NULL;
	resp.msg_namelen = sizeof(struct sockaddr);
	resp.msg_iov = NULL;
	resp.msg_iovlen = 0;
	resp.msg_control = ctrlbuff;
	resp.msg_controllen = (sizeof(struct cmsghdr) +
		IPV4_HDRLEN + ICMP_HDRLEN + sizeof(echo->time) + echo->datalen);
	gettimeofday(&then, NULL);
	signal(SIGALRM, alarm_handel_timeout);
	printf("Entered Ping\n");
	while (mgr->count)
	{
		printf("Loop\n");
		gettimeofday(&now, NULL);
		if ((now.tv_sec + (1.0 / 1000000) * now.tv_usec) -
			(then.tv_sec + (1.0 / 1000000) * then.tv_usec) > 1.0)
		{
			printf("Sending Message\n");
			ft_memset(packet, 0, IP_MAXPACKET);
			fill_packet(packet, echo);
			if (sendto(mgr->sock, packet, (IPV4_HDRLEN + ICMP_HDRLEN +
				sizeof(echo->time) + echo->datalen), 0,
					(struct sockaddr *)sin, sizeof(struct sockaddr)) < 0)
			{
				dprintf(STDERR_FILENO, "Error sendto(). %s\n", strerror(errno));
				exit(FAILURE);
			}
			alarm(3);
			if (mgr->flags.count == TRUE)
				mgr->count -= 1;
			echo->icmp.icmp_hun.ih_idseq.icd_seq = ntohs(++mgr->seq);
			gettimeofday(&then, NULL);
			if ((rbyte = recvmsg(mgr->sock, &resp, 0)) < 0) {
				dprintf(STDERR_FILENO, "Error recvmsg().%s\n", strerror(errno));
				exit(FAILURE);
			} else {
				printf("Recieved something! %zu\n", rbyte);
				alarm(0);
				cmsg = (struct cmsghdr *) resp.msg_control;
				(void) cmsg;
			}
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
	ping_loop(mgr, &mgr->echo, &mgr->sin);
	return (SUCCESS);
}
