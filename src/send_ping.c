/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_ping.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlutt <rlutt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 14:26:36 by rlutt             #+#    #+#             */
/*   Updated: 2018/08/17 14:29:20 by rlutt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ping.h"

static u_int16_t		update_checksum(t_echo *echo, u_int8_t *packet)
{
	return (checksum((packet + IPV4_HDRLEN),
					 (ICMP_HDRLEN + sizeof(echo->time) + echo->datalen)));
}

static void				fill_packet(u_int8_t *packet, t_echo *echo)
{
	echo->icmp.icmp_cksum = 0;
	ft_memcpy(packet, &echo->ip, IPV4_HDRLEN);
	ft_memcpy((packet + IPV4_HDRLEN), &echo->icmp, ICMP_HDRLEN);
	gettimeofday(&echo->time, NULL);
	ft_memcpy((packet + IPV4_HDRLEN + ICMP_HDRLEN),
			  &echo->time, sizeof(echo->time));
	ft_memcpy((packet + IPV4_HDRLEN + ICMP_HDRLEN + sizeof(echo->time)),
			  echo->data, echo->datalen);
	echo->icmp.icmp_cksum = update_checksum(echo, packet);
	ft_memcpy(packet + IPV4_HDRLEN, &echo->icmp, ICMP_HDRLEN);
}

int						send_ping(t_mgr *mgr, t_echo *echo)
{
	u_int8_t			packet[IP_MAXPACKET];

	ft_memset(packet, 0, IP_MAXPACKET);
	fill_packet(packet, echo);
	if (sendto(mgr->sock, packet, (IPV4_HDRLEN + ICMP_HDRLEN +
								   sizeof(echo->time) + echo->datalen), 0,
			   (struct sockaddr *)&mgr->sin, sizeof(struct sockaddr)) < 0)
	{
		dprintf(STDERR_FILENO, "Error sendto(). %s\n", strerror(errno));
		exit(FAILURE);
	}
	return (SUCCESS);
}
