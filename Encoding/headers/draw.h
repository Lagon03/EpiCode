# ifndef _DRAW_H_
# define _DRAW_H_

# include <stdlib.h>
# include <stdio.h>

# include "mask.h"
# include "encode.h"

// For the drawing use the following :
//  f : finder pattern
//  s : separator
//  a : alignment pattern
//  d : dark module
//  c : correction info (mask/correction)
//  t : timing
//  v : version info

struct QrCode_enc
{
    char** mat;
    struct EncData* data;
};

void protectMatrix(struct QrCode_Enc* data);
void unprotectMatrix(struct QrCode_Enc* data);

void fillMatrix(struct QrCode_Enc* data, char** input);

# endif
