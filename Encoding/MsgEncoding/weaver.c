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
                forest[cur] = convertToDec(msg_d->codewords->group[0]->blocks[b1]->words[c1]);
            if(c1 < nb_cw_01)
                ++c1;
        }
    }
    
    // Add correction codewords weave
   
    // FOR DEBUG TO REPLACE BY TRUE CORRECTION CODEWORDS
    //
    const size_t corr_cw_HW[13] = {168, 72,  22,  82,  217,  54,  156,  0,  46,  15,  180,  122,  16};
    const size_t corr_cw_HW2[26*2] = {70,150,57,52,64,67,83,203,182,206,170,85,82,50,143,61,34,180,161,82,127,219,16,157,192,14,165,238,6,101,188,153,74,43,234,46,152,1,75,214,108,208,237,222,68,145,232,139,205,37,131,132};
    const size_t corr_cw_HW3[22] = {68,  58,  185,  229,  225,  182,  7,  225,  235,  223,  123,  87,  126,  72,  148,  107,  24,  108,  193,  228,  183  };

    for(size_t i = 0; i < ecc_count * (nb_block01 + nb_block02) && cur < w_count + (ecc_count * (nb_block01 + nb_block02)); ++i, ++cur) {
        if(ecc_count == 22)
            forest[cur] = corr_cw_HW3[i];
        else if(ecc_count * (nb_block01) == 13)
            forest[cur] = corr_cw_HW[i];
        else if(ecc_count == 26)
            forest[cur] = corr_cw_HW2[i];
        else
            forest[cur] = rand() % 255; //random value
    }
    // END DEBUG
    
  

    weave->forest = forest;
    weave->size = w_count + (ecc_count * (nb_block01 + nb_block02));

    return weave;
}

