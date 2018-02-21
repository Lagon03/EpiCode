# include "mysdl.h"

int main()
{
    const char* file = "imgtest.bmp";
    init_sdl();
    SDL_Surface* img = create_image(200);
    display_image(img);
    save_image(img, file);
    return 1; 
}
