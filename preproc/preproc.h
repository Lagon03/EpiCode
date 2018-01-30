/*
 *  PREPROC
 *  file: preproc.h
 *  description: header of preprocessing
 */

#ifndef PREPROC_H_
#define PREPROC_H_

#include"../mysdl/mysdl.h"

SDL_Surface *grayscale(SDL_Surface* im);
SDL_Surface *blackAndWhite(SDL_Surface* img, int split);
SDL_Surface *otzu(SDL_Surface* img,int w_min,int w_max,int h_min,int h_max);
SDL_Surface *contrast(SDL_Surface* img);
#endif
