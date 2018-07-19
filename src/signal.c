#include "../incl/ping.h"

void	alarm_handel_timeout(int sig)
{
	if (sig != SIGALRM)
		return ;
	signal(SIGALRM, SIG_IGN);
	signal(SIGALRM, alarm_handel_timeout);
	printf("Request timeout for icmp_seg %zu\n", g_mgr->seq);
	ping_loop(g_mgr, &g_mgr->echo, &g_mgr->sin);
}