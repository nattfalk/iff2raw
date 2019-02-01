#ifndef _INC_IFF_IMAGE_
#define _INC_IFF_IMAGE_

#include "iff2raw.h"

typedef struct
{
    int width;
    int height;
    UBYTE bitplanes;
    UBYTE *data;
    UBYTE *palette;
} iff_image_data;

#endif
