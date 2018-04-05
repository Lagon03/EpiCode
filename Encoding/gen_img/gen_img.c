/*
**  QrCode Image Generator 
**  file : gen_img.c
**  description : Generates an image in BMP format with the QrCode printed
**  inside.
*/

# include "../headers/gen_img.h"
# include "../headers/encode.h"
# include <string.h>

static inline
void white_map(SDL_Surface *img)
{
    for(int y = 0; y < img->h; y++)
    {
        for(int x = 0; x < img->w; x++)
        {
            putpixel(img, x, y, SDL_MapRGB(img->format, 255, 255, 255));
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
            putpixel(img, x, y, SDL_MapRGB(img->format, 0, 0, 0));
        }
    }
}

void Generate_QrCode(char **mat, int version, const char* name, int mod_size)
{
    init_sdl();
    
    int size = mod_size * (version * 4 + 17 + 8); 
    int mat_size = version * 4 + 17;
    
    //warn("1");
    
    SDL_Surface *img = create_image(size, size);
    white_map(img);
    
    for(int y = mod_size * 4; y / mod_size - 4 < mat_size; y += mod_size)
    {
        for(int x = mod_size * 4; x / mod_size - 4 < mat_size; x += mod_size)
        {
            //warn("2");
            //warn("y %d, x %d", y, x);
            //warn("y/mod_size = %d ||x/modsize = %d", y/mod_size, x/mod_size);
            if(mat[y/mod_size - 4][x/mod_size - 4] == '1')
            {
                fill_black(img, x, y, mod_size);
            }
        }
    }
    
    //warn("3");
    //display_image(img);
    // output path : "../output/.." -> 10 characters
    char * output = malloc((strlen(name) + 10) * sizeof(char));
    output[0] = '\0';
    strcat(output, "./output/");
    strcat(output, name);

    save_image(img, output);
    SDL_Quit();
}
