# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <err.h>

# include "../headers/decode.h"
# include "../headers/encode.h"
# include "../headers/unweaver.h"
# include "../headers/decode_message.h"

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
    size_t val = convertToDec(mode);
    if (val == 1)
        return 0;
    else if (val == 2)
        return 1;
    else
        return 2;
}

char* decode(char* input, int version, int level)
{
    /*
     * From the input we shall now determine the correction codewords
     * and the message codewords which have been interweaved
     * To unweave we will need to retrieve the number of :
     *      - group     (1 or 2)
     *      - blocks
     *      - codewords in each block
     *      - correction codewords per block
     */

    struct Codewords* DCR = unweave(input, version, level);
    /*for(size_t g = 0; g < DCR->size; ++g)
    {
        for(size_t b = 0; b < DCR->group[g]->size; ++b)
        {
            for(size_t w = 0; w < DCR->group[g]->blocks[b]->size; ++w)
                printf("Code word  %2li : %s\n", w + 1, DCR->group[g]->blocks[b]->words[w]);
            for(size_t w = 0; w < (size_t)ECC_CODEWORDS_PER_BLOCK[level][version]; ++w)
                printf("CCode word %2li : %s\n", w + 1, DCR->group[g]->blocks[b]->correction[w]);
        }
    }*/

    /*
     * After the determining codewords and all we need to check them using
     * reed-solomon to ensure that the data isn't corrupted. 
     * If it is, we will either try to repare it using the correction 
     * codewords either we will abandon the process.
     */
   
    //-------------------------------------------------------------------------
    //  TODO : Add function to check health of the data using RS
    //-------------------------------------------------------------------------


    size_t l_data = TOTAL_DECC[level][version]; // length of the data codewords
    
    char* data = malloc(((l_data  * 8) + 1) * sizeof(char));
    data[0] = '\0';

    for(size_t g = 0; g < DCR->size; ++g)
    {
        for(size_t b = 0; b < DCR->group[g]->size; ++b)
        {
            for(size_t w = 0; w < DCR->group[g]->blocks[b]->size; ++w)
            {
                strncat(data, DCR->group[g]->blocks[b]->words[w], 8);            }
        }
    }
    freeCodeWords(DCR);

    char* c_mode;                           // mode : 0001 / 0010 / 0100
                                            // the mode is the first 4 byte of
                                            // the input
    char* c_count;                          // characters count
                                            // its length is computed after
                                            // defining the mode
    c_mode = listTakeAt(data, 0, 4);
    c_mode = realloc(c_mode, 5 * sizeof(char));
    c_mode[4] = '\0';

    int i_mode = modeToInt(c_mode);                 // mode converted to int
                                                    // 0/1/2
    int char_count = getCLength(version, i_mode);   // number of characters
                                                    // for the char count
                                                    // in the encoded message

    c_count = listTakeAt(data, 4, char_count);
    c_count = realloc(c_count, (char_count + 1) * sizeof(char));
    c_count[char_count] = '\0';

    int i_count = convertToDec(c_count);

    /*
     * At this point we got the mode as a string and a int :
     *      Example : c_mode = "0001\0" and i_mode = 0
     * We should also have the characters count with X bits :
     *      Example : c_count = "00001000" and char_count = 8 
     *              | byte mode version <= 9vim 
     */


    /*
     * At this point we are sure that the data is not corrupted so all we 
     * need to do is to make a new string which will contain the data to
     * convert from binary to char.
     */
  
    // Now we need to take only the relevant data
    size_t lim = getLength(data) - 4 - char_count;
    char* tmp = listTakeAt(data, 4 + char_count, lim);
    tmp = realloc(tmp, (lim + 1) * sizeof(char));
    tmp[lim] = '\0';
    //-------------------------------------------

    free(data);
    data = tmp;
    char* output;
    // Now we just need to decode it !
    switch(i_mode)
    {
        case 1:
            output = alpha_decoding(data, getLength(data), i_count);
            break;
        case 2:
            output = byte_decoding(data, getLength(data), i_count);
            break;
        default:
            output = num_decoding(data, getLength(data), i_count);
            break;
    }
    free(c_count);
    free(c_mode);

    return output;
}
