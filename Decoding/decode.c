# include <stdio.h>
# include <stdlib.h>

# include "decode.h"
# include "../Encode/headers/encode.h"

char* listTakeAt(char* src, size_t start, size_t len)
{
    char* dst = malloc(len * sizeof(char));
    for(size_t i = start; i < len && src[i] != '\0'; ++i)
    {
        dst[i] = src[i];
    }
    return dst;
}

char* decode(char* input, int version, int level)
{
    char* mode = malloc(5 * sizeof(char));  // mode : 0001 / 0010 / 0100
                                            // the mode is the first 4 byte of
                                            // the input
    
}
