#include "../incl/ping.h"

static void			fill_icmp_header(t_mgr *mgr, t_echo *msg, struct icmp *icmp, u_short datalen)
{
	icmp->icmp_type = ICMP_ECHO;
	icmp->icmp_code = 0;
	icmp->icmp_cksum = 0;
	icmp->icmp_hun.ih_idseq.icd_id = htons(mgr->pid);
	icmp->icmp_hun.ih_idseq.icd_seq = htons(mgr->seq);
	icmp->icmp_cksum = checksum((u_int8_t *)msg + IPV4_HDRLEN, ICMP_HDRLEN + datalen);
}

static void			fill_ip_header(t_mgr *mgr, struct ip *ip, int datalen)
{
	ip->ip_hl = IPV4_HDRLEN / sizeof(uint32_t);
	ip->ip_v = 4;
	ip->ip_tos = 0;
	ip->ip_len = htons(IPV4_HDRLEN + ICMP_HDRLEN + datalen);
	ip->ip_id = htons(0);
	ip->ip_off = htons(0);
	ip->ip_ttl = 255;
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

int						ping(t_mgr *mgr)
{
	struct sockaddr_in	sin;
	t_echo				echo;
	u_short				datalen;
//	u_int8_t			packet[IP_MAXPACKET];

	ft_strcpy(echo.data, "FT ECHO REQUEST");
	datalen = (u_short)ft_strlen(echo.data);
	fill_ip_header(mgr, &echo.ip, datalen);
	fill_icmp_header(mgr, &echo, &echo.icmp, datalen);
//	ft_memset(packet, 0, IP_MAXPACKET);
//	ft_memcpy(packet, &echo.ip, IPV4_HDRLEN);
//	ft_memcpy(packet + IPV4_HDRLEN, &echo.icmp, ICMP_HDRLEN);
//	ft_memcpy(packet + IPV4_HDRLEN + ICMP_HDRLEN, &echo.data, datalen);
//	ft_memset(&sin, 0, sizeof (struct sockaddr_in));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = echo.ip.ip_dst.s_addr;
	if (sendto(mgr->sock, &echo, IPV4_HDRLEN + ICMP_HDRLEN + datalen, 0,
		   (struct sockaddr *)&sin, sizeof(struct sockaddr)) < 0)
	{
		dprintf(STDERR_FILENO, "Error sendto(). %s\n", strerror(errno));
		exit(FAILURE);
	}
	return (SUCCESS);
}