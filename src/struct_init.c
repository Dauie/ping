#include "../incl/ping.h"

void		init_mgr(t_mgr *mgr)
{
	g_mgr = mgr;
	if ((mgr->pid = getpid()) < 0)
	{
		dprintf(STDERR_FILENO, "Error getpid()\n");
		exit(FAILURE);
	}
	mgr->uid = getuid();
	mgr->sock = 0;
	mgr->count = 1;
	mgr->seq = 1;
	ft_memset(mgr->domain, 0, DOMAIN_NAME_LEN);
	ft_memset(mgr->saddr, 0, IPV4_ADDR_LEN);
	ft_memset(mgr->daddr, 0, IPV4_ADDR_LEN);
	ft_memset(&mgr->stats, 0, sizeof(t_stats));
	ft_memset(&mgr->flags, 0, sizeof(t_flags));
}