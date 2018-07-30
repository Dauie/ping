#include "../incl/ping.h"

void		init_mgr(t_mgr *mgr)
{
	if ((mgr->pid = getpid()) < 0)
	{
		dprintf(STDERR_FILENO, "Error getpid()\n");
		exit(FAILURE);
	}
	mgr->uid = getuid();
	mgr->sock = 0;
	mgr->count = 1;
	mgr->seq = 1;
}