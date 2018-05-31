# include <stdio.h>
# include <stdlib.h>
# include <err.h>
# include <string.h>
# include <time.h>

# include "headers/analysis.h"
# include "headers/encode.h"
# include "headers/mask.h"
# include "headers/weaver.h"
# include "headers/fill_mat.h"
# include "headers/gen_img.h"
# include "headers/draw.h"
# include "headers/polynomials.h"
# include "headers/decode.h"

# include "headers/freect.h"

struct options* checkArg(int argc, char* argv[])
{
    struct options *arg = malloc(sizeof(struct options));
    arg->message = NULL;
    arg->correction = -1;
    arg->mode = -1;
    arg->version = 99;
    for(int i = 1; i < argc; ++i)
    {
        if(strcmp(argv[i], "-d") == 0) { // arg for the data
            ++i;
            if (i >= argc)
                err(1, "!!!--------> Error missing arguments.\n");
            arg->message = argv[i];
        }
        else if(strcmp(argv[i], "-v") == 0) { // arg for the data
            ++i;
            if (i >= argc)
                err(1, "!!!--------> Error missing arguments.\n");
            arg->version = strtol(argv[i], NULL, 10);
        }
        else if(strcmp(argv[i], "-c") == 0) {
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

char* GenName(void)
{
    char* name = (char*)malloc(sizeof(char)*25);
    time_t rawtime;

    time (&rawtime);
    struct tm  *timeinfo = localtime (&rawtime);
    strftime(name, sizeof(name)-1, "%d.%m", timeinfo);

    name[5] = '\0';
    //printf("%s\n", name);
    name[0] = 't';
    name[1] = 'e';
    name[2] = 's';
    name[3] = 't';
    return name;
}

int enc_main (int argc, char* argv[])
{
    struct options *arg = checkArg(argc, argv);
    if(arg->message == NULL)
    {
        arg->message = "HELLO WORLD";
        arg->correction = -1;
        arg->mode = -1;
        arg->version = 99;
    }
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
    /*printf("\tMode indicator             : %s\n", data->mode_ind);
      printf("\tCharacters count indicator : %s\n", data->character_count_ind);*/
    //printf("\tEncoded message            : %s\n", data->encoded_data);
    printf("------ Options  ------\n");
    printf("\tVersion                    : %li\n", data->version);
    printf("\tCorrection                 : %i\n", data->correction_level);
    printf("------ Raw bits ------\n");
    /*printf("\tRaw encoded bits           : %s%s%s\n", data->mode_ind,
      data->character_count_ind, data->encoded_data);
      size_t size = 4 + getSize(data->character_count_ind) 
      + getSize(data->encoded_data);
      printf("\tRaw encoded bits length    : %li\n", size);*/

    // Generating the QrCode Matrix
    struct QrCode_Enc* QrCode = initQrCode(data);
    struct Weave* weave = interweave(QrCode);

    char* weave_trans = malloc((((weave->size * 8) + Remainder_bits[data->version] + 1)
                * sizeof(char)));
    weave_trans[0] = '\0';
    for(size_t w = 0; w < weave->size; ++w) {
        char* word = convertToByte(weave->forest[w]);
        word = adjustSize(word, 8);
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
    printf("Interweaved data : \n");
      for(size_t i = 0; i < weave->size; ++i)
      printf("%ld ", weave->forest[i]);
      printf("\nLength : %li\n", weave->size);
      printf("Interweaved data to binary : \n%s\n", weave_trans);
      printf("Total length : %li | with %li remainder bits\n", (weave->size * 8)
      + Remainder_bits[data->version], Remainder_bits[data->version]);

    fill_mat(QrCode->mat, QrCode->size, data->version, weave_trans, (weave->size
                * 8) + Remainder_bits[data->version]);


    setFormatString(QrCode, S_bits[data->correction_level][6]);
    if(data->version >= 7)
        setVersionString(QrCode, V_bits[data->version - 7]);
    protectMatrix(QrCode);

    //We now evaluate
    int** mask_point = evaluate(QrCode, data->version, data->correction_level);
    size_t cur = -1;
    int min = 99999999;
    for(size_t i = 0; i < 8; ++i) {
        /*printf("Mask %li result : %i\n", i, mask_point[i][4]);
          printf("\t Penalty 1 : %i\n", mask_point[i][0]);
          printf("\t Penalty 2 : %i\n", mask_point[i][1]);
          printf("\t Penalty 3 : %i\n", mask_point[i][2]);
          printf("\t Penalty 4 : %i\n", mask_point[i][3]);*/
        if(mask_point[i][4] < min) {
            cur = i;
            min = mask_point[i][4];
        }
    }
    printf("Mask selected is %li\n", cur);    

    /*for(int i = 0; i < 8; ++i) {
      applyMask(QrCode->mat, QrCode->size, i);

      unprotectMatrix_B(QrCode);
      setFormatString(QrCode, S_bits[data->correction_level][i]);
      if(data->version >= 7)
      setVersionString(QrCode, V_bits[data->version - 7]);
      Generate_QrCode(QrCode->mat, data->version, "test.bmp", 8);
      printf("Applying mask %i\n", i);
      protectMatrix(QrCode);
      applyMask(QrCode->mat, QrCode->size, i);

      }*/
    printf("Applying the best mask >> mask %li\n", cur);
    applyMask(QrCode->mat, QrCode->size, cur);
    unprotectMatrix_B(QrCode);

    setFormatString(QrCode, S_bits[data->correction_level][cur]);
    if(data->version >= 7)
        setVersionString(QrCode, V_bits[data->version - 7]);

    char* name = GenName();
    name = "test.bmp";
    if(data->version >= 15)
        Generate_QrCode(QrCode->mat, data->version, name, 4);
    else
        Generate_QrCode(QrCode->mat, data->version, name, 8);
    /*char* test = "0100000001000101010001100101011100110111010000001110110000010001111011000101000010101101111011100100011110011010101101110000001111010101111100000001110101000001101111111110110001001000010110111010011010010111\0";
    // "Test" should be the output
    char* test2 = "01000000110001001000011001010110110001101100011011110010110000100000010101110110111101110010011011000110010000001110110000010001100111000000001010010110011100011010001100110110010010100101000100011010100101110111110010100100001010000010111100010001011100000110010000111011100101011110000101001010110100001111100110000000101101000000111100111001001100110000000\0";
    // "Hello, World" should be the output
    char *test3 = "001000001101110111000011010100110000101101010100011110001010101011010001001010110111001000000000110111001110110001001101000100010100010011101100110110100001000111000101111011000110111000010001001000011110110011110100110101000100011000110000000001100101111110001010001011010101101110100011011011100001011110111000010011111100100100101100011000111101110101010110001101000100000110111011111110111010101001000000110110001101111110111101111111101110001000101001101100101111001010100100100100100110110010101011101101001011011001111111011100010011001100000111111101100000000\0";
    // "HELLO WORLD HOW ARE THOU" should be the output
    char* string = decode(test, 1, 3);
    printf("Decoded string : %s\n", string);
    free(string);
    string = decode(test2, 2, 3);
    printf("Decoded string : %s\n", string);
    free(string);
    string = decode(test3, 3, 3);
    printf("Decoded string : %s\n", string);
    free(string);
    string = decode(weave_trans, data->version, data->correction_level);
    printf("Decoded string : %s\n", string);
    free(string);   */ 

    /*for(size_t x = 0; x < QrCode->size; ++x) {
      free(QrCode->mat[x]);
      }
      free(QrCode->mat);
      free(QrCode);

      free(weave->forest);
      free(weave);

      freeCodeWords(data->codewords);
    // need a custom made free function
    free(data->character_count_ind);
    free(data->encoded_data);
    free(data);
    free(arg);*/
    // ---

    Completefree(data, arg, QrCode, weave);

    free(mod);
    free(weave_trans);
    return 1;
}
