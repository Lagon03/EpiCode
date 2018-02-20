# ifndef _DRAW_H_
# define _DRAW_H_

# include <>
# include <>

# include ""
# include ""

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

struct QrCode_enc initMat(struct EncData* data);

# endif
