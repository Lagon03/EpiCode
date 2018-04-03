# include <stdlib.h>
# include <stdio.h>
# include <err.h>

# include "headers/analysis.h"
# include "headers/encode.h"
# include "headers/weaver.h"

void freeCodewords(struct Codewords* codewords)
{
    for(size_t g = 0; g < codewords->size; ++g) 
    {
        for(size_t b = 0; b < codewords->group[g]->size; ++b ) 
        {
            for(size_t i = 0; i < codewords->group[g]->blocks[b]->size; ++i) 
            {
                free(codewords->group[g]->blocks[b]->words[i]);
            }
            free(codewords->group[g]->blocks[b]->words);
            free(codewords->group[g]->blocks[b]->correction);
            free(codewords->group[g]->blocks[b]);
        }
        free(codewords->group[g]->blocks);
        free(codewords->group[g]);
    }
    free(codewords->group);
    free(codewords);
}

void freeED(struct EncData* data)
{
    // We free the character count indicator
    free(data->character_count_ind);
    // We free the encoded binary
    free(data->encoded_data);
    // We then free the codewords
    freeCodewords(data->codewords);
    // Then we finally free the EncData ptr
    free(data);
}

void freeQrCode(struct QrCode_Enc* QrCode)
{
    for(size_t x = 0; x < QrCode->size; ++x)
        free(QrCode->mat[x]);
    free(QrCode->mat);
    free(QrCode);
}

void freeWeave(struct Weave* weave)
{
    free(weave->forest);
    free(weave);
}

void Completefree(struct EncData* data, struct options* args, 
        struct QrCode_Enc* QrCode, struct Weave* weave)
{
    freeED(data);
    freeWeave(weave);
    freeQrCode(QrCode);

    free(args);
}


