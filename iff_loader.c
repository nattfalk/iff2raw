#pragma GCC diagnostic ignored "-Wunused-but-set-variable"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "iff2raw.h"
#include "iff_loader.h"
#include "byte_order.h"
#include "rle_decompress.h"
#include "iff_image.h"

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

int iff_loadimage(_options *opts, iff_image_data* image_data)
{
	ULONG _dump;

	char *chunk_id;
	UBYTE compression;

	UBYTE *body_data = NULL;

	if (opts->verbose_mode)
		fprintf(stderr, "IFF loader\n");

	if (!opts->input_filename)
	{
		fprintf(stderr, "No input filename specified\n");
		return S_ERR;
	}	

	FILE *fp = fopen(opts->input_filename, "rb");
	if (!fp)
	{
		fprintf(stderr, "Could not open input file %s\n", opts->input_filename);
		return S_ERR;
	}

	chunk_id = read_chunk_id(fp);
	if (strncmp("FORM", chunk_id, 4) != 0)
	{
		fprintf(stderr, "FORM chunk not found!\n");
		fclose(fp);
		return S_ERR;
	}
	ULONG chunk_size = read_dword(fp);

	while(1)
	{
		chunk_id = read_chunk_id(fp);
		if (feof(fp))
			break;

		if (strncmp("ILBM", chunk_id, 4) == 0)
		{
			if (opts->verbose_mode)
				fprintf(stderr, "Found ILBM chunk\n");
		}
		else if (strncmp("BMHD", chunk_id, 4) == 0)
		{
			if (opts->verbose_mode)
				fprintf(stderr, "Found BMHD chunk\n");
			chunk_size = read_dword(fp);

			image_data->width = read_word(fp);
			image_data->height = read_word(fp);
			_dump = read_word(fp);
			_dump = read_word(fp);
			image_data->bitplanes = read_byte(fp);
			if (image_data->bitplanes > 8)
				image_data->bitplanes = 8;
			_dump = read_byte(fp);
			compression = read_byte(fp);

			if (opts->verbose_mode)
			{
				fprintf(stderr, " - Image size: %ix%i\n", image_data->width, image_data->height);
				fprintf(stderr, " - Number of bitplanes: %i\n", image_data->bitplanes);
			}

			fseek(fp, chunk_size-11, SEEK_CUR);
		}
		else if (strncmp("CMAP", chunk_id, 4) == 0)
		{
			if (opts->verbose_mode)
				fprintf(stderr, "Found CMAP chunk\n");
			chunk_size = read_dword(fp);

			if (opts->verbose_mode)
				fprintf(stderr, " - Number of colors: %ld\n", chunk_size / 3);

			image_data->palette = malloc(chunk_size);
			fread((void*)image_data->palette, chunk_size, 1, fp);
		}
		else if (strncmp("BODY", chunk_id, 4) == 0)
		{
			if (opts->verbose_mode)
				fprintf(stderr, "Found BODY chunk\n");
			chunk_size = read_dword(fp);

			if (compression == 0)
			{
				if (opts->verbose_mode)
					fprintf(stderr, " - Image data not compressed\n");
				body_data = (UBYTE*)calloc(1, chunk_size);
				fread((void*)body_data, chunk_size, 1, fp);
			}
			else
			{
				if (opts->verbose_mode)
					fprintf(stderr, " - Image data compressed\n");
				UBYTE *compressedData = (UBYTE*)calloc(1, chunk_size);
				fread((void*)compressedData, chunk_size, 1, fp);
				
				int uncompressedSize = (image_data->width>>3)*image_data->height*image_data->bitplanes;
				UBYTE *uncompressedData = (UBYTE*)calloc(1, uncompressedSize);
				rel_decompress(compressedData, uncompressedData, chunk_size);

				free((void*)compressedData);

				body_data = uncompressedData;
			}
			
			image_data->data = body_data;
		}
	}
	fclose(fp);

	return S_OK;
}