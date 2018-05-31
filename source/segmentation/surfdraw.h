/*
**  Header Drawing functions
**  file : surfdraw.h
**  description : header file
*/

# ifndef _SURFDRAW_H_
# define _SURFDRAW_H_

#include <math.h>
# include "../sdl/mysdl.h"
# include "Dmat.h"
# include "Dvector.h"

void drawFP(SDL_Surface *img, struct Dmat *centers, struct Dvector *ems_vector, int A);
//void drawBox(SDL_Surface *img, double *coords);
void Draw_line(SDL_Surface *img, int x0, int y0, int x1, int y1);
void Draw_point(SDL_Surface *img, int x, int y);

# endif
