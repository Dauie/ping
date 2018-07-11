#ifndef MSG_TYPES_CODES_H
#define MSG_TYPES_CODES_H

typedef enum	e_msg_types
{
	ECHO_RPLY = 0,
	DST_UNRCH = 3,
	SRC_QUNCH = 4,
	REDIRECT = 5,
	ECHO = 8,
	TIME_EXCD = 11,
	PARAM_PRBLM = 12,
	TIME_STMP = 13,
	TSTMP_RPLY = 14,
	INFO_REQ = 15,
	INFO_RPLY = 16

};

/**
* Byte sizes of fields
*
*  1 byte  1 byte  2 bytes       2 bytes
*  ______________________________________________
* | Tnype  | Code  |   Checksum  |   Identifier   |
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
}			s_rcodes;

/**
* TYPE 11 (Time Exceeded Message Types)
**/
typedef enum	e_timexceed_codes
{
	TTL_INTRANSIT,
	TTL_FRAGMENT_EXCEED,
};

/**
* TYPE 4 (Source Quench Message Types)
**/
typedef enum	e_quench_msg_codes
{
	QUENCH_MSG
};

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
};

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
};

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
typedef enum	e_echo_msg_codes
{
	IDENTY_SEQ_MISNG,
};

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
**/
typedef enum e_tstamp_codes
{
	TIMESTAMP_ZERO,
};

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
*
**/

/**
* TYPE 15,16 (Information Request, Information Reply Message)
**/
typedef enum	e_reqst_msg_codes
{
	SEQUENCE_MISNG,
};

#endif