/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlutt <rlutt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 14:48:08 by rlutt             #+#    #+#             */
/*   Updated: 2018/08/17 16:58:21 by rlutt            ###   ########.fr       */
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

# include "../libft/incl/net.h"
# include "../libft/incl/return_types.h"
# include "../libft/incl/bool.h"
# include "../libft/incl/cnvrsn.h"

/**
* Flags Supported:
*
* -c Stop after sending (and receiving) count ECHO_RESPONSE packets.
*
* -v Verbose output. ICMP packets other than ECHO_RESPONSE that are
*    received are listed.
*
* -h Show help.
**/

typedef enum			e_msg_types
{
	TYPE_ECHO_RPLY = 0,
	TYPE_DST_UNRCH = 3,
	TYPE_TIME_EXCD = 11,
	TYPE_PARAM_PRBLM = 12,
}						t_msg_types;

typedef struct			s_echo
{
	struct ip			ip;
	struct icmp			icmp;
	struct timeval		time;
	char				data[256];
	u_short				datalen;
}						t_echo;

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
	struct sockaddr_in	sin;
	t_echo				echo;
	char				domain[DOMAIN_NAME_LEN];
	char				daddr[INET_ADDRSTRLEN];
	char				saddr[INET_ADDRSTRLEN];
	t_stats				stats;
	t_flags				flags;
}						t_mgr;

typedef struct			s_sigflg
{
	volatile int		timeoflg;
	volatile int		exitflg;
}						t_sigflg;

t_sigflg				g_sigflgs;

u_int16_t				checksum(void *data, size_t len);
int						create_socket(t_mgr *mgr);
int						handle_response(struct msghdr *resp,
								struct timeval *now, t_mgr *mgr, ssize_t rbyte);
void					init_icmp_header(t_mgr *mgr, struct icmp *icmp);
void					init_ip_header(t_mgr *mgr, struct ip *ip, t_echo *echo);
void					init_mgr(t_mgr *mgr);
void					init_sockaddr(struct sockaddr_in *sin, t_echo *echo);
int						ping(t_mgr *mgr);
int						ping_loop(t_mgr *mgr, t_echo *echo);
void					recv_ping(t_mgr *mgr, struct timeval *now);
int						send_ping(t_mgr *mgr, t_echo *echo);
int						setrecvtimeout(t_mgr *mgr, struct timeval *tout);
void					sigalrm_handle_timeout(int sig);
void					sigint_handle_exit(int sig);
long double				time_diff_ms(struct timeval *then, struct timeval *now);
long double				time_diff_sec(struct timeval *then,
										struct timeval *now);
float					get_percentage(size_t a, size_t b);

#endif
