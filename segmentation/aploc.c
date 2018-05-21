/*
**  Alignment pattern finder
**  file : aploc.c
**  description : Locates Alignment Patterns
*/

# define RESET_STATE(_state_)       \
    for (int i = 0 ; i < 5 ; i++)   \
         _state_[i] = 0;

# define PRINT_STATE(_state_)       \
    for (int i = 0 ; i < 5 ; i++)   \
        warn("State %d , size %d", i, _state_[i]);

# define foreach_state(_state_)     \
    for (int i = 0 ; i < 5 ; i++)
        
# define PRECISION 1

# include "aploc.h"

static inline                                                                   
int get_BW(SDL_Surface *img, int x, int y) //returns 0 if black, 1 if white     
{                                                                               
     Uint32 pixel = getpixel(img, x, y);                                         
     Uint8 r, g, b;                                                              
     SDL_GetRGB(pixel, img->format, &r, &g, &b);
     //printf("color at %d %d: %d \n", x, y, g);
     //putpixel(img, x, y, SDL_MapRGB(img->format, 255, 0, 0)); 
     if(g == 0)                                                                  
        return 0;                                                               
     else                                                                        
        return 1; 
}

static inline
int check_ratio(int *state)
{
    int totsize = 0;
    for(int i = 1; i < 4; i++)
        totsize += state[i];
    
    if (totsize < 3)
        return 0;
   
    //printf("state %d %d %d | %lf\n", state[0], state[1], state[2], msize);

    double msize = totsize/3;
    double max_var = msize/1.5;
    if((abs(msize - state[1]) <= max_var) &&
       (abs(msize - state[2]) <= max_var) &&
       (abs(msize - state[3]) <= max_var))
        return totsize;
    else
        return 0;
}

static inline
double get_center(int totalsize, int x)
{
    double half_segment = (double)totalsize / 2.0;
    double center = (double)x - half_segment;
    return center;
}

static inline
double check_ver(SDL_Surface *img, int start_y, int center_x, int totalsize)
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

    while((y >= 0) && (get_BW(img, center_x, y) == 1))
    {
        state_check[1]++ ;
        y--;
    }
    if(y < 0)
        return 0;

    while((y >= 0) && (get_BW(img, center_x, y) == 0))
    {
        state_check[0]++ ;
        y--;
    }
    if(y < 0)
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

    while((y <= h) && (get_BW(img, center_x, y) == 1))
    {
        state_check[3]++ ;
        y++;
    }
    if(y > h)
        return 0;

    while((y <= h) && (get_BW(img, center_x, y) == 0))
    {
        state_check[4]++ ;
        y++;
    }
    if(y > h)
        return 0;

    int check_totalsize = 0;
    foreach_state (state_check)
    {
        check_totalsize += state_check[i];
    }
    check_totalsize -= state_check[4]; 
    check_totalsize -= state_check[0];
    if(5 * abs(check_totalsize - totalsize) >= 2 * totalsize)
        return 0;

    double center = get_center(check_totalsize, y - state_check[4]);
    if(check_ratio(state_check) != 0)
        return center;
    return 0;    
}

static inline
double check_hor(SDL_Surface *img, int center_y, int start_x, int totalsize)
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
 
    while((x >= 0) && (get_BW(img, x, center_y) == 1))
    {
        state_check[1]++ ;
        x--;
    }
    if(x < 0)
        return 0;

    while((x >= 0) && (get_BW(img, x, center_y) == 0))
    {
        state_check[0]++ ;
        x--;
    }
    if(x < 0)
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
 
    while((x <= w) && (get_BW(img, x, center_y) == 1))
    {
        state_check[3]++ ;
        x++;
    }
    if(x > w)
        return 0;

    while((x <= w) && (get_BW(img, x, center_y) == 0))
    {
        state_check[4]++ ;
        x++;
    }
    if(x > w)
        return 0;
    
    int check_totalsize = 0;
    foreach_state (state_check)
    {
        check_totalsize += state_check[i];
    }
    check_totalsize -= state_check[4];
    check_totalsize -= state_check[0];
    if(5 * abs(check_totalsize - totalsize) >= 2 * totalsize)
        return 0;
    
    double center = get_center(check_totalsize, x - state_check[4]);
    if(check_ratio(state_check) != 0)
        return center;
    return 0;    
}

