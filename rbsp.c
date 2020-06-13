#include "stream.h"
#include "rbsp.h"

int rbsp_trailing_bits(void)
{
	char c;

	c = stream_get_one_bit();
	if (c != 1)
	{
		printf("failed to process rbsp trailing bits\n");
		return -1;
	}

	while (! stream_byte_aligned())
	{
		c = stream_get_one_bit();
		if(c != 0)
		{
			printf("failed to process rbsp trailing bits\n");
			return -1;
		}
	}

	return 0;

}

int more_rbsp_data(void)
{

}

