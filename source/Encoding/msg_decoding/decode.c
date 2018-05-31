# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <err.h>

# include "../headers/decode.h"
# include "../headers/encode.h"
# include "../headers/unweaver.h"
# include "../headers/decode_message.h"
# include "../headers/decode_RS.h"
# include "../headers/array.h"
# include "../headers/op.h"

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

char* convertEpi(char* input, size_t max_size) {
    //printf("Max size : %li\n", max_size);
    //printf("%s\n", input);
    char* buffer = malloc((max_size +1) * sizeof(char));
    buffer[max_size] = '\0';
    for(size_t es = 0, k = 0; (input[es] != '\0') && k < max_size; ++es, k += 2) {
        if(input[es] == 'w') {
            buffer[k] = '0';
            buffer[k + 1] = '0';
        }
        if(input[es] == 'r') {
            buffer[k] = '0';
            buffer[k + 1] = '1';
        }
        if(input[es] == 'g') {
            buffer[k] = '1';
            buffer[k + 1] = '1';
        }
        if(input[es] == 'b') {
            buffer[k] = '1';
            buffer[k + 1] = '0';
        }
        //printf("k = %li\n", k);
    }
    //printf("%s\n", buffer);
    return buffer;
}

char* decode(char* input, int version, int level)
{
    //printf("Input : %s\n", input);
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
    //size_t c_data = ECC_CODEWORDS_PER_BLOCK[level][version];

    char* data = malloc(((l_data  * 8) + 1) * sizeof(char));
    data[0] = '\0';

    // Group, one by one
    for(size_t g = 0; g < DCR->size; ++g)
    {
        // Block, one by one
        for(size_t b = 0; b < DCR->group[g]->size; ++b)
        {
            /*size_t nb_b = DCR->group[g]->blocks[b]->size;
            char* buffer = malloc((((nb_b + c_data) * 8) + 1) * sizeof(char));
            buffer[0] = '\0';

            // Word, one by one
            for(size_t w = 0; w < DCR->group[g]->blocks[b]->size; ++w)
            {           
                strncat(buffer, DCR->group[g]->blocks[b]->words[w], 8);
                strncat(data, DCR->group[g]->blocks[b]->words[w], 8);
            }
            for(size_t w = 0; w < c_data; ++w)
            {
                strncat(buffer, DCR->group[g]->blocks[b]->correction[w], 8);            
            }
            int* check = malloc((nb_b + c_data) * sizeof(int));
            for(size_t i = 0; i < nb_b + c_data; ++i) {
                char* tmp = listTakeAt(buffer, i * 8, i + 8);
                tmp = realloc(tmp, 9 * sizeof(char));
                tmp[8] = '\0';
                check[i] = convertToDec(tmp);
                free(tmp);
                //printf("%i ", check[i]);
            }
            //printf("\n");
            
            struct Array *msg = malloc(sizeof(struct Array));
            size_t l = nb_b + c_data;
            initArray(msg, l);
            for(size_t i = 0; i < l; ++i)
            {
                msg->array[i] = check[i];
                insertArray(msg);
            }

            printf("Msg in: [");
            for (size_t i = 0; i < msg->used - 1; ++i) 
            {
                printf("%u,", msg->array[i]);
            }
            printf("%u]\n", msg->array[msg->used-1]);

            msg->array[5] = 0;
            struct gf_tables *gf_table = malloc(sizeof(struct gf_tables));
            gf_table->gf_exp = malloc(sizeof(struct Array));
            gf_table->gf_log = malloc(sizeof(struct Array));
            initArray(gf_table->gf_exp, 512);
            initArray(gf_table->gf_log, 256);
            gf_table = init_tables();

            struct Array *err_loc;
            struct Array *synd;
            struct Array *pos;
            //struct Array *rev_pos;

            size_t nsym = ECC_CODEWORDS_PER_BLOCK[level][version];
            printf("nsym org = %li\n", nsym);
            synd = rs_calc_syndromes(msg, nsym, gf_table);
            err_loc = rs_find_error_locator(synd, nsym, 0, gf_table);
            pos = rs_find_errors(reverse_arr(err_loc), msg->used, gf_table);
            //rev_pos = reverse_arr(pos);

            printf("err_loc used = %li\n", err_loc->used);
            if(err_loc->used - 1 != 0)
                msg = rs_correct_errdata(msg, synd, pos, gf_table);
            printf("Msg out: [");
            for (size_t i = 0; i < msg->used - 1; ++i) 
            {   
                printf("%u,", msg->array[i]);
            }
            printf("%u]\n", msg->array[msg->used-1]);

            // TODO : add strncat the correct data to the data array
            free(buffer);
            free(check);*/

            // Word, one by one
            for(size_t w = 0; w < DCR->group[g]->blocks[b]->size; ++w)
            {           
                strncat(data, DCR->group[g]->blocks[b]->words[w], 8);
            }
        }
    }
    freeCodeWords(DCR);
    if((data[0] == 'w') | (data[0] == 'b') | (data[0] == 'r') | (data[0] == 'g'))
        data = convertEpi(data, (l_data * 8));
    
    //printf("Data retrieve from segmentation : %s\n", data);

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

    //printf("output : %s\n", output);
    return output;
}
