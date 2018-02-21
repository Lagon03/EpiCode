/*
**  QrReader I - Search Finder patterns
**  file: fpfind.c
**  description : Locates Qr Code Finder Patterns
*/

# include "fpfind.h"

# define RESET_STATE(_state_)       \
    for (int i = 0 ; i < 5 ; i++)   \
         _state_[i] = 0;

# define PRINT_STATE(_state_)       \
    for (int i = 0 ; i < 5 ; i++)   \
        warn("State %d , size %d", i, _state_[i]);

# define foreach_state(_state_)     \
    for (int i = 0 ; i < 5 ; i++)
        
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
    
    if((abs(module_size - (state[0])) <= max_var) &&
       (abs(module_size - (state[1])) <= max_var) &&
       (abs(module_size * 3 - (state[2])) <= max_var * 3) &&
       (abs(module_size - (state[3])) <= max_var) &&
       (abs(module_size - (state[4])) <= max_var))
        return totalsize;
    else
        return 0;
}

static inline
int get_BW(SDL_Surface *img, int x, int y) //returns 0 if black, 1 if white
{
    Uint32 pixel = getpixel(img, x, y);
    Uint8 r, g, b;
    SDL_GetRGB(pixel, img->format, &r, &g, &b);
    
    if(r == 0)
        return 0;
    else
        return 1;
}

static inline
double get_center(int totalsize, int x)
{
    double half_segment = (double)totalsize / 2.0;
    double center = (double)x - half_segment;
    return center;
}

static inline
double check_ver(SDL_Surface *img, int start_y, int center_x, int state_2, int totalsize)
{
    int h = img->h;
    int state_check[5] = {0};
    int y = start_y;
    
    //upwards check from center
 
    while((y >= 0) && (get_BW(img, center_x, y) == 0))
    {
        state_check[2]++ ;
        y--;
    }
    if(y < 0)
        return 0;

    while((y >= 0) && (get_BW(img, center_x, y) == 1) && (state_check[1] < state_2))
    {
        state_check[1]++ ;
        y--;
    }
    if(y < 0 || state_check[1] >= state_2)
        return 0;

    while((y >= 0) && (get_BW(img, center_x, y) == 0) && (state_check[0] < state_2))
    {
        state_check[0]++ ;
        y--;
    }
    if(y < 0 || state_check[0] >= state_2)
        return 0;
    
    //downwards check from center
    
    y = start_y + 1; 

    while((y <= h) && (get_BW(img, center_x, y) == 0))
    {
        state_check[2]++ ;
        y++;
    }
    if(y > h)
        return 0;

    while((y <= h) && (get_BW(img, center_x, y) == 1) && (state_check[3] < state_2))
    {
        state_check[3]++ ;
        y++;
    }
    if(y > h || state_check[3] >= state_2)
        return 0;

    while((y <= h) && (get_BW(img, center_x, y) == 0) && (state_check[4] < state_2))
    {
        state_check[4]++ ;
        y++;
    }
    if(y > h || state_check[4] >= state_2)
        return 0;

    int check_totalsize = 0;
    foreach_state (state_check)
    {
        check_totalsize += state_check[i];
    }
     
    if(5 * abs(check_totalsize - totalsize) >= 2 * totalsize)
        return 0;

    double center = get_center(check_totalsize, y);
    if(check_ratio(state_check) != 0)
        return center;
    return 0;    
}

static inline
double check_hor(SDL_Surface *img, int center_y, int start_x, int state_2, int totalsize)
{
    int w = img->w;
    int state_check[5] = {0};
    int x = start_x;
    
    //leftwards check from center
    
    while((x >= 0) && (get_BW(img, x, center_y) == 0))
    {
        state_check[2]++ ;
        x--;
    }
    if(x < 0)
        return 0;
 
    while((x >= 0) && (get_BW(img, x, center_y) == 1) && (state_check[1] < state_2))
    {
        state_check[1]++ ;
        x--;
    }
    if(x < 0 || state_check[1] >= state_2)
        return 0;

    while((x >= 0) && (get_BW(img, x, center_y) == 0) && (state_check[0] < state_2))
    {
        state_check[0]++ ;
        x--;
    }
    if(x < 0 || state_check[0] >= state_2)
        return 0;
    
    //rightwards check from center
    
    x = start_x + 1; 
    
    while((x <= w) && (get_BW(img, x, center_y) == 0))
    {
        state_check[2]++ ;
        x++;
    }
    if(x > w)
        return 0;
 
    while((x <= w) && (get_BW(img, x, center_y) == 1) && (state_check[3] < state_2))
    {
        state_check[3]++ ;
        x++;
    }
    if(x > w || state_check[3] >= state_2)
        return 0;

    while((x <= w) && (get_BW(img, x, center_y) == 0) && (state_check[4] < state_2))
    {
        state_check[4]++ ;
        x++;
    }
    if(x > w || state_check[4] >= state_2)
        return 0;
    
    int check_totalsize = 0;
    foreach_state (state_check)
    {
        check_totalsize += state_check[i];
    }
    
    if(5 * abs(check_totalsize - totalsize) >= 2 * totalsize)
        return 0;
    
    double center = get_center(check_totalsize, x);
    if(check_ratio(state_check) != 0)
        return center;
    return 0;    
}

