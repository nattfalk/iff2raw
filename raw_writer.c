#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "iff2raw.h"
#include "raw_writer.h"
#include "iff_image.h"
#include "math.h"

UBYTE get_color(UBYTE color, int color_mode)
{
    if (color_mode == 4)
        return color >> 4;
    else
        return color;
}

void raw_writeimage(_options *opts, iff_image_data *image_data)
{
    FILE *fp;

    if (opts->verbose_mode)
        fprintf(stderr, "\n== RAW writer ==\n");

    if (!opts->output_filename)
    {
        if (opts->verbose_mode)
            fprintf(stderr, "No output filename specified!\n");
        return;
    }

    // Save RAW image
    char raw_filename[255];
    strcpy(raw_filename, opts->output_filename);
    strcat(raw_filename, ".raw");
    if (opts->verbose_mode)
        fprintf(stderr, " - RAW image filename: '%s'\n", raw_filename);

    int size = (image_data->width >> 3) * image_data->height * image_data->bitplanes;
    if (opts->row_mode == 1)
    {
        // Interleaved mode
        if (opts->verbose_mode)
            fprintf(stderr, " - Saving interleaved image\n");

        fp = fopen(raw_filename, "wb+");
        if (!fp)
        {
            fprintf(stderr, "Could not open file '%s' for writing!", raw_filename);
            return;
        }
        fwrite(image_data->data, size, 1, fp);
        fclose(fp);
    }
    else
    {
        // Non-interleaved mode
        if (opts->verbose_mode)
            fprintf(stderr, " - Saving non-interleaved image\n");

        UBYTE *new_image_data = calloc(size, 1);

        ULONG src_offset = 0, dest_offset;

        int rowsize = image_data->width >> 3;
        int pagesize = rowsize * image_data->height;
        int y, bpl;

        for (y=0; y<image_data->height; y++)
        {
            for (bpl=0; bpl<image_data->bitplanes; bpl++)
            {
                dest_offset = (bpl * pagesize) + (y * rowsize);

                memcpy(new_image_data + dest_offset, image_data->data + src_offset, rowsize);
                src_offset += rowsize;
            }
        }

        fp = fopen(raw_filename, "w+b");
        if (!fp)
        {
            fprintf(stderr, "Could not open file '%s' for writing!", raw_filename);
            return;
        }
        fwrite((void*)new_image_data, size, 1, fp);
        fclose(fp);
        
        if (new_image_data)
            free(new_image_data);
    }
    
    // Save palette
    int i;
    UBYTE r, g, b;
    char palette_filename[255];
    strcpy(palette_filename, opts->output_filename);

    if (opts->palette_mode == 0)
    {
        // Write binary palette
        strcat(palette_filename, ".pal");
        if (opts->verbose_mode)
            fprintf(stderr, " - Palette filename: %s\n", palette_filename);

        if (opts->verbose_mode)
            fprintf(stderr, " - Saving palette in binary mode\n");

        fp = fopen(palette_filename, "w+b");
        if (!fp)
        {
            fprintf(stderr, "Could not open file '%s' for writing!", palette_filename);
            return;
        }

        int j = 1 << (int)image_data->bitplanes;
        for (i=0; i<j; i++)
        {
            r = get_color(*(image_data->palette+(i*3)), opts->color_mode);
            g = get_color(*(image_data->palette+(i*3)+1), opts->color_mode);
            b = get_color(*(image_data->palette+(i*3)+2), opts->color_mode);

            if (opts->color_mode == 4)
            {
                unsigned short rgb = (r << 8) | (g << 4) | b;
                fwrite(&rgb, sizeof(unsigned short), 1, fp);
            }
            else
            {
                ULONG rgb = (r << 16) | (g << 8) | b;
                fwrite(&rgb, sizeof(ULONG), 1, fp);
            }
        }
        fclose(fp);
    }
    else
    {
        // Write palette as asm-source
        strcat(palette_filename, ".s");
        if (opts->verbose_mode)
            fprintf(stderr, " - Palette filename: %s\n", palette_filename);

        if (opts->verbose_mode)
            fprintf(stderr, " - Saving palette in source mode\n");

        fp = fopen(palette_filename, "w+");
        if (!fp)
        {
            fprintf(stderr, "Could not open file '%s' for writing!", palette_filename);
            return;
        }

        int j = 1 << (int)image_data->bitplanes;
        if (opts->color_mode == 4)
            fprintf(fp, "      ;0rgb\n");    
        else
            fprintf(fp, "      ;00rrggbb\n");    
        for (i=0; i<j; i++)
        {
            r = get_color(*(image_data->palette+(i*3)), opts->color_mode);
            g = get_color(*(image_data->palette+(i*3)+1), opts->color_mode);
            b = get_color(*(image_data->palette+(i*3)+2), opts->color_mode);

            if (opts->color_mode == 4)
                fprintf(fp, " dc.w $0%1x%1x%1x\n", r, g, b);    
            else
                fprintf(fp, " dc.l $00%02x%02x%02x\n", r, g, b);    
        }
        fclose(fp);
    }
}
