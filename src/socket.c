/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlutt <rlutt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 14:49:33 by rlutt             #+#    #+#             */
/*   Updated: 2018/08/22 15:10:12 by rlutt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ping.h"

int					setrecvtimeout(t_mgr *mgr, struct timeval *tout)
{
	if (setsockopt(mgr->sock, SOL_SOCKET, SO_RCVTIMEO,
				tout, sizeof(struct timeval)) < 0)
	{
		dprintf(STDERR_FILENO, "Error setsockopt().\n");
		exit(FAILURE);
	}
	return (SUCCESS);
}

int					setsendtimeout(t_mgr *mgr, struct timeval *tout)
{
	if (setsockopt(mgr->sock, SOL_SOCKET, SO_SNDTIMEO,
				tout, sizeof(struct timeval)) < 0)
	{
		dprintf(STDERR_FILENO, "Error setsockopt().\n");
		exit(FAILURE);
	}
	return (SUCCESS);
}

int					setopt(t_mgr *mgr)
{
	int				on;

	on = 1;
	if (setsockopt(mgr->sock, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) < 0)
	{
		dprintf(STDERR_FILENO, "Error setsockopt()\n");
		exit(FAILURE);
	}
	setsendtimeout(mgr, &(struct timeval){0, 1000});
	setrecvtimeout(mgr, &(struct timeval){0, 1000});
	return (SUCCESS);
}

int					create_socket(t_mgr *mgr)
{
	if ((mgr->sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
	{
		dprintf(STDERR_FILENO, "Error socket().\n");
		exit(FAILURE);
	}
	setopt(mgr);
	return (SUCCESS);
}
