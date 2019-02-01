#pragma once

#ifndef _INC_IFF_LOADER_
#define _INC_IFF_LOADER_

#include "iff2raw.h"

ULONG read_dword(FILE* fp);
unsigned short read_word(FILE* fp);
UBYTE read_byte(FILE* fp);
char* read_chunk_id(FILE* fp);

void iff_loadimage(char *filename);

#endif
