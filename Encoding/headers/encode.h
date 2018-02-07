# ifndef CYPHER_ENCODE_H
# define CYPHER_ENCODE_H

# include <stdio.h>
# include <stdlib.h>

# include "analysis.h"

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
  int correction_level;
};

// tool functions

size_t sPow(size_t x, int n);
size_t getLastInf(size_t x);
char* adjustSize(char* bits, int limit);

// --------------

char* convertToByte(size_t input);
struct EncData* getEncodedSize(struct options *arg);


# endif
