#include <stdio.h>

#include "iff2raw.h"
#include "rle_decompress.h"

void rel_decompress(UBYTE* input, UBYTE* output, ULONG inputSize) {
	UBYTE value;
	ULONG i, inputPosition, outputPosition, count;

	if (inputSize < 1)
	{
		return;
	}

	inputPosition = 0;
	outputPosition = 0;

	do
	{
		count = input[inputPosition++];
		if (count == 0x80)
			break;

		if (count & 0x80) 
		{
			value = input[inputPosition++];
			count = 257 - count;
			for(i = 0; i < count; i++)
			{
				output[outputPosition++] = value;
			}
		}
		else
		{
			for(i = 0; i < count+1; i++)
			{
				output[outputPosition++] = input[inputPosition++];
			}
		}
	} while (inputPosition < inputSize);
}
