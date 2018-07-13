#include "../incl/ping.h"

void		init_mgr(t_mgr *mgr)
{
	mgr->pid = 0;
	mgr->seq = 1;
	mgr->uid = 0;
	mgr->sock = 0;
	mgr->count = 0;
	ft_memset(mgr->domain, 0, DOMAIN_NAME_LEN);
	ft_memset(mgr->addr, 0, IPV4_ADDR_LEN);
	ft_memset(&mgr->stats, 0, sizeof(t_stats));
	ft_memset(&mgr->flags, 0, sizeof(t_flags));
}