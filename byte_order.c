#include "iff2raw.h"
#include "byte_order.h"

#ifndef _byteswap_ulong
ULONG _byteswap_ulong(ULONG val)
{
	return 
		((val & 0xff000000) >> 24) | 
		((val & 0x00ff0000) >> 8) |
		((val & 0x0000ff00) << 8) |
		((val & 0x000000ff) << 24);
}
#endif

#ifndef _byteswap_ushort
USHORT _byteswap_ushort(USHORT val)
{
	return 
		((val & 0xff00) >> 8) |
		((val & 0x00ff) << 8);
}
#endif

int is_little_endidan()
{
	// Intel = Little endian
	// Motorola, etc. = Big endian
	int n = 1;
	return (*(char*)&n == 1);
}
