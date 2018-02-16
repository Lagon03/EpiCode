/*
**  QrReader I - Search Finder patterns
**  file : fpfind.h
**  description : Header file of fpfind.c
*/

# ifndef _FPFIND_H_
# define _FPFIND_H_

# include <stdio.h>
# include "Dmat.h"
# include "Dvector.h"
# include "../sdl/mysdl.h"

struct FPat
{
    struct Dmat *centers; //coords of center of FP
    struct Dvector *ems_vector; //estimated module size
};

struct FPat *findFP(SDL_Surface *img);

# endif
