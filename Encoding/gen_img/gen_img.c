/*
**  QrCode Image Generator 
**  file : gen_img.c
**  description : Generates an image in BMP format with the QrCode printed
**  inside.
*/

# include "../header/gen_img.h"

static inline
void white_map(SDL_Surface *img)
{
    for(int y = 0; y < img->h; y++)
    {
        for(int x = 0; x < img->w; x++)
        {
            putpixel(img, x, y, SDL_MapRGB(img->format, 255, 255, 255);
        }
    }
}

static inline
void fill_black(SDL_Surface *img, int begin_x, int begin_y, int mod_size)
{
    for(int y = begin_y; y < begin_y + mod_size; y++)
    {
        for(int x = begin_x; x < begin_x + mod_size; x++)
        {
            putpixel(img, x, y, SDL_MapRGB(img->format, 0, 0, 0);
        }
    }
}

void Generate_QrCode(char **mat, int version, const char* name, int mod_size)
{
    sdl_init();
    
    int size = mod_size * (version * 4 + 17 + 8); 
    int mat_size = version * 4 + 17;
    
    SDL_Surface *img = create_img(size);
    white_map(img);
    
    for(int y = mod_size * 4; y < size - 4 * mod_size; y += mod_size)
    {
        for(int x = mod_size * 4; j < size - 4 mod_size; x += mod_size)
        {
            if(mat[y/mod_size][x/mod_size] == '1')
            {
                fill_black(img, x, y, mod_size);
            }
        }
    }
    
    display_image(img);
    save_image(img, name);
    SDL_Quit();
}
