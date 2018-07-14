#include "../incl/ping.h"

void	add_type(void *mem, int type)
{
	char	*m;

	m = mem;
	*m = (unsigned char)type;
}

void	add_code(void *mem, int code)
{
	char	*m;

	m = ((char *)mem + CODE_OFFS);
	*m = (unsigned char)code;
}

void	add_checksum(void *mem, int code)
{
	unsigned short	*m;

	m = (unsigned short *)((char *)mem + CHKSM_OFFS);
	*m = (unsigned short)code;
}

void	add_identifier(void *mem, pid_t pid)
{
	unsigned short	*m;

	m = (unsigned short *)((char *)mem + ID_OFFS);
	*m = (unsigned short)pid;
}

void	add_sequence(void *mem, unsigned short *seq)
{
	unsigned short	*m;

	m = (unsigned short *)((char *)mem + ID_OFFS);
	*m = *seq;
	*seq += 1;
}
