#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "iff_loader.h"
#include "byte_order.h"
#include "rle_decompress.h"

ULONG read_dword(FILE* fp)
{
	ULONG val;
	fread((void *)&val, sizeof(val), 1, fp);
	if(is_little_endidan())
		val = _byteswap_ulong(val);
    return val;
}

unsigned short read_word(FILE* fp)
{
	unsigned short val;
	fread((void *)&val, sizeof(val), 1, fp);
	if(is_little_endidan())
		val = _byteswap_ushort(val);
    return val;
}

UBYTE read_byte(FILE* fp)
{
	UBYTE val;
	fread((void *)&val, sizeof(val), 1, fp);
    return val;
}

char* read_chunk_id(FILE* fp)
{
	char chunk_id[4];
	fread(&chunk_id, 4, 1, fp);
    return strdup(chunk_id);
}

void iff_loadimage(char *filename)
{
	ULONG _dump;

	char *chunk_id;
	unsigned short width;
	unsigned short height;
	UBYTE bpls;
	UBYTE compression;

	UBYTE *body_data = NULL;

	FILE *fp = fopen(filename, "rb");
	if (!fp)
		return;

	// fseek(fp, 0L, SEEK_END);
	// long filesize = ftell(fp);
	// rewind(fp);

	chunk_id = read_chunk_id(fp);
	if (strncmp("FORM", chunk_id, 4) != 0)
	{
		fprintf(stderr, "FORM chunk not found!\n");
		fclose(fp);
		return;
	}
	ULONG chunk_size = read_dword(fp);

	while(1)
	{
		chunk_id = read_chunk_id(fp);
		if (feof(fp))
			break;

		if (strncmp("ILBM", chunk_id, 4) == 0)
		{
			fprintf(stderr, "Found ILBM chunk\n");
		}
		else if (strncmp("BMHD", chunk_id, 4) == 0)
		{
			fprintf(stderr, "Found BMHD chunk\n");
			chunk_size = read_dword(fp);

			width = read_word(fp);
			height = read_word(fp);
			_dump = read_word(fp);
			_dump = read_word(fp);
			bpls = read_byte(fp);
			_dump = read_byte(fp);
			compression = read_byte(fp);
			fprintf(stderr, "image size %ix%ix%i\n", width, height, bpls);
			fprintf(stderr, "Compression = %i\n", compression);

			fseek(fp, chunk_size-11, SEEK_CUR);
		}
		else if (strncmp("CMAP", chunk_id, 4) == 0)
		{
			fprintf(stderr, "Found CMAP chunk\n");
			chunk_size = read_dword(fp);

			fprintf(stderr, "CMAP chunk size %ld\n", chunk_size);

			fseek(fp, chunk_size, SEEK_CUR);
		}
		else if (strncmp("BODY", chunk_id, 4) == 0)
		{
			fprintf(stderr, "Found BODY chunk\n");
			chunk_size = read_dword(fp);

			fprintf(stderr, "BODY chunk size %ld\n", chunk_size);

			if (compression == 0)
			{
				body_data = (UBYTE*)calloc(1, chunk_size);
				fread((void*)body_data, chunk_size, 1, fp);
			}
			else
			{
				UBYTE *compressedData = (UBYTE*)calloc(1, chunk_size);
				
				int uncompressedSize = (width>>3)*height*bpls;
				UBYTE *uncompressedData = (UBYTE*)calloc(1, uncompressedSize);
				rel_decompress(compressedData, uncompressedData, uncompressedSize);
				free((void*)compressedData);

				body_data = uncompressedData;
			}
			
			// TODO: 

			free((void*)body_data);
		}
	}

	fclose(fp);
}