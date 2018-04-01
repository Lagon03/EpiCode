# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <err.h>

# include "../headers/analysis.h"
# include "../headers/decode.h"
# include "../headers/encode.h"
# include "../headers/encode_message.h"

char* num_decoding(char* input, size_t len, size_t c_count)
{
    //size_t nbpair = 1;
    //if (c_count > 3)
    //    nbpair = c_count % 3 == 0 ? c_count / 3 : c_count / 3 + 1;
    
    char* output = malloc((c_count + 1) * sizeof(char));
    output[c_count] = '\0';
    
    size_t index = 0;
    size_t cur = 0;
    while(index < len && cur < c_count)
    {
        char* bits = malloc(11 * sizeof(char));
        bits[0] = '\0';
        
            for(size_t c = 0; c < 10 && index < len; ++c, ++index)
            {
                bits[c] = input[index];
                bits[c + 1] = '\0';
            }
        size_t i_letter = convertToDec(bits);
        printf("i_letter : %li\n", i_letter);
        size_t order = 3;
        if(i_letter < 10)
            order = 1;
        else if(i_letter < 100)
            order = 2;
        char* group = malloc((order + 1) * sizeof(char));
        snprintf(group, (order + 1) * sizeof(char), "%zu", i_letter);
        group[order] = '\0';
        printf("%s\n", input);
        for(size_t i = 0; i < order; ++i, ++cur)
        {
            output[cur] = group[i];
        }
        
        free(group);
        free(bits);
    }

    return output;
}

char* alpha_decoding(char* input, size_t len, size_t c_count)
{
    char* output = malloc((c_count + 1) * sizeof(char));
    output[c_count] = '\0';

    size_t l_pair = c_count / 2;

    size_t index = 0;
    size_t pair = 0;
    size_t cur = 0;
    while(index < len && cur < c_count)
    {
        ++pair;
        char* bits;
        if(pair <= l_pair)
        {
            bits = malloc(12 * sizeof(char));
            bits[11] = '\0';
            for(size_t c = 0; c < 11 && index < len; ++index, ++c)
                bits[c] = input[index];
            size_t res = convertToDec(bits);    
            output[cur] = alpha_[(res / 45)];
            output[cur + 1] = alpha_[(res % 45)];
            cur += 2;
        }
        else
        {
            bits = malloc(7 * sizeof(char));
            bits[6] = '\0';
            for(size_t c = 0; c < 6 && index < len; ++index, ++c)
                bits[c] = input[index];
            size_t res = convertToDec(bits);

            output[cur] = alpha_[res];
            ++cur;
        }
        free(bits);
    }

    return output;
}

char* byte_decoding(char* input, size_t len, size_t c_count)
{
    char* output = malloc((c_count + 1) * sizeof(char));
    output[c_count] = '\0';

    size_t index = 0;
    size_t cur = 0;
    while(index < len && cur < c_count)
    {
        char* bits = malloc(9 * sizeof(char));
        bits[8] = '\0';
        for(size_t c = 0; c < 8 && index < len; ++c, ++index)
            bits[c] = input[index];
        char letter = (char)convertToDec(bits);
        free(bits);

        output[cur] = letter;

        ++cur;
    }
    free(input); // The data is now of no use
    return output;
}
