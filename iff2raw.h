#ifndef _INC_IFF2RAW_
#define _INC_IFF2RAW_

#include <stdbool.h>

#define S_OK	-1
#define S_ERR	0

typedef unsigned char UBYTE;
typedef unsigned short USHORT;
typedef unsigned long ULONG;

typedef struct {
    char *input_filename;
    char *output_filename;

    int color_mode;
    int palette_mode;
    int row_mode;

    bool verbose_mode;
} _options;

#endif
