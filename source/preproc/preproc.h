/*
 *  PREPROC
 *  file: preproc.h
 *  description: header of preprocessing
 */

#ifndef PREPROC_H_
#define PREPROC_H_

#include"../sdl/mysdl.h"

SDL_Surface *grayscale(SDL_Surface* img);
void binarize(SDL_Surface* img, int threshold);
int otsu(SDL_Surface* img);
SDL_Surface *contrast(SDL_Surface* img);
#endif
