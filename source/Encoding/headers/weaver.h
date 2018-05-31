# ifndef _WEAVER_H_
# define _WEAVER_H_

# include <stdio.h>
# include <stdlib.h>

# include "analysis.h"
# include "encode.h"
# include "mask.h"

struct Weave
{
    size_t* forest;
    size_t size;
};

struct Weave* interweave(struct QrCode_Enc* data);

# endif
