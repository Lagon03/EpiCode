# include <stdio.h>
# include <stdlib.h>

# include "../headers/weaver.h"


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

    struct Weave* weave = malloc(sizeof(struct Weave));
    size_t* forest = malloc(w_count * sizeof(size_t));

    size_t nb_cw_01 = GROUP_CODEWORDS[msg_d->correction_level][0][msg_d->version];
    size_t nb_cw_02 = GROUP_CODEWORDS[msg_d->correction_level][1][msg_d->version];
    size_t nb_block01 = GROUP_BLOCK_CODEWORDS[0][msg_d->correction_level][msg_d->version];

    size_t cur = 0; // Position in the forest (interweaved codewords)
    if(GROUP_CODEWORDS[msg_d->correction_level][1][msg_d->version] != 0) {
        size_t nb_block02 = GROUP_BLOCK_CODEWORDS[1][msg_d->correction_level][msg_d->version];
        for(size_t c1 = 0, c2 = 0; (c1 < nb_cw_01) | (c2 < nb_cw_02);) { // c1 : number of the codewords in group / block 1 | c2 : number of the codewords in group / block 2
            for(size_t b1 = 0; b1 < nb_block01 && c1 < nb_cw_01; ++b1, ++cur) {
                forest[cur] = convertToDec(msg_d->codewords->group[0]->blocks[b1]->words[c1]);
            }
            for(size_t b2 = 0; b2 < nb_block02 && c2 < nb_cw_02; ++b2, ++cur) {
                forest[cur] = convertToDec(msg_d->codewords->group[1]->blocks[b2]->words[c2]);
                printf("\t%ld\n", forest[cur]);
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
                forest[cur] = convertToDec(msg_d->codewords->group[0]->blocks[b1]->words[c1]);
            if(c1 < nb_cw_01)
                ++c1;
        }
    }
    
    // Add correction codewords weave

    weave->forest = forest;
    weave->size = w_count;

    return weave;
}

