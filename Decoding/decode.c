# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <err.h>

# include "decode.h"
# include "../Encoding/headers/encode.h"

int getLength(char* input)
{
    int i = 0;
    while(input[i] != '\0')
        ++i;
    return i;
}

char* listTakeAt(char* src, size_t start, size_t len)
{
    char* dst = malloc(len * sizeof(char));
    for(size_t i = start; (i - start) < len && src[i] != '\0'; ++i)
        dst[i - start] = src[i];
    return dst;
}

static int getCLength(int version, int mode)
{
    int length = 8;
    if(version <= 9) {
        if(mode == 0)
            length = 10;    // Num
        else if(mode == 1)
            length = 9;     // Alpha
        else
            length = 8;     // Byte
    }
    else if(version <= 26) {
        if(mode == 0)
            length = 12;
        else if(mode == 1)
            length = 11;
        else
            length = 16;
    }
    else {
        if(mode == 0)
            length = 14;
        else if(mode == 1)
            length = 13;
        else
            length = 16;
    }
    return length;
}

static int modeToInt(char* mode)
{        
    if (strcmp("0001", mode) == 0)
        return 0;
    else if (strcmp("0010", mode) == 0)
        return 1;
    else
        return 2;
}

char* decode(char* input, int version, int level)
{
    char* c_mode;                           // mode : 0001 / 0010 / 0100
                                            // the mode is the first 4 byte of
                                            // the input
    char* c_count;                          // characters count
                                            // its length is computed after
                                            // defining the mode

    c_mode = listTakeAt(input, 0, 4);

    int i_mode = modeToInt(c_mode);                 // mode converted to int
    // 0/1/2
    int char_count = getCLength(version, i_mode);   // number of characters
    // for the char count
    // in the encoded message
    c_count = listTakeAt(input, 4, char_count);

    /*
     * At this point we got the mode as a string and a int :
     *      Example : c_mode = "0001\0" and i_mode = 0
     * We should also have the characters count with X bits :
     *      Example : c_count = "00001000" and char_count = 8 
     *              | byte mode version <= 9vim 
     */

    int m_length = getLength(input) - char_count - 4;   // total length of the
                                                        // untouched data
    /*
     * From this data we shall now determine the correction codewords
     * and the message codewords which have been interweaved
     * To unweave we will need to retrieve the number of :
     *      - group     (1 or 2)
     *      - blocks
     *      - codewords in each block
     *      - correction codewords per block
     */

    return c_mode;
}
