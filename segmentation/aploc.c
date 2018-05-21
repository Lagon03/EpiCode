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
int checkRatio(int *state, double msize)
{
    int totsize = 0;
    for(int i = 0; i < 3; i++)
        totsize += state[i];
    
    if (totsize < 3)
        return 0;
   
    //printf("state %d %d %d | %lf\n", state[0], state[1], state[2], msize);

    double max_var = msize/1.5;
    if((abs(msize - state[0]) <= max_var) &&
       (abs(msize - state[1]) <= max_var) &&
       (abs(msize - state[2]) <= max_var))
        return totsize;
    else
        return 0;
}

static inline
double VerifyCenterH(SDL_Surface *img, int center_x, int center_y, double CPA)
{
    int w = img->w;
    int state[3] = {0};
    int cr = 0;
    int cl = 0;
    int x = center_x;
    
    while((x >= 0) && (get_BW(img, x, center_y) == 0))
    {
        cl++;
        state[1]++;
        x--;
    }
    if(x < 0)
        return -CPA;
    
    while((x >= 0) && get_BW(img, x, center_y) == 1)
    {
        cl++;
        state[0]++;
        x--;
    }
    if(x < 0)
        return -CPA;
    
    x = center_x + 1;
     
    while((x < w) && (get_BW(img, x, center_y) == 0))
    {
        cr++;
        state[1]++;
        x++;
    }
    if(x >= w)
        return -CPA;
    
    while((x < w) && get_BW(img, x, center_y) == 1)
    {
        cr++;
        state[2]++;
        x++;
    }
    if(x >= w)
        return -CPA;
    
    int totsize = checkRatio(state, CPA);
    
    if(totsize == 0)
        return -CPA;
    
    double dif = abs(cr - cl);
    
    if(cl >= cr)
        return -dif/2;
    else
        return +dif/2;
}

static inline
double VerifyCenterV(SDL_Surface *img, int center_x, int center_y, double CPA)
{
    int h = img->h;
    int state[3] = {0};
    int cu = 0;
    int cd = 0;
    int y = center_y;
    
    while((y >= 0) && (get_BW(img, center_x, y) == 0))
    {
        cu++;
        state[1]++;
        y--;
    }
    if(y < 0)
        return -CPA;
    
    while((y >= 0) && get_BW(img, center_x, y) == 1)
    {
        cu++;
        state[0]++;
        y--;
    }
    if(y < 0)
        return -CPA;
    
    y = center_y + 1;
     
    while((y < h) && (get_BW(img, center_x, y) == 0))
    {
        cd++;
        state[1]++;
        y++;
    }
    if(y >= h)
        return -CPA;
    
    while((y < h) && get_BW(img, center_x, y) == 1)
    {
        cd++;
        state[2]++;
        y++;
    }
    if(y >= h)
        return -CPA;
    
    int totsize = checkRatio(state, CPA);
    
    if(totsize == 0)
        return -CPA;
    
    double dif = abs(cd - cu);
     
    if(cu >= cd)
        return -dif/2;
    else
        return +dif/2;
}

static inline
int VerifyCenterD(SDL_Surface *img, int center_x, int center_y, double CPA)
{
    int h = img->h;
    int w = img->w;
    int state[3] = {0};
    int i = 0;
    
    while((center_y >= i) && (center_x >= i) && (get_BW(img, center_x - i, center_y - i) == 0))
    {
        state[1]++;
        i++;
    }
    if(center_y < i || center_x < i)
        return 0;
    
    while((center_y >= i) && (center_x >= i) &&  get_BW(img, center_x - i, center_y - i) == 1)
    {
        state[0]++;
        i++;
    }
    if(center_y < i || center_x < i)
        return 0;
    
    i = 1;
     
    while((center_y + i < h) && (center_x + i < w) && (get_BW(img, center_x + i, center_y + i) == 0))
    {
        state[1]++;
        i++;
    }
    if(center_y + i >= h || center_x + i >= w)
        return 0;
    
    while((center_y + i < h) && (center_x + i < w) &&  get_BW(img, center_x + i, center_y + i) == 1)
    {
        state[2]++;
        i++;
    }
    if(center_y + i >= h || center_x + i >= w)
        return 0;
    
    int totsize = checkRatio(state, CPA);
    
    if(totsize == 0)
        return 0;
    return 1;
}

static inline
void LocateAP(SDL_Surface *img, double *Px, double *Py, double CPx, double CPy) //version 2 - 6 for now
{
    //printf("color at %lf/ %lf : %d\n", P[0], P[1], get_BW(img, P[0], P[1]));
    if(get_BW(img, *Px, *Py) == 1)
    {
        err(EXIT_FAILURE, "Segmentation not implemented : AP not found x00");
    }
    //check horizontal
    int check = VerifyCenterH(img, *Px, *Py, CPx);
    if(check == -CPx)
    {
        err(EXIT_FAILURE, "Segmentation not implemented : AP not found x01");
    }
    
    *Px += check;   
    //check vertical
    check = VerifyCenterV(img, *Px, *Py, CPy);
    if(check == -CPy)
    {
        err(EXIT_FAILURE, "Segmentation not implemented : AP not found x02");
    }
    
    *Py += check;
    //check diagonal
    if(VerifyCenterD(img, *Px, *Py, (CPx + CPy)/2) == 0)
    {
        err(EXIT_FAILURE, "Segmentation not implemented : AP not found x03");
    }
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
}
