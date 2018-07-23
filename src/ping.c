#include "../incl/ping.h"

static u_int16_t 		update_checksum(t_echo *echo, u_int8_t *packet)
{
	return (checksum((packet + IPV4_HDRLEN),
				(ICMP_HDRLEN + sizeof(echo->time) + echo->datalen)));
}

static void				init_icmp_header_request(t_mgr *mgr, icmp *icmp)
{
	icmp->icmp_type = ICMP_ECHO;
	icmp->icmp_code = 0;
	icmp->icmp_hun.ih_idseq.icd_id = htons((uint16_t)mgr->pid);
	icmp->icmp_hun.ih_idseq.icd_seq = htons((uint16_t)mgr->seq);
}

static void				init_ip_header(t_mgr *mgr, ip *ip, t_echo *echo)
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

void					prep_sockaddr(sockaddr_in *sin, t_echo *echo)
{
	ft_memset(sin, 0, sizeof (sockaddr_in));
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
			(struct sockaddr *)&mgr->sin, sizeof(sockaddr)) < 0)
	{
		dprintf(STDERR_FILENO, "Error sendto(). %s\n", strerror(errno));
		exit(FAILURE);
	}
	return (SUCCESS);
}

msghdr				*init_msghdr()
{
	iovec			*iov;
	msghdr			*resp;
	u_int8_t		*resp_data;

	if (!(resp = ft_memalloc(sizeof(msghdr))))
		return (NULL);
	if (!(resp_data = ft_memalloc(256)))
		return (NULL);
	if (!(iov = ft_memalloc(sizeof(iovec))))
		return (NULL);
	iov->iov_base = resp_data;
	iov->iov_len = 256;
	ft_memset(resp, 0, sizeof(msghdr));
	resp->msg_iov = iov;
	resp->msg_iovlen = 1;
	resp->msg_control = resp_data;
	resp->msg_controllen = sizeof(resp_data);
	return (resp);
}

void				clean_msghdr(msghdr **msg)
{
	free((*msg)->msg_iov->iov_base);
	free((*msg)->msg_iov);
	free(*msg);
}

long double time_diff(timeval *then, timeval *now)
{
	long double x =
			(double)(then->tv_usec - now->tv_usec) / 1000.0L +
			(double)(then->tv_sec - now->tv_sec) * 1000.0L;
	return x;
}

void		update_minmaxavg(t_stats *stats, float ms)
{
	if (stats->min == 0)
		stats->min = ms;
	if (stats->max == 0)
		stats->max = ms;
	if (stats->avg == 0)
		stats->avg = ms;
	if (ms > stats->max)
		stats->max = ms;
	if (ms < stats->min)
		stats->min = ms;
	stats->avg = (stats->avg + ms) / 2;
}

int 				handel_response(msghdr *resp, timeval *now, t_mgr *mgr, ssize_t rbyte)
{
	icmp			*icmp;
	timeval			*then;
	in_addr			*src;
	float			ms;
	char 			addr[IPV4_ADDR_LEN];
	u_short			seq;
	char 			ttl;

	icmp = (struct icmp *)&((u_int8_t *)resp->msg_control)[IPV4_HDRLEN];
	then = (timeval *)&((u_int8_t *)resp->msg_control)[IPV4_HDRLEN + ICMP_HDRLEN];
	ms = (float)time_diff(now, then);
	update_minmaxavg(&mgr->stats, ms);
	seq = ntohs((u_short)((struct icmp *)&((u_int8_t *)resp->msg_control)[IPV4_HDRLEN])->icmp_hun.ih_idseq.icd_seq);
	src = &((struct ip *)resp->msg_control)->ip_src;
	inet_ntop(AF_INET, src, addr, IPV4_ADDR_LEN);
	ttl = ((ip *)resp->msg_control)->ip_ttl;
	if (icmp->icmp_type == TYPE_ECHO_RPLY)
		printf("%zu bytes from %s: icmp_seq=%u ttl=%i time=%.2f ms\n",
			rbyte, addr, seq, (int)ttl, ms);
	else if (icmp->icmp_type == TYPE_DST_UNRCH)
		printf("From %s icmp_seq=%u Destination Host Unreachable\n", addr, seq);
	return (SUCCESS);
}

