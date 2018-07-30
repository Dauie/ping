#include "../incl/ping.h"

int 	setrecvtimeout(t_mgr *mgr, struct timeval *tout)
{
	if (setsockopt(mgr->sock, SOL_SOCKET, SO_RCVTIMEO, tout, sizeof(struct timeval)) < 0)
	{
		dprintf(STDERR_FILENO, "Error setsockopt()\n");
		exit(FAILURE);
	}
	return (SUCCESS);
}

int		setsendtimeout(t_mgr *mgr, struct timeval *tout)
{
	if (setsockopt(mgr->sock, SOL_SOCKET, SO_SNDTIMEO, tout, sizeof(struct timeval)) < 0)
	{
		dprintf(STDERR_FILENO, "Error setsockopt()\n");
		exit(FAILURE);
	}
	return (SUCCESS);
}

int		setopt(t_mgr *mgr)
{
	int on;
	struct timeval tout;

	on = 1;
	tout.tv_sec = 0;
	tout.tv_usec = 1000;
	if (setsockopt(mgr->sock, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) < 0)
	{
		dprintf(STDERR_FILENO, "Error setsockopt()\n");
		exit(FAILURE);
	}
	setsendtimeout(mgr, &tout);
	setrecvtimeout(mgr, &tout);
	return (SUCCESS);
}

int		create_socket(t_mgr *mgr)
{
	if ((mgr->sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
	{
		dprintf(STDERR_FILENO, "Error socket() - %s.\n", strerror(errno));
		exit(FAILURE);
	}
	setopt(mgr);
	return (SUCCESS);
}

