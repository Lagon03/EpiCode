/*
**  Qrlocater
**  file: 
**  description : Locates Qr Code Finder Patterns
*/

# include "Dmat.h"

# define RESET_STATE(_state_)       \
    for (int i = 0 ; i < 5 ; i++)   \
         _state_[i] = 0;

# define PRINT_STATE(_state_)       \
    for (int i = 1 ; i < 5 ; i++)   \
        warn("State %d , size %d", i, _state_[i]);

# define foreach_state(_state_)     \
    for (int i = 1 ; i < 5 ; i++)
        
# define PRECISION 1

//  Functions

static inline
int check_ratio(int *state)
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
        return totalsize;
    else
        return 0;
}

int get_BW(SDL_Surface *img, x, y) //returns 0 if black, 1 if white
{
    Uint32 pixel = getpixel(img, x, y);
    Uint8 r, g, b;
    SDL_GetRGB(pixel, img->format, &r, &g, &b);
    
    if(r == 0)
        return 0;
    else
        return 1;
}

double get_center(int totalsize, int x)
{
    double half_segment = (double)totalsize / 2.0;
    double center = (double)x - half_segment;
}

double check_ver(SDL_Surface *img, int start_y, int center_x, int state_2, int totalsize)
{
    int h = img->h;
    int *state_check = calloc(5, sizeof(int));
    int y = start_y;
    
    //upwards check from center
    
    while((y >= 0) && (get_BW(img, x, y) == 0))
    {
        state_check[2]++ ;
        y--;
    }
    if(y < 0)
        return 0;
 
    while((y >= 0) && (get_BW(img, x, y) == 1) && (state_check[1] < state_2))
    {
        state_check[1]++ ;
        y--;
    }
    if(y < 0 || state_check[1] >= state_2)
        return 0;

    while((y >= 0) && (get_BW(img, x, y) == 0) && (state_check[0] < state_2))
    {
        state_check[0]++ ;
        y--;
    }
    if(y < 0 || state_check[0] >= state_2)
        return 0;
    
    //downwards check from center
    
    y = start_y + 1; 
    
    while((y <= h) && (get_BW(img, x, y) == 0))
    {
        state_check[2]++ ;
        y++;
    }
    if(y > h)
        return 0;
 
    while((y <= h) && (get_BW(img, x, y) == 1) && (state_check[3] < state_2))
    {
        state_check[3]++ ;
        y++;
    }
    if(y > h || state_check[3] >= state_2)
        return 0;

    while((y <= h) && (get_BW(img, x, y) == 0) && (state_check[4] < state_2))
    {
        state_check[4]++ ;
        y++;
    }
    if(y > h || state_check[4] >= state_2)
        return 0;
    
    int check_totalsize = 0;
    foreach_state (state_check)
    {
        check_totalsize = state_check[i];
    }
    
    if(5 * abs(check_totalsize - totalsize) >= 2 * totalsize)
        return 0;
    
    double center = get_center(check_totalsize, y);
    if(check_ratio(state_check) != 0)
        return center;
    return 0;    
}

int handle_centers(SDL_Surface *img, struct Dmat *centers, int *state, int size,
                   int y, int x, int estimated_module_size)
{
    //Cross Vertical Check
    double center_x = get_center(totalsize, x);
    double center_y = check_ver(img, y, (int)center_x, state[2], totalsize);
    if (center_y == 0)
        return 0;

    //Cross Horizontal Check
    center_x = check_ver(img, center_y, center_x, state[2], totalsize);
    if(center_x == 0)
        return 0;

    int validation = check_diag(img, center_y, center_x, state[2], totalsize);
    if(validation == 0)
        return 0;
    
    // Add center if not found in centers
    new_center = calloc(2, sizeof(int));
    new_center[0] = center_x;
    new_center[1] = center_y;
    double new_estimated_module_size = (double)totalsize / 7.0;
    
    foreach_line(centers)
    {
        double dist_x = mat[i][0] - new_center[0];
        double dist_y = mat[i][1] - new_center[1];
        
        if(dist_x < 10 && dist_y < 10)
        {
            mat[i][0] = mat[i][0] + new_center[0] / 2;
            mat[i][1] = mat[i][1] + new_center[1] / 2;
            
            estimated_module_size
        } 
    }
}

// Main Functions

struct Dmat *findFP (SDL_Surface *img)
{
    struct Dmat *centers = init_Dmat(2 , 3);
    int *state = calloc(6, sizeof(int));
    int state_count = 0;

    for(int y = 0; y < img->h; y += PRECISION)
    {
        RESET_STATE(state);
        state_count = 0;
 
        for(int x = 0; x < img->w; x++)
        {
            //Uint32 pixel = getpixel(img, x, y);
            //Uint8 r, g, b;
            //SDL_GetRGB(pixel, img->format, &r, &g, &b);
            
            if(get_BW(img, x, y) == 0)
            {
                if(state_count % 2 == 1);
                    state_count++ ;
                state[state_count]++ ;
            }
            else
            {
                if(state_count % 2 == 1)
                    state[state_count]++ ;
                else
                {
                    if(state_count == 4)
                    {
                        int totalsize = 0;
                        if((totalsize = check_ratio(state)) == 1)
                        {
                            int confirmed = handle_centers(img, state, totalsize, y, x);
                        }
                        else
                        {
                            state_count = 3;
                            state[0] = state[2];
                            state[1] = state[3];
                            state[2] = state[4];
                            state[3] = 1;
                            state[4] = 0;
                            continue;
                        }
                        else
                        {
                            state_count++ ;
                            state[state_count]++ ;
                        }
                    }
                }
            }
        }
    }
    free(state);
    return //NOT DONE
}


