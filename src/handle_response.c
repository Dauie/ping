/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_response.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlutt <rlutt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 16:10:30 by rlutt             #+#    #+#             */
/*   Updated: 2018/08/29 14:21:50 by rlutt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ping.h"

static void			update_minmaxavg(t_stats *stats, float ms)
{
	if (stats->min == 0)
		stats->min = ms;
	if (stats->max == 0)
		stats->max = ms;
	if (stats->avg == 0)
		stats->avg = ms;
	if (ms > stats->max)
		stats->max = ms;
	if (ms < stats->min)
		stats->min = ms;
	stats->avg = (stats->avg + ms) / 2;
	stats->mdev += (ms - stats->avg) / 2.0f;
	if (stats->mdev < 0)
		stats->mdev *= -1;
}

int					handle_response(struct msghdr *resp, struct timeval *now,
									t_mgr *mgr, ssize_t rbyte)
{
	struct icmp		*icmp;
	struct in_addr	*src;
	float			ms;
	char			addr[INET_ADDRSTRLEN];
	u_short			seq;

	icmp = (struct icmp *)&((u_int8_t *)resp->msg_control)[IPV4_HDRLEN];
	ms = (float)ft_timediff_ms(now, (struct timeval *)&((u_int8_t *)
			resp->msg_control)[IPV4_HDRLEN + ICMP_HDRLEN]);
	update_minmaxavg(&mgr->stats, ms);
	seq = ntohs((u_short)((struct icmp *)&((u_int8_t *)
			resp->msg_control)[IPV4_HDRLEN])->icmp_hun.ih_idseq.icd_seq);
	src = &((struct ip *)resp->msg_control)->ip_src;
	inet_ntop(AF_INET, src, addr, INET_ADDRSTRLEN);
	if (icmp->icmp_type == ICMP_ECHOREPLY)
		printf("%zu bytes from %s: icmp_seq=%u ttl=%i time=%.2f ms\n",
			rbyte - IPV4_HDRLEN, addr, seq, ((struct ip *)resp->msg_control)->ip_ttl, ms);
	else if (icmp->icmp_type == ICMP_UNREACH && mgr->flags.verbose == TRUE)
		printf("From %s icmp_seq=%u Destination Host Unreachable\n", addr, seq);
	else if (icmp->icmp_type == ICMP_TIMXCEED && mgr->flags.verbose == TRUE)
		printf("From %s icmp_seq=%u Time to live exceeded\n", addr, seq);
	else if (icmp->icmp_type == ICMP_PARAMPROB && mgr->flags.verbose == TRUE)
		printf("From %s icmp_seq=%u Parameter problem.\n", addr, seq);
	return (SUCCESS);
}
