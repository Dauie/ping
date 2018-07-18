#ifndef PING_H
#define PING_H

//# define IPV4_HDRLEN sizeof(struct ip)
//# define ICMP_HDRLEN sizeof(struct icmp)
#define IPV4_HDRLEN 20         // IPv4 header length
#define ICMP_HDRLEN 8         // ICMP header length for echo request, excludes data
# define IP_MAXPACKET 65535
# define IPV4_ADDR_LEN sizeof(struct sockaddr_in)
# define DOMAIN_NAME_LEN 253

#include <errno.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdio.h>
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
	TYPE_SRC_QUNCH = 4,
	TYPE_REDIRECT = 5,
	TYPE_ECHO_REQ = 8,
	TYPE_TIME_EXCD = 11,
	TYPE_PARAM_PRBLM = 12,
	TYPE_TIME_STMP = 13,
	TYPE_TSTMP_RPLY = 14,
	TYPE_INFO_REQ = 15,
	TYPE_INFO_RPLY = 16

}				t_msg_types;

typedef struct		s_echo
{
	struct ip		ip;
	struct icmp		icmp;
	char			data[256];
}					t_echo;

typedef struct		s_flags
{
	uint8_t			count: 1;
	uint8_t			verbose: 1;
}					t_flags;

typedef struct		s_stats
{
	float			avg;
	float			max;
	float			min;
	size_t			recvd;
	size_t			sent;
}					t_stats;

typedef struct		s_manager
{
	pid_t			pid;
	uid_t			uid;
	int				sock;
	unsigned short	seq;
	size_t			count;
	char			domain[DOMAIN_NAME_LEN];
	char			daddr[IPV4_ADDR_LEN];
	char 			saddr[IPV4_ADDR_LEN];
	t_stats			stats;
	t_flags			flags;
}					t_mgr;

void				add_type(void *mem, int type);
void				add_code(void *mem, int code);
void				add_checksum(void *mem, int code);
void				add_identifier(void *mem, pid_t pid);
u_int16_t			checksum(void *data, size_t len);
void				add_sequence(void *mem, unsigned short *seq);
int					create_socket(t_mgr *mgr);
void				init_mgr(t_mgr *mgr);
int					ping(t_mgr *mgr);

#endif
