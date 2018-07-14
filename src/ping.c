#include "../incl/ping.h"

static void			fill_ip_header(t_mgr *mgr, t_echo *msg, int datalen)
{
	msg->ip.ihl = IPV4_HDRLEN / sizeof(uint32_t);
	msg->ip.version = 4;
	msg->ip.tos = 0;
	msg->ip.tot_len = htons(IPV4_HDRLEN + ICMP_HDRLEN + datalen);
	msg->ip.id = htons(0);
	msg->ip.frag_off = htons(0);
	msg->ip.ttl = 255;
	msg->ip.proto = IPPROTO_ICMP;
	if (inet_pton (AF_INET, mgr->saddr, &(msg->ip.saddr)) != 1)
	{
		dprintf(STDERR_FILENO, "Error inet_pton().\n");
		exit(FAILURE);
	}
	if (inet_pton (AF_INET, mgr->daddr, &(msg->ip.daddr)) != 1)
	{
		dprintf(STDERR_FILENO, "Error inet_pton().\n");
		exit(FAILURE);
	}
}

int				ping(t_mgr *mgr)
{
	t_echo		echo;
	char		data[IP_MAXPACKET];
	u_short		datalen;

	ft_strcpy(data, "FT ECHO REQUEST");
	datalen = (u_short)ft_strlen(data);
	fill_ip_header(mgr, &echo, datalen);
	//fill_icmp_header();
	return (SUCCESS);
}