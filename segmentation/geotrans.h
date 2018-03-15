/*
** geotrans.h
*/

# ifndef _GEO_TRANS_H_
# define _GEO_TRANS_H_

# include <stdlib.h>
# include <stdio.h>
# include <math.h>

double *SolveAffineEquations(double x1, double y1, double x2, double y2, double x3, double y3, size_t size);

# endif
