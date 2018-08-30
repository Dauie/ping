/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_ping.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlutt <rlutt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 14:26:36 by rlutt             #+#    #+#             */
/*   Updated: 2018/08/29 16:49:52 by rlutt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ping.h"

static void				fill_packet(u_int8_t *packet, t_echopkt *echo)
{
	echo->phdr.icmp.icmp_cksum = 0;
	ft_memcpy(packet, &echo->iphdr, IPV4_HDRLEN);
	packet += IPV4_HDRLEN;
	ft_memcpy(packet, &echo->phdr.icmp, ICMP_HDRLEN);
	packet += ICMP_HDRLEN;
	gettimeofday(&echo->sent, NULL);
	ft_memcpy(packet, &echo->sent, sizeof(struct timeval));
	packet += sizeof(struct timeval);
	ft_memcpy(packet, echo->data, echo->datalen - sizeof(struct timeval));
	packet -= sizeof(struct timeval) + ICMP_HDRLEN;
	echo->phdr.icmp.icmp_cksum = ft_checksum(packet, ICMP_HDRLEN + echo->datalen, TRUE);
	ft_memcpy(packet, &echo->phdr.icmp, ICMP_HDRLEN);
}

int						send_ping(t_mgr *mgr, t_echopkt *echo)
{
	u_int8_t			packet[IP_MAXPACKET];
	size_t				pktlen;

	ft_memset(packet, 0, IP_MAXPACKET);
	fill_packet(packet, echo);
	pktlen = IPV4_HDRLEN + ICMP_HDRLEN + echo->datalen;
	if (sendto(mgr->sock, packet, pktlen, 0, (struct sockaddr *)&mgr->daddr,
			sizeof(struct sockaddr)) < 0)
	{
		dprintf(STDERR_FILENO, "Error sendto().\n");
		exit(FAILURE);
	}
	return (SUCCESS);
}
