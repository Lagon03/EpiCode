/*
**  Header File FPValid.h
*/

# ifndef _FP_VALID_H_
# define _FP_VALID_H_

# include <math.h>
# include "fpfind.h"

struct FPresults
{
    double dist;
    int indexA;
    int x1, x2, x3;
    int y1, y2, y3;
};

struct FPresults *QrCode_found(struct FPat *fp);
double *get_Dcoord (struct FPat *fp, int A); //Only if using projection transformation

# endif
