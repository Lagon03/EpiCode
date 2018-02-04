/*
**  Scan
**  file: scan.c
**  description : Qr Code Scan Finder Patterns
*/

# include <stdio.h>
# include "scan.h"
# include "Dmat.h"

# define RESET_STATE(_state_)       \
    for (int i = 0 ; i < 6 ; i++)   \
         _state_[i] = 0;

# define PRINT_STATE(_state_)       \
    for (int i = 1 ; i < 6 ; i++)   \
        warn("State %d , size %d", i, _state_[i]);

# define foreach_state(_state_)     \
    for (int i = 1 ; i <6 ; i++)
        
# define PRECISION 1

//  Functions
static inline
void draw_line(SDL_Surface *img, int x_begin, int y_begin, int x_end, int y_end, char color)
{   
    int r = 255;
    int g = 0;
    int b = 0;
    
    if( color == 'g')
    {
        r = 0;
        g = 255;
    }
    
    if( color == 'b')
    {
        r = 0;
        b = 255;
    }

    for(int y = y_begin; y <= y_end; y++)
    {
        for(int x = x_begin; x <= x_end; x++)
            putpixel(img, x, y, SDL_MapRGB(img->format, r, g, b));
    } 
}

void print_seg(SDL_Surface *img, struct Dmat *mat, char color)
{
    for(size_t i = 0; i < mat->lines; i++)
    {
        if(mat->mat[i][0] == 0 && mat->mat[i][2] == 0)
            return;
        printf("Segment Number %ld :", i + 1);
        draw_line(img, mat->mat[i][0], mat->mat[i][1], mat->mat[i][2],
mat->mat[i][3], color);
        for(size_t j = 0; j < mat->cols; j++)
        {
            printf(" %d;", mat->mat[i][j]);
        }
        printf("\n");
    }
}


static inline
int check_patternF(int *state)
{
    int totalsize = 0;
    foreach_state(state)
    {
        totalsize += state[i];
    }
    
    if (totalsize < 7)
        return 0;

    int module_size = totalsize / 7; 
    int max_var = module_size / 2;   //pixel error correction
    
    if((abs(module_size - (state[1])) < max_var) &&
       (abs(module_size - (state[2])) < max_var) &&
       (abs(module_size * 3 - (state[3])) < max_var * 3) &&
       (abs(module_size - (state[4])) < max_var) &&
       (abs(module_size - (state[5])) < max_var))
        return 1;
    else
        return 0;
}

// Main Functions

struct Dmat *horizontal_scan (SDL_Surface *img)
{
    struct Dmat *mat = init_Dmat(2 , 4);
    int *state = calloc(6, sizeof(int));
    int state_count = 0;
    int b_checkpoint = 0;
    int segcoords[4]; // ( x, y , x', y')
    

    for(int y = 0; y < img->h; y += PRECISION)
    {
        RESET_STATE(state);
        state_count = 0; 
        for(int x = 0; x < img->w; x++)
        {
            Uint32 pixel = getpixel(img, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, img->format, &r, &g, &b);
            
            if(state_count <= 5)
            {
                if (state_count == 0)
                {
                    if(r == 0)
                    {
                        segcoords[0] = x;
                        segcoords[1] = y;
                        segcoords[3] = y;
                        state_count++;
                    }
                }
                else if (state_count % 2 == 1)
                {
                    if(r == 255)
                    {
                        if (state_count == 5)
                            segcoords[2] = x;
                        state_count++; 
                    }
                    else
                    {
                        state[state_count]++;
                    }
                }
                else if (state_count % 2 == 0)
                {
                    if(r == 0)
                    {
                        if (state_count == 2)
                            b_checkpoint = x-1;
                        state_count++;
                    }
                    else
                    {
                        state[state_count]++; 
                    }

                }
            }
            else
            { 
                if( check_patternF(state) == 1)
                {
                      //PRINT_STATE(state);
                      add_Dmat(mat, segcoords);
                }

                x = b_checkpoint;
                RESET_STATE(state);
                state_count = 0;
            }
        }
    }
    free(state);
    return mat;
}

struct Dmat *vertical_scan (SDL_Surface *img)
{
    struct Dmat *mat = init_Dmat(2 , 4);
    int *state = calloc(6, sizeof(int));
    int state_count = 0;
    int b_checkpoint = 0;
    int segcoords[4]; // ( x, y , x', y')
    

    for(int x = 0; x < img->w; x += PRECISION)
    {
        RESET_STATE(state);
        state_count = 0;

        for(int y = 0; y < img->h; y++)
        {
            Uint32 pixel = getpixel(img, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, img->format, &r, &g, &b);
            
            if(state_count <= 5)
            {
                if (state_count == 0)
                {
                    if(r == 0)
                    {
                        segcoords[0] = x;
                        segcoords[1] = y;
                        segcoords[2] = x;
                        state_count++;
                    }
                }
                else if (state_count % 2 == 1)
                {
                    if(r == 255)
                    {
                        if (state_count == 5)
                            segcoords[3] = y;
                        state_count++; 
                    }
                    else
                    {
                        state[state_count]++;
                    }
                }
                else if (state_count % 2 == 0)
                {
                    if(r == 0)
                    {
                        if (state_count == 2)
                            b_checkpoint = y-1;
                        state_count++;
                    }
                    else
                    {
                        state[state_count]++; 
                    }

                }
            }
            else
            {   
                //warn("%d, %d  pixel", x, y);
                //PRINT_STATE(state);
                if( check_patternF(state) == 1)
                {   
                      //warn("%d row", x);
                      //PRINT_STATE(state);
                      add_Dmat(mat, segcoords);
                }

                y = b_checkpoint;
                RESET_STATE(state);
                state_count = 0;
            }
            //PRINT_STATE(state);
        }
    }
    free(state);
    return mat;
}
