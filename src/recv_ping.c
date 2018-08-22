/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recv_ping.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlutt <rlutt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 14:31:59 by rlutt             #+#    #+#             */
/*   Updated: 2018/08/22 13:18:57 by rlutt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ping.h"

static struct msghdr	*init_msghdr(void)
{
	struct iovec	*iov;
	struct msghdr	*resp;
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

static void				clean_msghdr(struct msghdr **msg)
{
	free((*msg)->msg_iov->iov_base);
	free((*msg)->msg_iov);
	free(*msg);
}

static void				print_timeout_msg(t_mgr *mgr)
{
	if (g_sigflgs.timeoflg == TRUE)
	{
		printf("Request timeout for icmp_seq %zu\n", mgr->seq - 5);
		g_sigflgs.timeoflg = FALSE;
	}
}

void					recv_ping(t_mgr *mgr, struct timeval *now)
{
	struct msghdr		*resp;
	ssize_t				rbyte;

	resp = init_msghdr();
	if ((rbyte = recvmsg(mgr->sock, resp, MSG_DONTWAIT)) <= 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
		{
			clean_msghdr(&resp);
			return ;
		}
		dprintf(STDERR_FILENO, "Error recvmsg().\n");
		exit(FAILURE);
	}
	else
	{
		alarm(0);
		mgr->stats.recvd++;
		handle_response(resp, now, mgr, rbyte);
	}
	print_timeout_msg(mgr);
	clean_msghdr(&resp);
}
