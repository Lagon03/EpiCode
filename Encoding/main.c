# include <stdio.h>
# include <stdlib.h>
# include <err.h>
# include <string.h>

# include "headers/analysis.h"
# include "headers/encode.h"
# include "headers/mask.h"
# include "headers/weaver.h"
# include "headers/fill_mat.h"

struct options* checkArg(int argc, char* argv[])
{
    struct options *arg = malloc(sizeof(struct options));
    arg->message = NULL;
    arg->correction = -1;
    arg->mode = -1;
    for(int i = 1; i < argc; ++i)
    {
        if(strcmp(argv[i], "-d") == 0) { // arg for the data
            ++i;
            if (i >= argc)
                err(1, "!!!--------> Error missing arguments.\n");
            arg->message = argv[i];
        }
        if(strcmp(argv[i], "-c") == 0){
            ++i;
            if (i >= argc)
                err(1, "!!!--------> Error missing arguments.\n");
            switch((int)argv[i][0]) {
                case 'M': // Medium
                    {
                        arg->correction = 1;
                        break;
                    }
                case 'Q': // Quartile
                    {
                        arg->correction = 2;
                        break;
                    }
                case 'H': // High
                    {
                        arg->correction = 3;
                        break;
                    }
                default: // Low
                    {
                        arg->correction = 0;
                        break;
                    }
            }
        }
    }
    return arg;
}

int main (int argc, char* argv[])
{
    struct options *arg = checkArg(argc, argv);
    if(arg->message == NULL)
        arg->message = "HELLO WORLD";
    int* mod;
    Mode mode;

    mod = selectMode(arg->message);
    printf("Input         : %s\n", arg->message);
    mode = checkmod(mod);
    arg->mode = (int)mode;

    printf("Detected mode : ");
    switch(mode) 
    {  
        case Numeric:
            {
                printf("Numerical mode\n");  
                break;
            }
        case Alphanumeric:  
            {
                printf("Alphanumerical mode\n");  
                break;
            }
        default :  
            {
                printf("Byte mode\n");
                break;
            }
    } 
    struct EncData *data = getEncodedSize(arg);
    if(arg->correction == -1)
        data->correction_level = 0;

    printf("\nEncoded data informations  :\n");
    printf("\tMode indicator             : %s\n", data->mode_ind);
    printf("\tCharacters count indicator : %s\n", data->character_count_ind);
    printf("\tEncoded message            : %s\n", data->encoded_data);
    printf("------ Options  ------\n");
    printf("\tVersion                    : %li\n", data->version);
    printf("\tCorrection                 : %i\n", data->correction_level);
    printf("------ Raw bits ------\n");
    printf("\tRaw encoded bits           : %s%s%s\n", data->mode_ind,
            data->character_count_ind, data->encoded_data);
    size_t size = 4 + getSize(data->character_count_ind) 
        + getSize(data->encoded_data);
    printf("\tRaw encoded bits length    : %li\n", size);
    // Generating the QrCode Matrix

    //data->version = 8;
    //printf("!--------------Modified version to %li--------------!\n", data->version);
    struct QrCode_Enc* QrCode = initQrCode(data);
    struct Weave* weave = interweave(QrCode);

    char* weave_trans = malloc((((weave->size * 8) + Remainder_bits[data->version])
                * sizeof(char)));
    weave_trans[0] = '\0';
    for(size_t w = 0; w < weave->size; ++w) {
        char* word = convertToByte(weave->forest[w]);
        strcat(weave_trans, word);
        free(word);
    }
    if(Remainder_bits[data->version] > 0) {
        char* r_bits = malloc((Remainder_bits[data->version] + 1) * sizeof(char));
        r_bits[Remainder_bits[data->version]] = '\0';
        for(size_t r = 0; r < Remainder_bits[data->version]; ++r)
            r_bits[r] = '0';
        strcat(weave_trans, r_bits);
        free(r_bits);
    }
    //weave->size = weave->size + Remainder_bits[data->version];
   
    for(size_t i = 0; i < (weave->size * 8) + Remainder_bits[data->version]; ++i)
        printf("%d\n", weave_trans[i]);

    fill_mat(QrCode->mat, QrCode->size, data->version, weave_trans, (weave->size
            * 8) + Remainder_bits[data->version]);

    for(size_t x = 0; x < QrCode->size; ++x) {
        printf("[");
        for(size_t y = 0; y < QrCode->size; ++y) {
            if(QrCode->mat[x][y] == 0)
                printf("  ");
            else
                printf("%c ", QrCode->mat[x][y]);
        }
        free(QrCode->mat[x]);
        printf("]\n");
    }
    free(QrCode->mat);
    free(QrCode);
    
    printf("Interweaved data : \n");
    for(size_t i = 0; i < weave->size; ++i)
        printf("%ld ", weave->forest[i]);
    printf("\nLength : %li\n", weave->size);
    printf("Interweaved data to binary : \n%s\n", weave_trans);
    printf("Total length : %li | with %li remainder bits\n", (weave->size * 8)
            + Remainder_bits[data->version], Remainder_bits[data->version]);
    free(weave->forest);
    free(weave);

    freeCodeWords(data->codewords);
    // need a custom made free function
    free(data->character_count_ind);
    free(data->encoded_data);
    free(data);
    // ---
    free(mod);
    free(arg);
    free(weave_trans);

    return 1;
}
