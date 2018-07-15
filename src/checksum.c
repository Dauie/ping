#include "../incl/ping.h"

u_short checksum(u_int16_t *buff, int nwords)
{
	u_long sum;

	sum = 0;
	while (nwords)
	{
		sum += *buff;
		nwords--;
		buff++;
	}
	sum = (sum >> 16) + (sum & 65535);
	sum += (sum >> 16);
	return((u_short)~sum);
}