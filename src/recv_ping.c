/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recv_ping.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlutt <rlutt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 14:31:59 by rlutt             #+#    #+#             */
/*   Updated: 2018/08/17 14:33:30 by rlutt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ping.h"

struct msghdr				*init_msghdr()
{
	struct iovec			*iov;
	struct msghdr			*resp;
	u_int8_t		*resp_data;

	if (!(resp = ft_memalloc(sizeof(struct msghdr))))
		return (NULL);
	if (!(resp_data = ft_memalloc(256)))
		return (NULL);
	if (!(iov = ft_memalloc(sizeof(struct iovec))))
		return (NULL);
	iov->iov_base = resp_data;
	iov->iov_len = 256;
	ft_memset(resp, 0, sizeof(struct msghdr));
	resp->msg_iov = iov;
	resp->msg_iovlen = 1;
	resp->msg_control = resp_data;
	resp->msg_controllen = sizeof(resp_data);
	return (resp);
}

void				clean_msghdr(struct msghdr **msg)
{
	free((*msg)->msg_iov->iov_base);
	free((*msg)->msg_iov);
	free(*msg);
}

void				update_minmaxavg(t_stats *stats, float ms)
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
}

int 				handle_response(struct msghdr *resp, struct timeval *now,
								   t_mgr *mgr, ssize_t rbyte)
{
	struct icmp		*icmp;
	struct timeval	*then;
	struct in_addr	*src;
	float			ms;
	char 			addr[INET_ADDRSTRLEN];
	u_short			seq;
	char 			ttl;

	icmp = (struct icmp *)&((u_int8_t *)resp->msg_control)[IPV4_HDRLEN];
	then = (struct timeval *)&((u_int8_t *)
			resp->msg_control)[IPV4_HDRLEN + ICMP_HDRLEN];
	ms = (float)time_diff_ms(now, then);
	update_minmaxavg(&mgr->stats, ms);
	seq = ntohs((u_short)((struct icmp *)&((u_int8_t *)
			resp->msg_control)[IPV4_HDRLEN])->icmp_hun.ih_idseq.icd_seq);
	src = &((struct ip *)resp->msg_control)->ip_src;
	inet_ntop(AF_INET, src, addr, INET_ADDRSTRLEN);
	ttl = ((struct ip *)resp->msg_control)->ip_ttl;
	if (icmp->icmp_type == TYPE_ECHO_RPLY)
		printf("%zu bytes from %s: icmp_seq=%u ttl=%i time=%.2f ms\n",
			   rbyte, addr, seq, (int)ttl, ms);
	else if (icmp->icmp_type == TYPE_DST_UNRCH && mgr->flags.verbose == TRUE)
		printf("From %s icmp_seq=%u Destination Host Unreachable\n", addr, seq);
	else if (icmp->icmp_type == TYPE_TIME_EXCD && mgr->flags.verbose == TRUE)
		printf("From %s icmp_seq=%u Time to live exceeded\n", addr, seq);
	else if (icmp->icmp_type == TYPE_PARAM_PRBLM && mgr->flags.verbose == TRUE)
		printf("From %s icmp_seq=%u Parameter problem.\n", addr, seq);
	return (SUCCESS);
}

void				recv_ping(t_mgr *mgr, struct timeval *now)
{
	struct msghdr	*resp;
	ssize_t			rbyte;

	resp = init_msghdr();
	if ((rbyte = recvmsg(mgr->sock, resp, MSG_DONTWAIT)) < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
		{
			clean_msghdr(&resp);
			return;
		}
		dprintf(STDERR_FILENO, "Error recvmsg().%s\n", strerror(errno));
		exit(FAILURE);
	}
	else
	{
		alarm(0);
		mgr->stats.recvd++;
		handle_response(resp, now, mgr, rbyte);
	}
	if (g_sigflgs.timeoflg == TRUE)
	{
		printf("Request timeout for icmp_seq %zu\n", mgr->seq - 5);
		g_sigflgs.timeoflg = FALSE;
	}
	clean_msghdr(&resp);
}