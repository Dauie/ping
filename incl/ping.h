#ifndef PING_H
#define PING_H

#define IPV4_HDRLEN 20         // IPv4 header length
#define ICMP_HDRLEN 8         // ICMP header length for echo request, excludes data
# define IP_MAXPACKET 65535
# define IPV4_ADDR_LEN sizeof(struct sockaddr_in)
# define DOMAIN_NAME_LEN 253

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <ifaddrs.h>


#include "../libft/incl/return_types.h"
#include "../libft/incl/bool.h"
#include "../libft/incl/cnvrsn.h"

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

typedef enum	e_msg_types
{
	TYPE_ECHO_RPLY = 0,
	TYPE_DST_UNRCH = 3,
	TYPE_TIME_EXCD = 11,
	TYPE_PARAM_PRBLM = 12,
}				t_msg_types;

typedef struct timeval	timeval;
typedef struct in_addr	in_addr;
typedef struct ip		ip;
typedef struct icmp		icmp;
typedef struct msghdr	msghdr;
typedef struct iovec	iovec;
typedef struct sockaddr	sockaddr;
typedef struct sockaddr_in sockaddr_in;

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
	char				daddr[IPV4_ADDR_LEN];
	char				saddr[IPV4_ADDR_LEN];
	t_stats				stats;
	t_flags				flags;
}						t_mgr;

typedef struct 			s_sigflg
{
	volatile int		timeoflg;
	volatile int		exitflg;
}						t_sigflg;

t_sigflg				g_sigflgs;

void					add_type(void *mem, int type);
void					add_code(void *mem, int code);
void					add_checksum(void *mem, int code);
void					add_identifier(void *mem, pid_t pid);
u_int16_t				checksum(void *data, size_t len);
void					add_sequence(void *mem, unsigned short *seq);
int						create_socket(t_mgr *mgr);
void					init_mgr(t_mgr *mgr);
int						ping(t_mgr *mgr);
int 					ping_loop(t_mgr *mgr, t_echo *echo);
void					sigalrm_handel_timeout(int sig);
void					sigint_handel_exit(int sig);
#endif
