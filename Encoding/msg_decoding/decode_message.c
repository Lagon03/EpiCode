# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <err.h>

# include "../headers/analysis.h"
# include "../headers/decode.h"
# include "../headers/encode.h"

char* byte_decoding(char* input, size_t len, size_t c_count)
{
    printf("Input : %s\n", input);
    char* output = malloc((c_count + 1) * sizeof(char));
    printf("%li\n", c_count);
    output[c_count] = '\0';

    size_t index = 0;
    size_t cur = 0;
    while(index < len && cur < c_count)
    {
        char* bits = malloc(8 * sizeof(char));
        for(size_t c = 0; c < 8 && index < len; ++c, ++index)
            bits[c] = input[index];
        printf("%s\n", bits);
        char letter = (char)convertToDec(bits);
        free(bits);

        output[cur] = letter;
        
        cur += 1;
    }

    free(input); // The data is now of no use
    return output;
}
