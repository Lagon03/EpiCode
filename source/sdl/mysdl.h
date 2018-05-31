/*
**  MYSDL
**  file: mysdl.h
**  description: header of mysdl
*/

#ifndef MYSDL_H_
#define MYSDL_H_

#include<stdlib.h>
#include<err.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>

void wait_for_keypressed(void);
void init_sdl(void);
SDL_Surface* load_image(char *path);
void display_image(SDL_Surface *img);
SDL_Surface* create_image(int height, int width);
void save_image(SDL_Surface *img, const char* file);
Uint32 getpixel(SDL_Surface *surface, unsigned x, unsigned y);
void putpixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);

#endif
