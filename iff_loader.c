#include <stdio.h>

#include "iff_loader.h"

ULONG read_dword(FILE* fp)
{
	ULONG val;
	fread((void *)&val, sizeof(val), 1, fp);
	if(is_little_endidan())
		val = _byteswap_ulong(val);

	// fwrite((const void*)&val, sizeof(val), 1, fp);
    return val;
}

unsigned short read_word(FILE* fp)
{
	// if(is_little_endidan())
	// 	val = _byteswap_ushort(val);
	// fwrite((const void*)&val, sizeof(val), 1, fp);
    return (unsigned short)0;
}

UBYTE read_byte(FILE* fp)
{
	// fwrite((const void*)&val, sizeof(val), 1, fp);
    return (UBYTE)0;
}

char* read_chunk_id(FILE* fp)
{
	// fwrite(chunkId, 4, 1, fp);
    return 0;
}
