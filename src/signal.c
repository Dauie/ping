#include "../incl/ping.h"

void	alarm_handel_timeout(int sig)
{
	if (sig != SIGALRM)
		return ;
	printf("Request timeout for icmp_seg %zu\n", g_mgr->seq);
	signal(SIGALRM, alarm_handel_timeout);
	ping_loop(g_mgr, &g_mgr->echo, &g_mgr->sin);
}