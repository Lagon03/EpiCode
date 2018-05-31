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

int display;

struct options* checkArg(int argc, char* argv[])
{
    struct options *arg = malloc(sizeof(struct options));
    display = 1;
    arg->message = NULL;
    arg->correction = -1;
    arg->mode = -1;
    arg->version = 99;
    arg->epi = 0;
    arg->output = "\0";
    arg->name = NULL;
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
        else if(strcmp(argv[i], "-x") == 0) { // arg for the display
            display = 0;
        }
        else if(strcmp(argv[i], "-t") == 0) { // arg for the epicode
            arg->epi = 1;
        }
        else if(strcmp(argv[i], "-o") == 0) { // arg for the output
            ++i;
            if (i >= argc)
                err(1, "!!!--------> Error missing arguments.\n");
            arg->output = argv[i];
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
        else if(strcmp(argv[i], "-n") == 0) {
            i++;
            if (i >= argc)
                err(1, "|||--------> Error missing argument.\n");
        }
    }
    return arg;
}

char* GenName(void)
{
    time_t rawtime;

    char* name = (char*)malloc(sizeof(char)*17);

    time (&rawtime);
    struct tm  *timeinfo = localtime (&rawtime);
    strftime(name, 16, "%d.%m - %H:%M", timeinfo);

    name[16] = '\0';
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
    /*printf("------ Raw bits ------\n");
    printf("\tRaw encoded bits           : %s%s%s\n", data->mode_ind,
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
    /*printf("Interweaved data : \n");
      for(size_t i = 0; i < weave->size; ++i)
      printf("%ld ", weave->forest[i]);
      printf("\nLength : %li\n", weave->size);
      printf("Interweaved data to binary : \n%s\n", weave_trans);
      printf("Total length : %li | with %li remainder bits\n", (weave->size * 8)
      + Remainder_bits[data->version], Remainder_bits[data->version]);*/
    
    
    char* epic = malloc(sizeof(char));
    if(arg->epi == 1) {
        size_t epic_size = ((weave->size * 8) + Remainder_bits[data->version]) / 2;
        free(epic);
        char* epic = malloc((epic_size * 2) * sizeof(char));
        //epic[epic_size * 2] = '\0';
        for(size_t es = 0, ess = 0; es < epic_size - 2 ; es += 2, ++ess) {
            // now we check the value of the epic char
            // 00 -> white | 01 -> red
            // 10 -> blue  | 11 -> green
            printf("%c%c -> ", weave_trans[es], weave_trans[es + 1]);
            if(weave_trans[es] == weave_trans[es + 1]) {
                if (weave_trans[es] == '0')
                    epic[ess] = 'w';
                else if (weave_trans[es] == '1')
                    epic[ess] = 'g';
            }
            else {
                if (weave_trans[es] == '0')
                    epic[ess] = 'r';
                else if (weave_trans[es] == '1')
                    epic[ess] = 'b';
            }
            printf("%c\n", epic[ess]);
        }
        //printf("test\n");
        for(size_t i = epic_size / 2, k = 0; 
                i < (weave->size * 8) + Remainder_bits[data->version]; ++i, k += 2) {
            if(weave_trans[k] == weave_trans[k + 1]) {
                if (weave_trans[k] == '0')
                    epic[i] = 'w';
                else if (weave_trans[k] == '1')
                    epic[i] = 'g';
            }
            else {
                if (weave_trans[k] == '0')
                    epic[i] = 'b';
                else if (weave_trans[k] == '1')
                    epic[i] = 'r';
            }
            if(k + 1 >= epic_size)
                k = 0;
        }
        //printf("epic : %s\n", epic);
        fill_mat(QrCode->mat, QrCode->size, data->version, epic, weave->size * 8 + Remainder_bits[data->version]);
    }
    else {
        fill_mat(QrCode->mat, QrCode->size, data->version, weave_trans, (weave->size
                    * 8) + Remainder_bits[data->version]);
    }

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
    if(data->version >= 15)
        Generate_QrCode(QrCode->mat, data->version, name, 4, arg->output, display);
    else
        Generate_QrCode(QrCode->mat, data->version, name, 8, arg->output, display);

    Completefree(data, arg, QrCode, weave);

    free(mod);
    free(weave_trans);
    free(epic);
    return 1;
}
