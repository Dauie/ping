/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlutt <rlutt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 14:50:06 by rlutt             #+#    #+#             */
/*   Updated: 2018/08/22 15:17:18 by rlutt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incl/ping.h"

t_sigflg			g_sigflgs;

static void				useage(void)
{
	printf("Usage:\tping [-chv]\n"
			"[-c count][-h help][-v verbose]\n");
	exit(SUCCESS);
}

static void				set_option(t_mgr *mgr, char opt, char **av, int *i)
{
	if (opt == 'c')
	{
		mgr->flags.count = TRUE;
		if (!av[*i + 1] || (mgr->count = (size_t)ft_atoi(av[*i + 1])) == 0)
		{
			dprintf(STDERR_FILENO, "ping: invalid count of packets"
					" to transmit.\n");
			useage();
		}
		*i += 1;
	}
	else if (opt == 'v')
		mgr->flags.verbose = TRUE;
	else
	{
		dprintf(STDERR_FILENO, "ping: invalid option -- %c\n", opt);
		useage();
	}
}

static int				parse_arguments(t_mgr *mgr, int ac, char **av)
{
	int					i;

	i = 0;
	while (av[++i])
	{
		if (av[i][0] != '-' || ac == 2)
		{
			if (av[i][1] == 'h')
				useage();
			if (!(mgr->daddr.sin_addr.s_addr = ft_domtoip(av[i], NULL)))
			{
				dprintf(STDERR_FILENO, "ping: cannot resolve"
						" %s: Unknown host\n", av[i]);
				exit(FAILURE);
			}
			ft_strcpy(mgr->domain, inet_ntoa(mgr->daddr.sin_addr));
		}
		else if (av[i][0] == '-' && av[i][1])
			set_option(mgr, av[i][1], av, &i);
	}
	if (!mgr->daddr.sin_addr.s_addr)
		useage();
	return (SUCCESS);
}

int						main(int ac, char **av)
{
	t_mgr				*mgr;

	if (ac == 1)
		useage();
	if (!(mgr = ft_memalloc(sizeof(t_mgr))))
		return (FAILURE);
	init_mgr(mgr);
	if (!(mgr->saddr.sin_addr.s_addr = ft_gethstaddr(NULL, FALSE)))
	{
		dprintf(STDERR_FILENO, "ping: could not find suitable"
				" IP for localhost\n");
		exit(FAILURE);
	}
	parse_arguments(mgr, ac, av);
	create_socket(mgr);
	ft_memset(&g_sigflgs, 0, sizeof(t_sigflg));
	ping(mgr);
	return (SUCCESS);
}
