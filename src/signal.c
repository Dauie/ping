#include "../incl/ping.h"

void	sigalrm_handel_timeout(int sig)
{
	if (sig != SIGALRM)
		return ;
	g_sigflgs.timeoflg = TRUE;
}

void	sigint_handel_exit(int sig)
{
	if (sig != SIGINT)
		return ;
	g_sigflgs.exitflg = TRUE;
}

