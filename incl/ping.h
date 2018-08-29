/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlutt <rlutt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 14:48:08 by rlutt             #+#    #+#             */
/*   Updated: 2018/08/29 14:53:31 by rlutt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PING_H
# define PING_H

# define ICMP_HDRLEN 8

# include <errno.h>
# include <stdint.h>
# include <stdio.h>
# include <time.h>
# include <signal.h>
# include <sys/time.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <net/if.h>
# include <arpa/inet.h>
# include <netinet/ip.h>
# include <netinet/ip_icmp.h>
# include <ifaddrs.h>

# include "../libft/incl/net_ping.h"
# include "../libft/incl/return_types.h"
# include "../libft/incl/bool.h"
# include "../libft/incl/cnvrsn.h"

/*
** Flags Supported:
**
** -c Stop after sending (and receiving) count ECHO_RESPONSE packets.
**
** -v Verbose output. ICMP packets other than ECHO_RESPONSE that are
**    received are listed.
**
** -h Show help.
*/

typedef struct			s_flags
{
	uint8_t				count: 1;
	uint8_t				verbose: 1;
}						t_flags;

typedef struct			s_stats
{
	float				avg;
	float				mdev;
	float				max;
	float				min;
	size_t				recvd;
	size_t				sent;
	struct timeval		start;
	struct timeval		end;
}						t_stats;

typedef struct			s_manager
{
	pid_t				pid;
	uid_t				uid;
	int					sock;
	size_t				count;
	size_t				seq;
	t_echopkt			echo;
	char				domain[DOMAIN_NAME_LEN];
	struct sockaddr_in	daddr;
	struct sockaddr_in	saddr;
	t_stats				stats;
	t_flags				flags;
}						t_mgr;

typedef struct			s_sigflg
{
	volatile int		timeoflg;
	volatile int		exitflg;
}						t_sigflg;

extern t_sigflg			g_sigflgs;

int						handle_response(struct msghdr *resp,
								struct timeval *now, t_mgr *mgr, ssize_t rbyte);
int						ping(t_mgr *mgr);
void					recv_ping(t_mgr *mgr, struct timeval *now);
int						send_ping(t_mgr *mgr, t_echopkt *echo);
void					sigalrm_handle_timeout(int sig);
void					sigint_handle_exit(int sig);

#endif