static inline
double check_diag(SDL_Surface *img, int center_y, int center_x, int totalsize)
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
 
    while((center_y >= i) && (center_x >= i) && (get_BW(img, center_x - i, center_y - i) == 1))
    {
        state_check[1]++ ;
        i++;
    }
    if(center_y < i ||center_x < i)
        return 0;

    while((center_y >= i) && (center_x >= i) && (get_BW(img, center_x - i, center_y - i) == 0))
    {
        state_check[0]++ ;
        i++;
    }
    
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
 
    while(((center_y + i) < h) && ((center_x + i) < w) && (get_BW(img, center_x + i, center_y + i) == 1))
    {
        state_check[3]++ ;
        i++;
    }
    if((center_y + i) >= h || (center_x + i) >= w)
        return 0;

    while(((center_y + i) < h) && ((center_x + i) < w) && (get_BW(img, center_x + i, center_y + i) == 0))
    {
        state_check[4]++ ;
        i++;
    }
    if((center_y + i) >= h || (center_x + i) >= w)
        return 0;

    int check_totalsize = 0;
    foreach_state (state_check)
    {
        check_totalsize += state_check[i];
    }
    check_totalsize -= state_check[0];
    check_totalsize -= state_check[4];
    if(abs(check_totalsize - totalsize) >= 2 * totalsize)
        return 0;

    if(check_ratio(state_check) != 0)
        return 1;
    return 0;    
}

static inline
int handle_centers(SDL_Surface *img, struct Dmat *centers, int *state, int y,
                                                        int x, int totalsize)
{
    x = x - state[4];
    double center_x = get_center(totalsize, x);
    double center_y = check_ver(img, y, (int)center_x, totalsize);
    if(center_y == 0)
        return 0;
    
    center_x = check_hor(img, center_y, center_x, totalsize);
    if(center_x == 0)
        return 0;

    int validation = check_diag(img, center_y, center_x, totalsize);
    if(validation == 0)
        return 0;
    
    double *new_center = calloc(2, sizeof(double));
    new_center[0] = center_x;
    new_center[1] = center_y;
    int found = 0;
    
    foreach_line(centers)
    {
        double dist_x = abs(mat[i][0] - new_center[0]);
        double dist_y = abs(mat[i][1] - new_center[1]);
        if(dist_x < 10 && dist_y < 10)
        {
            mat[i][0] = (mat[i][0] + new_center[0]) / 2.0; 
            mat[i][1] = (mat[i][1] + new_center[1]) / 2.0;
            found = 1;
            break;
        }
    }
    
    if(found == 0)
    {
        add_Dmat(centers, new_center);
    }
    free(new_center);
    return 1;
}

void ScanAP(SDL_Surface *img, double *Px, double *Py)
{
    //Setting scan bounds
    int scanS = 50;
    int TLx = *Px - scanS/2;
    int TLy = *Py - scanS/2;
    int BRx = *Px + scanS/2;
    int BRy = *Py + scanS/2;
    
    struct Dmat *centers = init_Dmat(2, 2);
    int *state = calloc(5, sizeof(int));
    int state_count = 0;
    
    //Scan
    for(int y = TLy; y < BRy; y++)
    {
        RESET_STATE(state);
        state_count = 0;
        for(int x = TLx; x < BRx; x++)
        {
            if(get_BW(img, x, y) == 0)
            {
                if(state_count % 2 == 1)
                    state_count++;
                state[state_count]++;
            }
            else
            {
                if(state_count % 2 == 1)
                    state[state_count]++;
                else
                {
                    if(state_count == 2)
                    {
                        int totalsize = check_ratio(state);
                        if(totalsize >= 1)
                        {
                            handle_centers(img, centers, state, y, x, totalsize);
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
                        state_count++;
                        state[state_count]++;
                    }
                }
            }
        }
    }
    
    free(state);   
    foreach_line(centers)
    {
        double dist_x = abs(mat[i][0] - *Px)/ 2.0;
        double dist_y = abs(mat[i][1] - *Py)/ 2.0;
        if(dist_x < 10 && dist_y < 10)
        {
            *Px = mat[i][0];
            *Py = mat[i][1];
        }
    }
}
