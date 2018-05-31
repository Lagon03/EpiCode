# ifndef CYPHER_ENCODE_MESSAGE_H
# define CYPHER_ENCODE_MESSAGE_H

# include <stdio.h>
# include <stdlib.h>

# include "analysis.h"
# include "encode.h"

extern const char* alpha_;

char* num_encoding(char* data, size_t len);
char* alpha_encoding(char* data, size_t len);
char* byte_encoding(char* data, size_t len);

# endif
