/*
**  Header file for QrCode Extraction 
**  file : extcode.h
**  description : indexes function...
*/

# ifndef _EXTCODE_H_
# define _EXTCODE_H_

# include <math.h>
# include "fpfind.h"
# include "Dvector.h"
# include "Dmat.h"
# include <stdlib.h>
# include <err.h>

struct Finder
{
    double *center;    //center coords
    double *ul;        //top left
    double *dr;        //bot right
};
 
struct QrCode
{
    struct Finder *A;   // top left
    struct Finder *B;   // top right
    struct Finder *C;   // bot left
    double *coords;
    int  version;
    double m_size;
    char **mat;
};

static inline
struct Finder *init_Finder(double x_center, double y_center, int m_size)
{
    double *center = malloc(sizeof(double)*2);
    center[0] = x_center;
    center[1] = y_center;
    
    //warn("%lf,%lf", x_center, y_center);
     
    double *ul = malloc(sizeof(double)*2);
    double *dr = malloc(sizeof(double)*2);
    ul[0] = x_center - (double)m_size * 3.5;
    ul[1] = y_center - (double)m_size * 3.5;
    dr[0] = x_center + (double)m_size * 3.5;
    dr[1] = y_center + (double)m_size * 3.5;
    
   // warn("coords tp: %lf, %lf coords %lf %lf", ul[0], ul[1], dr[0], dr[1]);
     
    struct Finder *nov = malloc(sizeof(struct Finder));
    nov->center = center;
    nov->ul = ul;
    nov->dr = dr;
    return nov;
}

struct QrCode *extract_QrCode_NoG(SDL_Surface *img, struct FPat *finders);

# endif
