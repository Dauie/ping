#include "../incl/ping.h"

u_int16_t checksum(void *buff, size_t nwords)
{
	u_int8_t	*data;
	u_int16_t	word;
	u_int32_t	accum;
	size_t		i;

	data = (uint8_t *)buff;
	accum = 0xffff;
	i = 0;
	while (i < nwords)
	{
		ft_memcpy(&word, data + i, 2);
		accum += ntohs(word);
		if (accum > 0xffff)
			accum -= 0xffff;
		i++;
	}
	if (nwords == 1)
	{
		word = 0;
		ft_memcpy(&word, data + nwords - 1, 1);
		accum += ntohs(word);
		if (accum > 0xffff)
			accum -= 0xffff;
	}
}