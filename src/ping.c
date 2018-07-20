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
	struct iovec	iov;
	u_int8_t		addrbuff[256];
	u_int8_t 		resp_data[256];
	ssize_t 		rbyte;

	iov.iov_base = &resp_data;
	iov.iov_len = 256;
	ft_memset(&resp, 0, sizeof(struct msghdr));
	resp.msg_name = &addrbuff;
	resp.msg_namelen = 256;
	resp.msg_iov = &iov;
	resp.msg_iovlen = 1;
	resp.msg_control = &resp_data;
	resp.msg_controllen = sizeof(resp_data);

	gettimeofday(&then, NULL);
	//signal(SIGALRM, alarm_handel_timeout);
	printf("Entered Ping\n");
	while (mgr->count)
	{
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
			printf("Sent\n");
			//alarm(1);
			if (mgr->flags.count == TRUE)
				mgr->count -= 1;
			echo->icmp.icmp_hun.ih_idseq.icd_seq = ntohs(++mgr->seq);
			gettimeofday(&then, NULL);
			printf("At recvmsg\n");
			if ((rbyte = recvmsg(mgr->sock, &resp, MSG_DONTWAIT)) < 0) {
				if (errno == EAGAIN)
					continue;
				dprintf(STDERR_FILENO, "Error recvmsg().%s\n", strerror(errno));
				exit(FAILURE);
			} else if (rbyte == 0)
			{
				if (g_toflg == TRUE)
				{
					printf("Request timeout for icmp_seq %zu\n", mgr->seq - 1);
					g_toflg = FALSE;
				}
			} else
			{
				// Print packet.
				char *address = (char *)resp_data;
				int i;
				u_short word;
				for(i = 0 ; i + 1 < rbyte; i += 2)
				{
					ft_memcpy(&word, (u_short*)&address[i], 2);
					printf("%02X", (u_short)address[i]);
					if (i % 16 == 0)
						write(1, "\n", 1);
				}
				if (i & 1)
				{
					  word = 0;
					ft_memcpy(&word, (uint8_t *)address + i - 1, 1);
					printf("%02X", word);
				}

				cmsg = (struct cmsghdr *)&resp;
				struct icmp *eye = (struct icmp *)(resp_data + rbyte) - (sizeof(echo->time) + echo->datalen + ICMP_HDRLEN) ;
				struct timeval *resptime = (struct timeval *)((u_int8_t*)resp_data + IPV4_HDRLEN + ICMP_HDRLEN);
				(void)eye;
				(void)resptime;
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
