/*
**  Header Drawing functions
**  file : surfdraw.h
**  description : header file
*/

# ifndef _SURFDRAW_H_
# define _SURFDRAW_H_

# include "../sdl/mysdl.h"
# include "Dmat.h"
# include "Dvector.h"

void drawFP(SDL_Surface *img, struct Dmat *centers, struct Dvector *ems_vector);
void drawBox(SDL_Surface *img, double *coords);

# endif
