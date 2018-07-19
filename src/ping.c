#include "../incl/ping.h"

static u_int16_t 		update_checksum(t_echo *echo, u_int8_t *packet)
{
	echo->icmp.icmp_cksum = 0;
	return (checksum(packet + IPV4_HDRLEN,
			ICMP_HDRLEN + sizeof(echo->time.tv_usec) + echo->datalen));
}

static void				init_icmp_header_request(t_mgr *mgr, struct icmp *icmp)
{
	icmp->icmp_type = ICMP_ECHO;
	icmp->icmp_code = 0;
	icmp->icmp_cksum = 0;
	icmp->icmp_hun.ih_idseq.icd_id = htons(mgr->pid);
	icmp->icmp_hun.ih_idseq.icd_seq = htons(1);
	/**
	 * Checksum will be calculated after timestamp is added
	 **/
}

static void				init_ip_header(t_mgr *mgr, struct ip *ip, t_echo *echo)
{
	ip->ip_hl = IPV4_HDRLEN / sizeof(uint32_t);
	ip->ip_v = 4;
	ip->ip_tos = 0;
	ip->ip_len = htons(IPV4_HDRLEN + ICMP_HDRLEN + sizeof(echo->time.tv_usec) +
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

	ft_memcpy(packet, &echo->ip, IPV4_HDRLEN);
	ft_memcpy(packet + IPV4_HDRLEN, &echo->icmp, ICMP_HDRLEN);
	gettimeofday(&echo->time, NULL);
	ft_memcpy(packet + IPV4_HDRLEN + ICMP_HDRLEN, &echo->time.tv_usec, sizeof(echo->time.tv_usec));
	ft_memcpy(packet + IPV4_HDRLEN + ICMP_HDRLEN + sizeof(echo->time.tv_usec), echo->data, echo->datalen);
	echo->icmp.icmp_cksum = update_checksum(echo, packet);
	ft_memcpy(packet + IPV4_HDRLEN, &echo->icmp, ICMP_HDRLEN);
}

int 					ping_loop(t_mgr *mgr, t_echo *echo, struct sockaddr_in *sin)
{
	u_int8_t packet[IP_MAXPACKET];

	while (mgr->count)
	{
		fill_packet(packet, echo);
		if (sendto(mgr->sock, echo, IPV4_HDRLEN + ICMP_HDRLEN + echo->datalen, 0,
				   (struct sockaddr *)sin, sizeof(struct sockaddr)) < 0)
		{
			dprintf(STDERR_FILENO, "Error sendto(). %s\n", strerror(errno));
			exit(FAILURE);
		}
		if (mgr->flags.count == TRUE)
			mgr->count -= 1;
		echo->icmp.icmp_hun.ih_idseq.icd_seq += 1;
	}
	return (SUCCESS);
}

int						ping(t_mgr *mgr)
{
	struct sockaddr_in	sin;
	t_echo				echo;

	ft_strcpy(echo.data, "!\"#$%&'()*+,-./01234567");
	echo.datalen = (u_short)ft_strlen(echo.data);
	init_ip_header(mgr, &echo.ip, &echo);
	init_icmp_header_request(mgr, &echo.icmp);
	prep_sockaddr(&sin, &echo);
	ping_loop(mgr, &echo, &sin);
	return (SUCCESS);
}
