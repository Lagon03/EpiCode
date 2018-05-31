/*
**  Drawing functions
**  file : surfdraw.c
**  description : Draws on the image functions
*/

# include "surfdraw.h"

// Sub Functions

    
static inline                                                                   
void draw_line(SDL_Surface *img, int x_begin, int y_begin, int x_end, int
y_end, char c)
{       
    Uint8 r = 255;
    Uint8 g = 0;
    Uint8 b = 0;          
    if( c == 'b')
    {
        r = 0;
        g = 0;
        b = 255;
    }
    else if (c == 'g')
    {
        r = 0;
        g = 255;
        b = 0;
    }
                                                              
    for(int y = y_begin; y <= y_end; y++)                                      
    {                                                                          
         for(int x = x_begin; x <= x_end; x++)                               
            putpixel(img, x, y, SDL_MapRGB(img->format, r, g, b));        
    }                                                                          
}                                                                               

void Draw_point(SDL_Surface *img, int x, int y)
{
    x -= 5;
    y -= 5;
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 10; j++)
            putpixel(img, x + i, y + j, SDL_MapRGB(img->format, 255, 0, 0));
    }
}                                                                                
                                                                                 
void drawFP(SDL_Surface *img, struct Dmat *centers, struct Dvector
*ems_vector, int A) 
{                                                                               
    int wline = 5;                                                                             
    if(centers->size == 0)                                                      
        return;                                                                 
    
    char c = 'r';                                                                             
    foreach_line(centers)                                                       
    {   
        if (i == (size_t)A)
            c = 'g';                                                                   
        double center_x = mat[i][0];                                            
        double center_y = mat[i][1];                                            
        double ems = ems_vector->array[i];                                      
                                                                                 
        double corner_tl_x = center_x - ems * 3.5;                              
        double corner_tl_y = center_y - ems * 3.5;                              
        double corner_br_x = center_x + ems * 3.5;                              
        double corner_br_y = center_y + ems * 3.5;                              
        draw_line(img, (int)corner_tl_x - wline , (int)corner_tl_y - wline,
                  (int)corner_tl_x, (int)corner_br_y + wline, c);
        draw_line(img, (int)corner_br_x, (int)corner_tl_y - wline,
                   (int)corner_br_x + wline, (int)corner_br_y + wline, c);
        draw_line(img, (int)corner_tl_x , (int)corner_tl_y - wline,
                  (int)corner_br_x, (int)corner_tl_y, c);
        draw_line(img, (int)corner_tl_x , (int)corner_br_y,
                  (int)corner_br_x, (int)corner_br_y + wline, c);
        c = 'r';
    }                                                                           
}

void Draw_line(SDL_Surface *img, int x0, int y0, int x1, int y1)
{
    if(x0 > x1 || y0 > y1)
    {
        int tmp = x1;
        x1 = x0;
        x0 = tmp;
        tmp = y1;
        y1 = y0;
        y0 = tmp;
    }
    
    double dy = y1 - y0;
    double dx = x1 - x0;
    double m = dy/dx;
    
    if( m <= 1)
    { 
        double y = y0;
        for(int x = x0; x <= x1; x++)
        {
            putpixel(img, x, (int)round(y), SDL_MapRGB(img->format, 0, 255, 0));
            y += m;
        }
    }
    else
    { 
        double x = x0;
        for(int y = y0; y <= y1; y++)
        {
            putpixel(img, (int)round(x), y, SDL_MapRGB(img->format, 0, 255, 0));
            x += 1/m;
        }
    }
}

/*void drawBox(SDL_Surface *img, double *coords) 
{   
                                                                                
    int wline = 5;

    double corner_tl_x = coords[0];                              
    double corner_tl_y = coords[1];                              
    double corner_br_x = coords[2];                              
    double corner_br_y = coords[3];
   
    double center_x = (corner_tl_x + corner_br_x) / 2;
    double center_y = (cornet_tl_y + corner_br_y) / 2;                              
    draw_line(img, (int)corner_tl_x - wline , (int)corner_tl_y - wline,
                   (int)corner_tl_x, (int)corner_br_y + wline);
    draw_line(img, (int)corner_br_x, (int)corner_tl_y - wline,
                   (int)corner_br_x + wline, (int)corner_br_y + wline);
    draw_line(img, (int)corner_tl_x , (int)corner_tl_y - wline,
                   (int)corner_br_x, (int)corner_tl_y);
    draw_line(img, (int)corner_tl_x , (int)corner_br_y,
                   (int)corner_br_x, (int)corner_br_y + wline);
}*/