void				recv_ping(t_mgr *mgr, timeval *now)
{
	msghdr			*resp;
	ssize_t 		rbyte;

	resp = init_msghdr();
	if ((rbyte = recvmsg(mgr->sock, resp, MSG_DONTWAIT)) < 0)
	{
		if (errno == EAGAIN)
		{
			clean_msghdr(&resp);
			return;
		}
		dprintf(STDERR_FILENO, "Error recvmsg().%s\n", strerror(errno));
		exit(FAILURE);
	}
	else
	{
		alarm(0);
		mgr->stats.recvd++;
		handel_response(resp, now, mgr, rbyte);
	}
	if (g_sigflgs.timeoflg == TRUE)
	{
		printf("Request timeout for icmp_seq %zu\n", mgr->seq - 5);
		g_sigflgs.timeoflg = FALSE;
	}
	clean_msghdr(&resp);
}

int 					ping_loop(t_mgr *mgr, t_echo *echo)
{
	timeval	then;
	timeval	now;

	gettimeofday(&then, NULL);
	signal(SIGALRM, sigalrm_handel_timeout);
	signal(SIGINT, sigint_handel_exit);
	while (mgr->count && g_sigflgs.exitflg == FALSE)
	{
		gettimeofday(&now, NULL);
		if ((now.tv_sec + (1.0 / 1000000) * now.tv_usec) -
			(then.tv_sec + (1.0 / 1000000) * then.tv_usec) > 1.0)
		{
			send_ping(mgr, echo);
			mgr->stats.sent++;
			alarm(4);
			if (mgr->flags.count == TRUE)
				mgr->count -= 1;
			echo->icmp.icmp_hun.ih_idseq.icd_seq = ntohs(++mgr->seq);
			gettimeofday(&then, NULL);
		}
		recv_ping(mgr, &now);
	}
	return (SUCCESS);
}

float					get_percentage(size_t a, size_t b)
{
	size_t 				diff;

	diff = a - b;
	if (diff == 0)
		return (0.00f);
	else
		return (((float)diff / a) * 100);
}

void					print_stats(t_mgr *mgr)
{
	long double packet_loss;
	long double duration;

	packet_loss = get_percentage(mgr->stats.sent, mgr->stats.recvd);
	duration = time_diff(&mgr->stats.end, &mgr->stats.start);
	printf("\n--- %s ping statistics ---\n", mgr->domain);
	printf("%zu packets transmitted, %zu received, %.2Lf%% packet loss, time %.0Lfms\n",
		mgr->stats.sent, mgr->stats.recvd, packet_loss, duration);
	/*TODO: MDEV standard deviation or 'jitter'*/
	printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f ms\n", mgr->stats.min, mgr->stats.avg, mgr->stats.max);
}

int						ping(t_mgr *mgr)
{
	ft_strcpy(mgr->echo.data, "                !\"#$%&'()*+,-./01234567");
	mgr->echo.datalen = (u_short)ft_strlen(mgr->echo.data);
	init_ip_header(mgr, &mgr->echo.ip, &mgr->echo);
	init_icmp_header_request(mgr, &mgr->echo.icmp);
	prep_sockaddr(&mgr->sin, &mgr->echo);
	printf("PING %s (%s) %zu(%zu) bytes of data.\n",
		mgr->domain, mgr->daddr, mgr->echo.datalen + sizeof(timeval),
			IPV4_HDRLEN + ICMP_HDRLEN + mgr->echo.datalen + sizeof(timeval));
	gettimeofday(&mgr->stats.start, NULL);
	ping_loop(mgr, &mgr->echo);
	gettimeofday(&mgr->stats.end, NULL);
	print_stats(mgr);
	return (SUCCESS);
}
