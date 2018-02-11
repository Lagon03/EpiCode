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

struct Dmat *findFP(SDL_Surface *img);

# endif
