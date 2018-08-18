/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlutt <rlutt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 14:49:21 by rlutt             #+#    #+#             */
/*   Updated: 2018/08/17 14:49:21 by rlutt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ping.h"

void	sigalrm_handle_timeout(int sig)
{
	if (sig != SIGALRM)
		return ;
	g_sigflgs.timeoflg = TRUE;
}

void	sigint_handle_exit(int sig)
{
	if (sig != SIGINT)
		return ;
	g_sigflgs.exitflg = TRUE;
}
