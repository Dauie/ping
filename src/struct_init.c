/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlutt <rlutt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 14:49:43 by rlutt             #+#    #+#             */
/*   Updated: 2018/08/17 14:49:43 by rlutt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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