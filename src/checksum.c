#include "../incl/ping.h"

u_int16_t checksum(void *data, size_t len)
{
	u_int16_t	word;
	u_int32_t	accum;
	size_t		i;

	accum = 0xffff;
	i = 0;
	while (i + 1 < len)
	{
		ft_memcpy(&word, (uint8_t *)data + i, 2);
		accum += ntohs(word);
		if (accum > 0xffff)
			accum -= 0xffff;
		i += 2;
	}
	if (len & 1)
	{
		word = 0;
		ft_memcpy(&word, (uint8_t *)data + len - 1, 1);
		accum += ntohs(word);
		if (accum > 0xffff)
			accum -= 0xffff;
	}
	return (ntohs(~accum));
}