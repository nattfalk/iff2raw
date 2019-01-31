#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <getopt.h>

#include "iff2raw.h"
#include "byte_order.h"
#include "iff_loader.h"

char *input_filename;
char *output_filename;
int color_mode = 4;
int palette_mode = 0;

void print_usage()
{
    fprintf(stderr, "Usage: iff2raw [flags]\n\n");
    fprintf(stderr, "Flags:\n");
    fprintf(stderr, "  -i <filename>    Input filename\n");
    fprintf(stderr, "  -o <filename>    Output filename without extension\n");
    fprintf(stderr, "  -c <mode>        Color mode. 4=4 bits per channel (default), 8=8 bits per channel\n");
    fprintf(stderr, "  -p <mode>        Palette output mode. 0=binary (default), 1=asm-source\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "  -h               Print this message and exit\n");
    fprintf(stderr, "\n");
}

static bool collect_arguments(int argc, char **argv) 
{
    int ch;

    if (argc < 2)
    {
        fprintf(stderr, "** Wrong number of arguments!\n\n");
        return false;
    }

    while ((ch = getopt(argc, argv, "c:h:i:o:p:")) != -1) 
    {
        switch (ch) {
            case 'i':
                input_filename = strdup(optarg);
                break;

            case 'o':
                output_filename = strdup(optarg);
                break;

            case 'c':
                color_mode = atoi(optarg);
                break;

            case 'p':
                palette_mode = atoi(optarg);
                break;

            case 'h':
            default:
                return false;
        }
    }

    return true;
}

int main(int argc, char** argv)
{
    fprintf(stderr, "iff2raw 0.1 Copyright (c) Michael Nattfalk 2019\n\n");

    if(!collect_arguments(argc, argv)) {
        print_usage();
        exit(1);
    }




    return 0;
}