#include "../incl/ping.h"


int		setopt(t_mgr *mgr)
{
	int on;
	struct timeval tout;

	on = 1;
	tout.tv_sec = 1;
	if (setsockopt(mgr->sock, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) < 0)
	{
		dprintf(STDERR_FILENO, "Error setsockopt()\n");
		exit(FAILURE);
	}
	if (setsockopt(mgr->sock, IPPROTO_ICMP, SO_RCVTIMEO, (char *)&tout, sizeof(struct timeval)) < 0)
	{
		dprintf(STDERR_FILENO, "Error setsockopt()\n");
		exit(FAILURE);
	}
	return (SUCCESS);
}

int		create_socket(t_mgr *mgr)
{
	if ((mgr->sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0)
	{
		dprintf(STDERR_FILENO, "Error socket() - %s.\n", strerror(errno));
		exit(FAILURE);
	}
	setopt(mgr);
	return (SUCCESS);
}

