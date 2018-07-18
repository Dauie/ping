#include <netinet/ip_icmp.h>
#include "../incl/ping.h"

static void				fill_icmp_header(t_mgr *mgr, t_echo *echo,
											struct icmp *icmp)
{
	icmp->icmp_type = ICMP_ECHO;
	icmp->icmp_code = 0;
	icmp->icmp_cksum = 0;
	icmp->icmp_hun.ih_idseq.icd_id = htons(mgr->pid);
	icmp->icmp_hun.ih_idseq.icd_seq = htons(mgr->seq);
	icmp->icmp_cksum = checksum((u_int8_t *)echo + IPV4_HDRLEN,
								ICMP_HDRLEN + echo->datalen);
}

static void				fill_ip_header(t_mgr *mgr, struct ip *ip, t_echo *echo)
{
	ip->ip_hl = IPV4_HDRLEN / sizeof(uint32_t);
	ip->ip_v = 4;
	ip->ip_tos = 0;
	ip->ip_len = htons(IPV4_HDRLEN + ICMP_HDRLEN + echo->datalen);
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

void					prep_sockaddr(struct sockaddr_in *sin, t_echo *echo)
{
	ft_memset(sin, 0, sizeof (struct sockaddr_in));
	sin->sin_family = AF_INET;
	sin->sin_addr.s_addr = echo->ip.ip_dst.s_addr;
}

int 					ping_loop(t_mgr *mgr, t_echo *echo, struct sockaddr_in *sin)
{
	while (mgr->count)
	{
		if (sendto(mgr->sock, &echo, IPV4_HDRLEN + ICMP_HDRLEN + echo->datalen, 0,
				   (struct sockaddr *)&sin, sizeof(struct sockaddr)) < 0)
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

	ft_strcpy(echo.data, "FT ECHO REQUEST 42");
	echo.datalen = (u_short)ft_strlen(echo.data);
	fill_ip_header(mgr, &echo.ip, &echo);
	fill_icmp_header(mgr, &echo, &echo.icmp);
	prep_sockaddr(&sin, &echo);
	ping_loop(mgr, &echo, &sin);
	return (SUCCESS);
}
