/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlutt <rlutt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 14:26:27 by rlutt             #+#    #+#             */
/*   Updated: 2018/08/17 16:03:05 by rlutt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ping.h"

void				print_stats(t_mgr *mgr)
{
	long double		packet_loss;
	long double		duration;

	packet_loss = get_percentage(mgr->stats.sent, mgr->stats.recvd);
	duration = time_diff_ms(&mgr->stats.end, &mgr->stats.start);
	printf("\n--- %s ping statistics ---\n", mgr->daddr);
	printf("%zu packets transmitted, %zu received,"
				" %.2Lf%% packet loss, time %.0Lfms\n",
		mgr->stats.sent, mgr->stats.recvd, packet_loss, duration);
	printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n",
		mgr->stats.min, mgr->stats.avg, mgr->stats.max, mgr->stats.mdev);
}

static int			ping_loop(t_mgr *mgr, t_echo *echo)
{
	struct timeval	then;
	struct timeval	now;

	gettimeofday(&then, NULL);
	signal(SIGALRM, sigalrm_handle_timeout);
	signal(SIGINT, sigint_handle_exit);
	while (mgr->count && g_sigflgs.exitflg == FALSE)
	{
		gettimeofday(&now, NULL);
		if (time_diff_sec(&then, &now) > 1.0)
		{
			send_ping(mgr, echo);
			mgr->stats.sent++;
			alarm(4);
			if (mgr->flags.count == TRUE)
				mgr->count -= 1;
			echo->icmp.icmp_hun.ih_idseq.icd_seq = ntohs(++mgr->seq);
			gettimeofday(&then, NULL);
		}
		recv_ping(mgr, &now);
	}
	return (SUCCESS);
}

int					ping(t_mgr *mgr)
{
	ft_strcpy(mgr->echo.data, "                !\"#$%&'()*+,-./01234567");
	mgr->echo.datalen = (u_short)ft_strlen(mgr->echo.data);
	init_ip_header(mgr, &mgr->echo.ip, &mgr->echo);
	init_icmp_header(mgr, &mgr->echo.icmp);
	init_sockaddr(&mgr->sin, &mgr->echo);
	printf("PING %s (%s) %zu(%zu) bytes of data.\n",
		mgr->domain, mgr->daddr, mgr->echo.datalen + sizeof(struct timeval),
			IPV4_HDRLEN + ICMP_HDRLEN +
				mgr->echo.datalen + sizeof(struct timeval));
	gettimeofday(&mgr->stats.start, NULL);
	ping_loop(mgr, &mgr->echo);
	gettimeofday(&mgr->stats.end, NULL);
	print_stats(mgr);
	return (SUCCESS);
}
