/*
**  Drawing functions
**  file : surfdraw.c
**  description : Draws on the image functions
*/

# include "surfdraw.h"

// Sub Functions

static inline                                                                   
void draw_line(SDL_Surface *img, int x_begin, int y_begin, int x_end, int
y_end)
{                                                                               
    for(int y = y_begin; y <= y_end; y++)                                      
    {                                                                          
         for(int x = x_begin; x <= x_end; x++)                               
            putpixel(img, x, y, SDL_MapRGB(img->format, 255, 0, 0));        
    }                                                                          
}                                                                               
                                                                                 
                                                                                 
void drawFP(SDL_Surface *img, struct Dmat *centers, struct Dvector
*ems_vector) 
{                                                                               
    int wline = 5;                                                              
                                                                                 
    if(centers->size == 0)                                                      
        return;                                                                 
                                                                                 
    foreach_line(centers)                                                       
    {                                                                           
        double center_x = mat[i][0];                                            
        double center_y = mat[i][1];                                            
        double ems = ems_vector->array[i];                                      
                                                                                 
        double corner_tl_x = center_x - ems * 3.5;                              
        double corner_tl_y = center_y - ems * 3.5;                              
        double corner_br_x = center_x + ems * 3.5;                              
        double corner_br_y = center_y + ems * 3.5;                              
         draw_line(img, (int)corner_tl_x - wline , (int)corner_tl_y - wline,
                  (int)corner_tl_x, (int)corner_br_y + wline);
         draw_line(img, (int)corner_br_x, (int)corner_tl_y - wline,
                   (int)corner_br_x + wline, (int)corner_br_y + wline);
         draw_line(img, (int)corner_tl_x , (int)corner_tl_y - wline,
                  (int)corner_br_x, (int)corner_tl_y);
         draw_line(img, (int)corner_tl_x , (int)corner_br_y,
                  (int)corner_br_x, (int)corner_br_y + wline);              
    }                                                                           
}

void drawBox(SDL_Surface *img, double *coords) 
{                                                                               
    int wline = 5;

    double corner_tl_x = coords[0];                              
    double corner_tl_y = coords[1];                              
    double corner_br_x = coords[2];                              
    double corner_br_y = coords[3];                              
    draw_line(img, (int)corner_tl_x - wline , (int)corner_tl_y - wline,
                   (int)corner_tl_x, (int)corner_br_y + wline);
    draw_line(img, (int)corner_br_x, (int)corner_tl_y - wline,
                   (int)corner_br_x + wline, (int)corner_br_y + wline);
    draw_line(img, (int)corner_tl_x , (int)corner_tl_y - wline,
                   (int)corner_br_x, (int)corner_tl_y);
    draw_line(img, (int)corner_tl_x , (int)corner_br_y,
                   (int)corner_br_x, (int)corner_br_y + wline);
}
