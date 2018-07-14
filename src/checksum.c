
u_short checksum(u_short *buff, int nwords)
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
	return(~sum);
}