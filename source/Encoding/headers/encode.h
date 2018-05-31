# ifndef CYPHER_ENCODE_H
# define CYPHER_ENCODE_H

# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>

# include "analysis.h"



extern const size_t TOTAL_DECC[4][41];
extern const size_t GROUP_CODEWORDS[4][2][41];
extern const size_t GROUP_BLOCK_CODEWORDS[2][4][41];
extern const int8_t ECC_CODEWORDS_PER_BLOCK[4][41];
extern const int8_t NUM_ERROR_CORRECTIOn_BLOCKS[4][41];
extern const size_t L_LEVEL[3][41];
extern const size_t M_LEVEL[3][41];
extern const size_t Q_LEVEL[3][41];
extern const size_t H_LEVEL[3][41];

extern const char SpecAdd[2][8];

extern const size_t Remainder_bits[41];
extern const char* S_bits[4][8];
extern const char* V_bits[34];


/* Maximum number of characters a 40-L code can contain
 * Numeric : 7096 characters
 * Alpha   : 4296 characters
 * Byte    : 2953 characters
 */

// Numeric mode indicator       : 0001
// Alphanumeric mode indicator  : 0010
// Byte mode indicator          : 0100

struct ErrorCorrection
{
  char level;
  size_t capacity;
};

struct EncData
{
  char* mode_ind;
  char* character_count_ind;
  char* encoded_data;
  size_t version;
  size_t epi_v;
  int correction_level;
  struct Codewords* codewords;
};

struct Codewords
{
    // group then block then word then character
    // words[group][block][word][char]
    //char**** words;
    struct Group** group;
    size_t size;
    size_t nb_block;
    size_t words;
};

struct Group
{
    struct Block** blocks;
    int id;
    size_t size;
};

struct Block
{
    char** words;
    char** correction;
    size_t size;
    int id;
};

// tool functions

size_t sPow(size_t x, int n);
size_t getLastInf(size_t x);
char* adjustSize(char* bits, int limit);
char* adjustSizeInv(char* bits, int limit);

// --------------
void freeCodeWords(struct Codewords* codewords);
char* convertToByte(size_t input);
size_t convertToDec(char* input);
struct EncData* getEncodedSize(struct options *arg);


# endif
