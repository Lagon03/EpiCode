/*
**  MYSDL
**  file: mysdl.h
**  description: header of mysdl
*/

#ifndef MYSDL_H_
#define MYSDL_H_

#include<stdlib.h>
#include<err.h>
#include<SDL/SDL.h>
#include<SDL/SDL_image.h>

void wait_for_keypressed(void);
void init_sdl(void);
SDL_Surface* load_image(char *path);
SDL_Surface* display_image(SDL_Surface *img);
Uint32 getpixel(SDL_Surface *surface, unsigned x, unsigned y);
void putpixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);

#endif
