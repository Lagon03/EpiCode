/*
**  Basic_Segmentation Header
**  file : scan.h
**  description : Header file for scan.c
*/

# ifndef _SCAN_H_
# define _SCAN_H_

# include <stdio.h>
# include "Dmat.h"
# include "../sdl/mysdl.h" // include <err.h>

void print_seg (SDL_Surface *img, struct Dmat *mat);
struct Dmat *horizontal_scan (SDL_Surface *img);
struct Dmat *vertical_scan (SDL_Surface *img);

# endif
