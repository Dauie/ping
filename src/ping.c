/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlutt <rlutt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 14:26:27 by rlutt             #+#    #+#             */
/*   Updated: 2018/08/29 14:49:16 by rlutt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ping.h"

void		init_sockaddr(struct sockaddr_in *sin, t_echopkt *echo)
{
	sin->sin_family = AF_INET;
	sin->sin_addr.s_addr = echo->iphdr.ip_dst.s_addr;
}

static void				print_stats(t_mgr *mgr)
{
	long double		packet_loss;
	long double		duration;

	packet_loss = ft_getpercent(mgr->stats.sent, mgr->stats.recvd);
	duration = ft_timediff_ms(&mgr->stats.end, &mgr->stats.start);
	printf("\n--- %s ping statistics ---\n", mgr->domain);
	printf("%zu packets transmitted, %zu received,"
				" %.2Lf%% packet loss, time %.0Lfms\n",
		mgr->stats.sent, mgr->stats.recvd, packet_loss, duration);
	printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n",
		mgr->stats.min, mgr->stats.avg, mgr->stats.max, mgr->stats.mdev);
}

static int					ping_loop(t_mgr *mgr, t_echopkt *echo)
{
	struct timeval	then;
	struct timeval	now;

	gettimeofday(&then, NULL);
	signal(SIGALRM, sigalrm_handle_timeout);
	signal(SIGINT, sigint_handle_exit);
	while (mgr->count && g_sigflgs.exitflg == FALSE)
	{
		gettimeofday(&now, NULL);
		if (ft_timediff_sec(&then, &now) > 1.0)
		{
			send_ping(mgr, echo);
			mgr->stats.sent++;
			alarm(4);
			if (mgr->flags.count == TRUE)
				mgr->count -= 1;
			echo->phdr.icmp.icmp_seq = ntohs(++mgr->seq);
			gettimeofday(&then, NULL);
		}
		recv_ping(mgr, &now);
	}
	return (SUCCESS);
}

int					ping(t_mgr *mgr)
{
	mgr->echo.data = (u_int8_t *)ft_strdup("         !\"#$%&'()*+,-./01234567");
	mgr->echo.datalen = (u_short)ft_strlen((char *)mgr->echo.data) + sizeof(struct timeval);
	ft_setip_hdr(&mgr->echo.iphdr, 64, IPPROTO_ICMP, mgr->echo.datalen);
	ft_seticmp_hdr(&mgr->echo.phdr.icmp, ICMP_ECHO, (int)mgr->seq, mgr->pid);
	ft_setip_dstsrc(&mgr->echo.iphdr, &mgr->saddr.sin_addr, &mgr->daddr.sin_addr);
	init_sockaddr(&mgr->daddr, &mgr->echo);
	printf("PING %s (%s) %u(%zu) bytes of data.\n",
		mgr->domain, inet_ntoa(mgr->daddr.sin_addr), mgr->echo.datalen, IPV4_HDRLEN + ICMP_HDRLEN +
				mgr->echo.datalen + sizeof(struct timeval));
	gettimeofday(&mgr->stats.start, NULL);
	ping_loop(mgr, &mgr->echo);
	gettimeofday(&mgr->stats.end, NULL);
	print_stats(mgr);
	return (SUCCESS);
}