static inline
double check_diag(SDL_Surface *img, int center_y, int center_x, int state_2, int totalsize)
{
    int state_check[5] = {0};
    
    //to top-left check from center
    
    int i = 0;
    
    while((center_y >= i) && (center_x >= i) && (get_BW(img, center_x - i, center_y - i) == 0))
    {
        state_check[2]++ ;
        i++;
    }
    if(center_y < i || center_x < i)
        return 0;
 
    while((center_y >= i) && (center_x >= i) && (get_BW(img, center_x - i, center_y - i) == 1) && (state_check[1] <= state_2))
    {
        state_check[1]++ ;
        i++;
    }
    if(center_y < i ||center_x < i || state_check[1] > state_2)
        return 0;

    while((center_y >= i) && (center_x >= i) && (get_BW(img, center_x - i, center_y - i) == 0) && (state_check[0] <= state_2))
    {
        state_check[0]++ ;
        i++;
    }
    if(state_check[0] > state_2)
        return 0;
    
    //to bottom-right check from center
    int h = img->h;
    int w = img->w;
    
    i = 1; 
    
    while(((center_y + i) < h) && (center_x+1) < w && (get_BW(img, center_x + i, center_y + i) == 0))
    {
        state_check[2]++ ;
        i++;
    }
    if((center_y + i) > h || (center_x + i) >= w)
        return 0;
 
    while(((center_y + i) < h) && ((center_x + i) < w) && (get_BW(img, center_x + i, center_y + i) == 1) && (state_check[3] < state_2))
    {
        state_check[3]++ ;
        i++;
    }
    if((center_y + i) >= h || (center_x + i) >= w || state_check[3] > state_2)
        return 0;

    while(((center_y + i) < h) && ((center_x + i) < w) && (get_BW(img, center_x + i, center_y + i) == 0) && (state_check[4] < state_2))
    {
        state_check[4]++ ;
        i++;
    }
    if((center_y + i) >= h || (center_x + i) >= w || state_check[4] >= state_2)
        return 0;

    int check_totalsize = 0;
    foreach_state (state_check)
    {
        check_totalsize += state_check[i];
    }
      
    if(abs(check_totalsize - totalsize) >= 2 * totalsize)
        return 0;

    if(check_ratio(state_check) != 0)
        return 1;
    return 0;    
}

static inline
int handle_centers(SDL_Surface *img, struct Dmat *centers, int *state,
                   int y, int x, struct Dvector *ems_vector, int totalsize)
{
    //warn("Cross Vertical Check");
    double center_x = get_center(totalsize, x);
    double center_y = check_ver(img, y, (int)center_x, state[2], totalsize);
    if (center_y == 0)
        return 0;

    //warn("Cross Horizontal Check");
    center_x = check_hor(img, center_y, center_x, state[2], totalsize);
    if(center_x == 0)
        return 0;
    
    //warn("Cross Diagonal Check");
    int validation = check_diag(img, center_y, center_x, state[2], totalsize);
    if(validation == 0)
        return 0;
   
    //warn("Add center if not found in centers");
    double *new_center = calloc(2, sizeof(double));
    new_center[0] = center_x;
    new_center[1] = center_y;
    double new_ems = (double)totalsize / 7.0;
    int found = 0;

    foreach_line(centers)
    {
        
        //warn("calc distance %lu/%lu", i, centers->size); 
        double dist_x = abs(mat[i][0] - new_center[0]);
        double dist_y = abs(mat[i][1] - new_center[1]);
        //warn("checking in dmat and dvector");
        if(dist_x < 10 && dist_y < 10)
        {
            mat[i][0] = (mat[i][0] + new_center[0]) / 2.0;
            mat[i][1] = (mat[i][1] + new_center[1]) / 2.0;
            ems_vector->array[i] = (ems_vector->array[i] + new_ems) / 2.0;
            found = 1;
            break;
        }
        //warn("check ok"); 
    }

    if( found == 0)
    {
        //warn("adding to Dmat");
        add_Dmat(centers, new_center);
        //warn("adding to Dvector");
        add_Dvector(ems_vector, new_ems);
    }
    
    //warn("end"); 
    free(new_center);
    return 1;
}

// Main Functions

struct FPat *findFP (SDL_Surface *img)
{
    struct Dvector *ems_vector = init_Dvector(4);
    struct Dmat *centers = init_Dmat(2 , 2);
    int *state = calloc(5, sizeof(int));
    int state_count = 0;
    
    //warn("start");
    
    for(int y = 0; y < img->h; y += PRECISION)
    {
        RESET_STATE(state);
        state_count = 0;
 
        for(int x = 0; x < img->w; x++)
        {
            
            if(get_BW(img, x, y) == 0)
            {
                if(state_count % 2 == 1)
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
                        int totalsize = check_ratio(state);
                        if(totalsize >= 1)
                        {
                            //warn("doing Uber check");
                            handle_centers(img, centers, state, y, x, ems_vector, totalsize);
                            //warn("out"); 
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
                        
                        RESET_STATE(state);
                        state_count = 0;
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
    
    //warn("end");
    free(state);
    struct FPat *foundFP = malloc(sizeof(struct FPat));
    foundFP->centers = centers;
    foundFP->ems_vector = ems_vector;
    return foundFP;
}


