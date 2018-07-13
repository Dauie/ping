#ifndef MSG_TYPES_CODES_H
#define MSG_TYPES_CODES_H

/**
* Message type codes for each ICMP message type.
**/
// Not going to be used. Already Defined.
typedef enum	e_msg_types
{
	ECHO_RPLY = 0,
	DST_UNRCH = 3,
	SRC_QUNCH = 4,
	REDIRECT = 5,
	ECHO_REQ = 8,
	TIME_EXCD = 11,
	PARAM_PRBLM = 12,
	TIME_STMP = 13,
	TSTMP_RPLY = 14,
	INFO_REQ = 15,
	INFO_RPLY = 16

}				t_msg_types;

/**
* Offset size of message sections from start of ICMP packet.
**/
typedef enum	e_sec_offsets
{
	TYPE_OFFS = 0,
	CODE_OFFS = 1,
	CHKSM_OFFS = 2,
	ID_OFFS = 4,
	PTR_OFFS = 4,
	GTWY_ADDR_OFFS = 4,
	SEQ_OFFS = 6,
	INET_HDR_OFFS = 8,
	ORIG_TSTMP_OFFS = 8,
	RECV_TSTMP_OFFS = 12,
	TRANS_TSTMP_OFFS = 16,
}				t_sec_offsets;

/**
* Byte sizes of fields
*
*  1 byte  1 byte  2 bytes       2 bytes
*  ______________________________________________
* | Type  | Code  |   Checksum  |   Identifier   |
*  ----------------------------------------------
*
* 4 bytes
*  _____________________________
* | Any Address Section         |
*  ------------------------------
*
*  Checksum:
*  The checksum is the 16-bit ones's complement of the one's
*  complement sum of the ICMP message starting with the ICMP Type.
*  For computing the checksum , the checksum field should be zero.
*  This checksum may be replaced in the future.
*
**/

/**
* Sizes for different ICMP message types
* DFLT_MSG_SZ is used for everything besides timestamp, and info request.
**/
typedef enum	e_msg_sizes
{
	INFOREQ_MSG_SZ = 8,
	DFLT_MSG_SZ = 12,
	TIMESTMP_MSG_SZ = 32,
}				t_msg_sizes;


/**
*  Destination Unreachable MSG && Time Exceeded MSG &&
*   Source Quench MSG &&
*
*   0                   1                   2                   3
*   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
*  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*  |     Type      |     Code      |          Checksum             |
*  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*  |                             unused                            |
*  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*  |      Internet Header + 64 bits of Original Data Datagram      |
*  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*
*/

/**
* TYPE 3 (Destination Unreachable Message Types)
**/
typedef enum	e_unreachable_codes
{
	NET_UNREACH,
	HST_UNREACH,
	PROTO_UNREACH,
	PORT_UNREACH,
	FRAG_NEEDED,
	ROUT_FAIL
}				t_unreachable_codes;

/**
* TYPE 11 (Time Exceeded Message Types)
**/
typedef enum	e_timexceed_codes
{
	TTL_INTRANSIT,
	TTL_FRAGMENT_EXCEED,
}				t_timexceed_codes;

/**
* TYPE 4 (Source Quench Message Types)
**/
typedef enum	e_quench_codes
{
	QUENCH_MSG
}				t_qunch_codes;

/**
*  Parameter Problem Message
*
*   0                   1                   2                   3
*   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
*  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*  |     Type      |     Code      |          Checksum             |
*  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*  |    Pointer    |                   unused                      |
*  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*  |      Internet Header + 64 bits of Original Data Datagram      |
*  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
**/

/**
* TYPE 12 (Parameter Problem Message Types)
**/
typedef enum	e_param_issue_codes
{
	PTR_INDICATE_ERR,
}				t_param_issue_codes;

/**
*  Redirect Message
*
*   0                   1                   2                   3
*   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
*  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*  |     Type      |     Code      |          Checksum             |
*  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*  |                 Gateway Internet Address                      |
*  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*  |      Internet Header + 64 bits of Original Data Datagram      |
*  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
**/

/**
* TYPE 5 (Redirect Message Types)
**/
typedef enum	e_redirect_msg_codes
{
	REDIR_NET,
	REDIR_HST,
	REDIR_SRVCT_NET,
	REDIR_SRVCT_HST,
}				t_redirect_msg_codes;

/**
*  Echo or Echo Reply Message
*   0                   1                   2                   3
*   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
*  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*  |     Type      |     Code      |          Checksum             |
*  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*  |           Identifier          |        Sequence Number        |
*  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*  |     Data ...
*  +-+-+-+-+-
**/

/**
* TYPE 8,0 (Echo Messages, Echo Reply Messages)
**/
typedef enum	e_warn_codes
{
	IDNTFR_SEQ_MISNG,
}				t_warn_codes;

/**
*  Timestamp or Timestamp Reply Message
*
*    0                   1                   2                   3
*    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
*   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*   |     Type      |      Code     |          Checksum             |
*   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*   |           Identifier          |        Sequence Number        |
*   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*   |     Originate Timestamp                                       |
*   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*   |     Receive Timestamp                                         |
*   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*   |     Transmit Timestamp                                        |
*   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
**/

/**
* TYPE 13,14 (Timestamp Message, Timestamp Reply Message)
* Refer to Echo Message Code Types
**/


/**
*  Information Request or Information Reply Message
*
*   0                   1                   2                   3
*   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
*  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*  |     Type      |      Code     |          Checksum             |
*  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*  |           Identifier          |        Sequence Number        |
*  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
* Refer to Echo Message Code Types
**/

/**
* TYPE 15,16 (Information Request, Information Reply Message)
* Refer to Echo Message Code Types
**/


/**-----------------------------------------------------------------------------
*   IP Header
*
*   0                   1                   2                   3
*   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
*  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*  |Version|  IHL  |Type of Service|          Total Length         |
*  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*  |         Identification        |Flags|      Fragment Offset    |
*  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*  |  Time to Live |    Protocol   |         Header Checksum       |
*  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*  |                       Source Address                          |
*  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*  |                    Destination Address                        |
*  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*  |                    Options                    |    Padding    |
*  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
**/

#endif
