#include "iff2raw.h"
#include "rle_decompress.h"

void rel_decompress(UBYTE* input, UBYTE* output, ULONG inputSize) {
	UBYTE marker, symbol;
	ULONG i, inputPosition, outputPosition, count;

	if (inputSize < 1)
	{
		return;
	}

	inputPosition = 0;
	marker = input[inputPosition++];
	outputPosition = 0;

	do
	{
		symbol = input[inputPosition++];

		if (symbol == marker)
		{
			count = input[inputPosition++];

			if (count <= 2)
			{
				for (i = 0; i <= count; ++i)
				{
					output[outputPosition++] = marker;
				}
			}
			else
			{
				if (count & 0x80)
				{
					count = ((count & 0x7f) << 8) + input[inputPosition++];
				}

				symbol = input[inputPosition++];

				for (i = 0; i <= count; ++i)
				{
					output[outputPosition++] = symbol;
				}
			}
		}
		else
		{
			output[outputPosition++] = symbol;
		}
	} while (inputPosition < inputSize);
}
