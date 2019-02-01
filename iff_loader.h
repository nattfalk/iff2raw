#ifndef _INC_IFF_LOADER_
#define _INC_IFF_LOADER_

#include "iff2raw.h"
#include "iff_image.h"

ULONG read_dword(FILE* fp);
unsigned short read_word(FILE* fp);
UBYTE read_byte(FILE* fp);
char* read_chunk_id(FILE* fp);

int iff_loadimage(_options *opts, iff_image_data* image_data);

#endif
