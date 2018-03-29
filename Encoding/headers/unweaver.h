# ifndef _UNWEAVER_H_
# define _UNWEAVER_H_

# include <stdio.h>
# include <stdlib.h>

# include "encode.h"

struct Codewords* unweave(char* input, int version, int level);

# endif
