/*
 *  MYSDL
 *  Contain the given sdl function for the project
 */

#include"mysdl.h"

// Waiting for a key function
void wait_for_keypressed(void) {
  SDL_Event event;
  // Infinite loop, waiting for event
  for(;;) {
    // Take an event
    SDL_PollEvent( &event );
    // Switch on event type
    switch (event.type) {
      // Someon e pressed a key -> leave the function
      case SDL_KEYDOWN: return;
      default: break;
    }
    // Loop until we got the expected event
  }
}

// Init SDL function
void init_sdl(void) {
  // Init only the video part
  if( SDL_Init(SDL_INIT_VIDEO) == -1 )
    errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
  // We don't really need a function for that ...
}

// Loading image from a file
SDL_Surface* load_image(char *path) {
  SDL_Surface *img;
  // Load an image using SDL_image with format detection
  img = IMG_Load(path);
  if (!img)
    errx(3, "can't load %s: %s", path, IMG_GetError());
  return img;
}


void display_image(SDL_Surface *img)
{
    int SCREEN_WIDTH = img->w;
    int SCREEN_HEIGHT = img->h;

    SDL_Window *window = SDL_CreateWindow("Display image", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                                                           SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    
    if(window == NULL)
        errx(1, "CreateWindow error: %s\n", SDL_GetError());
   
    SDL_Surface *screen = SDL_GetWindowSurface(window);
    
    if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
        warnx("BlitSurface error %s\n", SDL_GetError());

    SDL_UpdateWindowSurface(window);
    
    wait_for_keypressed();
    //getchar();

    SDL_DestroyWindow(window);
    
    return;
}

// Create an image
SDL_Surface* create_image(int height, int width)
{
    SDL_Surface* img = SDL_CreateRGBSurface(0, height, width, 32, 0, 0, 0, 0);
    if(img == NULL)
        errx(1,"Could not create an RGB SDL_Surface: %s, \n", SDL_GetError());
    return img;
}

void save_image(SDL_Surface *img, const char* file)
{
    if(SDL_SaveBMP(img, file) < 0)
        errx(1,"Could not save SDL_Surface to BMP: %s, \n", SDL_GetError());
}

static inline Uint8* pixelref(SDL_Surface *surf, unsigned x, unsigned y) {
  int bpp = surf->format->BytesPerPixel;
  return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}

Uint32 getpixel(SDL_Surface *surface, unsigned x, unsigned y) {
  Uint8 *p = pixelref(surface, x, y);
  switch(surface->format->BytesPerPixel) {
    case 1:
      return *p;
    case 2:
      return *(Uint16 *)p;
    case 3:
      if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
        return p[0] << 16 | p[1] << 8 | p[2];
      else
        return p[0] | p[1] << 8 | p[2] << 16;
    case 4:
      return *(Uint32 *)p;
  }
  return 0;
}

void putpixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel) {
  Uint8 *p = pixelref(surface, x, y);
  switch(surface->format->BytesPerPixel) {
    case 1:
      *p = pixel;
      break;
    case 2:
      *(Uint16 *)p = pixel;
      break;
    case 3:
      if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
        p[0] = (pixel >> 16) & 0xff;
        p[1] = (pixel >> 8) & 0xff;
        p[2] = pixel & 0xff;
      } else {
        p[0] = pixel & 0xff;
        p[1] = (pixel >> 8) & 0xff;
        p[2] = (pixel >> 16) & 0xff;
      }
      break;
    case 4:
      *(Uint32 *)p = pixel;
      break;
  }
}
