#include "../incl/ping.h"

int		create_socket(t_mgr *mgr)
{
	if ((mgr->sock = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1)
	{
		dprintf(STDERR_FILENO, "[-]Error creating socket\n");
		return (FAILURE);
	}
	mgr->cslen = sizeof(session->csin);
	return (SUCCESS);
}

