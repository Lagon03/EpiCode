# include <stdlib.h>
# include <stdio.h>

# include "../headers/unweaver.h"
# include "../headers/encode.h"

struct Codewords* unweave(char* input, int version, int level)
{
    size_t w_count = TOTAL_DECC[level][version];
        // Total word count
    size_t ecc_count = ECC_CODEWORDS_PER_BLOCK[level][version];
        // Total codewords count

    size_t nb_cw_01 = GROUP_CODEWORDS[level][0][version];
        // Codewords count in the first group
    size_t nb_cw_02 = GROUP_CODEWORDS[level][1][version];
        // Codewords count in the second group
    size_t nb_block_01 = GROUP_BLOCK_CODEWORDS[0][level][version];
        // Number of block in the first group
    size_t nb_block_02 = GROUP_BLOCK_CODEWORDS[1][level][version];
        // Number of block in the second group
   
    /*printf("Codewords in first group : %li\n", nb_cw_01);
    printf("Codewords in second group : %li\n", nb_cw_02);
    printf("Blocks in first group : %li\n", nb_block_01);
    printf("Blocks in second group : %li\n", nb_block_02);*/

    /*
     * During the encoding process we weaved/leaved the codewords together
     * block by block which means we know have to deweave/deleave it.
     *
     * Theoretically we just have to do the same as we did for encoding
     * But instead of taking the codewords to create a "forest" we take the
     * "forest" to make group of block of codewords
     *
     * ------------------------------------------------------------------------
     *
     * For the first part of the input we only have the data codewords,
     * the second part however is composed of the error correction codewords.
     * The distinction between the those part means that they'll be two phases:
     *  - Recuperation of the codewords / reorganisation into groups and blocks
     *  - Recuperation of the error codewords / reorganisation into groups 
     *                                                              and blocks 
     */

    //=========================================================================
    //  Phase Zero : Data Codewords Structure Initialization
    //=========================================================================
    
    // Initialization of the Data Codewords struct (from encode.h)
    struct Codewords* DCR = malloc(sizeof(struct Codewords));
    if(nb_block_01 != 0 && nb_block_02 == 0)
    {
        DCR->group = malloc(sizeof(struct Group*));
        DCR->size = 1;
    }
    else
    {
        DCR->group = malloc(2 * sizeof(struct Group*));
        DCR->size = 2;
    }
    DCR->words = w_count;
    DCR->nb_block = nb_block_01 + nb_block_02;

    for(size_t g = 0; g < DCR->size; ++g)
    {
        size_t block_nb = (g == 0 ? nb_block_01 : nb_block_02);
        if(block_nb != 0)
        {
            // group init
            DCR->group[g] = malloc(sizeof(struct Group));
            DCR->group[g]->blocks = malloc(block_nb * sizeof(struct Block*));
            DCR->group[g]->id = g;
            DCR->group[g]->size = block_nb;
           
            
            struct Group* p_group = DCR->group[g];
            // end group init
            
            for(size_t b = 0; b < block_nb; ++b)
            {
                // block init
                size_t data_cw = (g == 0 ? nb_cw_01 : nb_cw_02);
                p_group->blocks[b] = malloc(sizeof(struct Block));
                struct Block* p_block = p_group->blocks[b];
                p_block->words = malloc(data_cw * sizeof(char*));
                p_block->id = g;
                p_block->size = data_cw;
                                
                // word init
                for(size_t w = 0; w < data_cw; ++w) {
                    p_block->words[w] = malloc(9 * sizeof(char));
                    p_block->words[w][8] = '\0';
                }

                // correction word init
                p_block->correction = malloc(ecc_count * sizeof(char*));
                for(size_t i = 0; i < ecc_count; ++i)
                {
                    p_block->correction[i] = malloc(9 * sizeof(char));
                    p_block->correction[i][8] = '\0';
                }
                DCR->group[g]->blocks[b] = p_block;
            }
            DCR->group[g] = p_group;
        }
    }
    // End of initilization for Data Codewords structure (for both phase)

    //=========================================================================
    //  Phase One : Data Codewords Recuperation (DCR)
    //=========================================================================
       
    size_t index = 0; // Position in the data stream
    
    if(nb_cw_02 != 0)
    {
        for(size_t c1 = 0, c2 = 0; (c1 < nb_cw_01) | (c2 < nb_cw_02);)
        {
            for(size_t b1 = 0; b1 < nb_block_01 && c1 < nb_cw_01; ++b1) 
            {
                char* p_word = DCR->group[0]->blocks[b1]->words[c1];
                for(size_t i = 0; i < 8; ++i, ++index)
                    p_word[i] = input[index];
            }
            for(size_t b2 = 0; b2 < nb_block_02 && c2 < nb_cw_02; ++b2) {
                char* p_word = DCR->group[1]->blocks[b2]->words[c2];
                for(size_t i = 0; i < 8; ++i, ++index)
                    p_word[i] = input[index];
            }
            if(c1 < nb_cw_01)
                ++c1;
            if(c2 < nb_cw_02)
                ++c2;
        }
    }
    else
    {
        for(size_t c1 = 0; (c1 < nb_cw_01);)
        {
            for(size_t b1 = 0; b1 < nb_block_01 && c1 < nb_cw_01; ++b1) 
            {
                char* p_word = DCR->group[0]->blocks[b1]->words[c1];
                for(size_t i = 0; i < 8; ++i, ++index)
                    p_word[i] = input[index];
            }
            if(c1 < nb_cw_01)
                ++c1;
        }
    }

    //=========================================================================
    //  End Phase One
    //=========================================================================

    //=========================================================================
    //  Start Phase Two : Correction Codewords Recupration (CCR)
    //=========================================================================

    size_t nb_cc = nb_cw_01 > ecc_count ? nb_cw_01 : ecc_count;
    if(nb_cw_02 != 0)
    {
        for(size_t c1 = 0, c2 = 0; (c1 < nb_cc) | (c2 < nb_cc);)
        {
            for(size_t b1 = 0; b1 < nb_block_01 && c1 < nb_cc; ++b1) 
            {
                char* p_cc = DCR->group[0]->blocks[b1]->correction[c1];
                for(size_t i = 0; i < 8; ++i, ++index)
                    p_cc[i] = input[index];
            }
            for(size_t b2 = 0; b2 < nb_block_02 && c2 < nb_cc; ++b2) {
                char* p_cc = DCR->group[1]->blocks[b2]->correction[c2];
                for(size_t i = 0; i < 8; ++i, ++index)
                    p_cc[i] = input[index];
            }
            if(c1 < nb_cc)
                ++c1;
            if(c2 < nb_cc)
                ++c2;
        }
    }
    else
    {
        nb_cc = ecc_count;
        for(size_t c1 = 0; (c1 < nb_cc); ++c1)
        {
            for(size_t b1 = 0; b1 < nb_block_01; ++b1) 
            {
                printf("%li && %li\n", c1, b1);
                char* p_cc = DCR->group[0]->blocks[b1]->correction[c1];
                for(size_t i = 0; i < 8; ++i, ++index)
                    p_cc[i] = input[index];
            }
        }
    }
    //=========================================================================
    //  End Phase Two
    //=========================================================================
    
    // Now that we retrieved the data codewords and the correction codewords,
    // there might be some remainder_bits but in normal circumstance we will
    // ignore them.
    
    return DCR;
}
