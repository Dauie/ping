#include "../incl/ping.h"

void	alarm_handel_timeout(int sig)
{
	if (sig != SIGALRM)
		return ;
	g_toflg = TRUE;
}