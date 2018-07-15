#include "../incl/ping.h"

static void			fill_icmp_header(t_mgr *mgr, t_echo *msg, t_icmp_hdr *icmp, u_short datalen)
{
	icmp->type = ICMP_ECHO;
	icmp->code = 0;
	icmp->id = htons(mgr->pid);
	icmp->seq = htons(mgr->seq);
	icmp->checksum = checksum((u_int16_t *)msg + IPV4_HDRLEN, ICMP_HDRLEN + datalen);
}

static void			fill_ip_header(t_mgr *mgr, t_ip_hdr *ip, int datalen)
{
	ip->ihl = IPV4_HDRLEN / sizeof(uint32_t);
	ip->version = 4;
	ip->tos = 0;
	ip->tot_len = htons(IPV4_HDRLEN + ICMP_HDRLEN + datalen);
	ip->id = htons(0);
	ip->frag_off = htons(0);
	ip->ttl = 255;
	ip->proto = IPPROTO_ICMP;
	if (inet_pton(AF_INET, mgr->saddr, &(ip->saddr)) <= 0)
	{
		dprintf(STDERR_FILENO, "Error inet_pton() 1. %s\n", strerror(errno));
		exit(FAILURE);
	}
	if (inet_pton(AF_INET, mgr->daddr, &(ip->daddr)) <= 0)
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

	ft_strcpy(echo.data, "FT ECHO REQUEST");
	datalen = (u_short)ft_strlen(echo.data);
	fill_ip_header(mgr, &echo.ip, datalen);
	fill_icmp_header(mgr, &echo, &echo.icmp_hdr, datalen);
	ft_memset(&sin, 0, sizeof (struct sockaddr_in));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = echo.ip.daddr;
	if (sendto(mgr->sock, &echo, IPV4_HDRLEN + ICMP_HDRLEN + datalen, 0,
		   (struct sockaddr *)&sin, sizeof(struct sockaddr)) < 0)
	{
		dprintf(STDERR_FILENO, "Error sendto(). %s\n", strerror(errno));
		exit(FAILURE);
	}
	return (SUCCESS);
}