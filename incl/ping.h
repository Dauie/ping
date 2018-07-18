#ifndef PING_H
#define PING_H

# define IPV4_HDRLEN 20
# define ICMP_HDRLEN 8
# define IP_MAXPACKET 65535
# define IPV4_ADDR_LEN 16
# define DOMAIN_NAME_LEN 253

#include <errno.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
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

typedef struct		s_icmp_hdr
{
	uint8_t			type;
	uint8_t			code;
	uint16_t		checksum;
	uint16_t		id;
	uint16_t		seq;
}					t_icmp_hdr;

typedef	struct 		s_ip_hdr
{
	uint8_t			ihl: 4,
					version: 4;
	u_int8_t		tos;
	u_int16_t		tot_len;
	u_int16_t		id;
	u_int16_t		frag_off;
	u_int8_t		ttl;
	u_int8_t		proto;
	u_int16_t		checksum;
	u_int32_t		saddr;
	u_int32_t		daddr;
}					t_ip_hdr;

typedef struct		s_echo
{
	t_ip_hdr		ip;
	t_icmp_hdr		icmp_hdr;
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
