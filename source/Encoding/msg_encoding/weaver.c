# include <stdio.h>
# include <stdlib.h>

# include "../headers/weaver.h"
# include "../headers/encode.h"

// The blocks are interleaved by doing the following:
//
//    take the first data codeword from the first block
//    followed by the first data codeword from the second block
//    followed by the first data codeword from the third block
//    followed by the first data codeword from the fourth block
//    followed by the second data codeword from the first block
//    and so on
//
struct Weave* interweave(struct QrCode_Enc* data) {
    struct EncData* msg_d = data->data;
    size_t w_count = TOTAL_DECC[msg_d->correction_level][msg_d->version];
    size_t ecc_count = ECC_CODEWORDS_PER_BLOCK[msg_d->correction_level][msg_d->version];

    struct Weave* weave = malloc(sizeof(struct Weave));

    size_t nb_cw_01 = GROUP_CODEWORDS[msg_d->correction_level][0][msg_d->version];
    size_t nb_cw_02 = GROUP_CODEWORDS[msg_d->correction_level][1][msg_d->version];
    size_t nb_block01 = GROUP_BLOCK_CODEWORDS[0][msg_d->correction_level][msg_d->version];
    size_t nb_block02 = GROUP_BLOCK_CODEWORDS[1][msg_d->correction_level][msg_d->version];

    size_t* forest = malloc((w_count + (ecc_count * (nb_block01 + nb_block02))) * sizeof(size_t));


    /*printf("Correction level : %i\n", msg_d->correction_level);
    printf("Version : %li\n", msg_d->version);
    printf("Number of codeword in block in g1 : %li\n", nb_block01);
    printf("Number of codeword in block 1 : %li\n", nb_cw_01);
    printf("Number of codeword in block in g2 : %li\n", nb_block02);
    printf("Number of codeword in block 2 : %li\n", nb_cw_02);
    printf("Number of correction codeword : %li\n", ecc_count);*/


    size_t cur = 0; // Position in the forest (interweaved codewords)
    if(GROUP_CODEWORDS[msg_d->correction_level][1][msg_d->version] != 0) {
        for(size_t c1 = 0, c2 = 0; (c1 < nb_cw_01) | (c2 < nb_cw_02);) { // c1 : number of the codewords in group / block 1 | c2 : number of the codewords in group / block 2
            for(size_t b1 = 0; b1 < nb_block01 && c1 < nb_cw_01; ++b1, ++cur) {
                forest[cur] = convertToDec(msg_d->codewords->group[0]->blocks[b1]->words[c1]);
            }
            for(size_t b2 = 0; b2 < nb_block02 && c2 < nb_cw_02; ++b2, ++cur) {
                forest[cur] = convertToDec(msg_d->codewords->group[1]->blocks[b2]->words[c2]);
            }
            if(c1 < nb_cw_01)
                ++c1;
            if(c2 < nb_cw_02)
                ++c2;
        }
    }
    else {
        for(size_t c1 = 0; c1 < nb_cw_01;) { // c1 : number of the codewords in group / block 1 | c2 : number of the codewords in group / block 2
            for(size_t b1 = 0; b1 < nb_block01; ++b1, ++cur) 
            {
                char* word = msg_d->codewords->group[0]->blocks[b1]->words[c1];
                /*printf("%li\n", c1);
                printf("Word : %s = %li\n", word, convertToDec(word));*/
                forest[cur] = convertToDec(word);
            }
            if(c1 < nb_cw_01)
                ++c1;
        }
    }

    size_t nb_err_cw = nb_cw_01 > ecc_count ? nb_cw_01 : ecc_count;
    if(GROUP_CODEWORDS[msg_d->correction_level][1][msg_d->version] != 0) {
        for(size_t c1 = 0, c2 = 0; (c1 < nb_err_cw) | (c2 < nb_err_cw);) { // c1 : number of the codewords in group / block 1 | c2 : number of the codewords in group / block 2
            for(size_t b1 = 0; b1 < nb_block01 && c1 < ecc_count; ++b1, ++cur) 
            {
                char* word = msg_d->codewords->group[0]->blocks[b1]->correction[c1];
                /*printf("%li\n", c1);
                printf("Word : %s = %li\n", word, convertToDec(word));*/
                forest[cur] = convertToDec(word);
            }
            for(size_t b2 = 0; b2 < nb_block02 && c2 < ecc_count; ++b2, ++cur) 
            {
                char* word = msg_d->codewords->group[1]->blocks[b2]->correction[c2];
                /*printf("%li\n", c2);
                printf("Word : %s = %li\n", word, convertToDec(word));*/
                forest[cur] = convertToDec(word);
            }
            if(c1 < nb_err_cw)
                ++c1;
            if(c2 < nb_err_cw)
                ++c2;
        }
    }
    else {
        for(size_t c1 = 0; c1 < nb_err_cw;) { // c1 : number of the codewords in group / block 1 | c2 : number of the codewords in group / block 2
            for(size_t b1 = 0; b1 < nb_block01 && c1 < ecc_count; ++b1, ++cur) 
            {
                char* word = msg_d->codewords->group[0]->blocks[b1]->correction[c1];
                /*printf("%li\n", c1);
                printf("Word : %s = %li\n", word, convertToDec(word));*/
                forest[cur] = convertToDec(word);
            }
            if(c1 < nb_err_cw)
                ++c1;
        }
    }

    weave->forest = forest;
    weave->size = w_count + (ecc_count * (nb_block01 + nb_block02));

    /*for(size_t i = 0; i < weave->size; ++i)
        printf("%li ", forest[i]);
    printf("\n");*/

    return weave;
}

