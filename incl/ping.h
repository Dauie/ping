#ifndef PING_H
#define PING_H

# define IPV4_ADDR_LEN 16
# define DOMAIN_NAME_LEN 253

#include <arpa/inet.h>
#include <netdb.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socket.h>


#include "./msg_types_codes.h"
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

//typedef unsigned short u_short;

typedef struct		s_icmp_hdr
{
	uint8_t			type;
	uint8_t			code;
	u_short			checksum;
	u_short			id;
	u_short			seq;
}					t_icmp_hdr;

typedef	struct 		s_ip_hdr
{
	u_char			hl;
	u_char			tos;
	short 			totlen;
	short 			flags;
	u_char 			ttl;
	u_char 			proto;
	u_short 		checksum;
	struct in_addr	src;
	struct in_addr	dst;
}					t_ip_hdr;

typedef struct		s_echo_reply
{
	t_ip_hdr		ip_hdr;
	t_icmp_hdr		icmp_hdr;
	char 			pad[256];
}					t_echorply;

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
	int 			sock;
	unsigned short	seq;
	size_t			count;
	char 			domain[DOMAIN_NAME_LEN];
	char			addr[IPV4_ADDR_LEN];
	t_stats			stats;
	t_flags			flags;
}					t_mgr;

void				add_type(void *mem, int type);
void				add_code(void *mem, int code);
void				add_checksum(void *mem, int code);
void				add_identifier(void *mem, pid_t pid);
void				add_sequence(void *mem, unsigned short *seq);
int					create_socket(t_mgr *mgr);
void				init_mgr(t_mgr *mgr);
int					ping(t_mgr *mgr);

#endif
