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
  size_t version;
  struct ErrorCorrection Correction;
};

// tool functions

size_t sPow(size_t x, int n);
size_t getLastInf(size_t x);

// --------------

char* convertToByte(size_t input);
size_t getEncodedSize(char *input);
struct EncData getVersion(char* input, Mode mode);


# endif
