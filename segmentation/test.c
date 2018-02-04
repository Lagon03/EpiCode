/*
** Test File and Print on Image File
*/

# include "scan.h"
# include "findFP.h"
# include "../preproc/preproc.h"

# define foreach_seg(_Dmat_) \
    for(size_t i = 0; i < _Dmat_->lines && _Dmat_->mat[i][2] != 0; i++)

//----------------------------Print Functions----------------------------------

void draw_square(SDL_Surface *img, int x_begin, int y_begin, int x_end, int y_end)
{                                                                               
    for(int y = y_begin; y <= y_end; y++)                                       
    {                                                                           
        for(int x = x_begin; x <= x_end; x++)                                   
        putpixel(img, x, y, SDL_MapRGB(img->format, 255, 0, 0));            
    }                                                                           
}

//-----------------------------------------------------------------------------

int main(int argc, char* argv[]){
    init_sdl();
    if( argc > 1){
        SDL_Surface *img = load_image(argv[1]);
        img = grayscale(img);
        img = blackAndWhite(img, 0);
        struct Dmat *hseg_matrix = horizontal_scan (img);
        struct Dmat *vseg_matrix = vertical_scan (img);
        struct Dmat *inter = find_FP(hseg_matrix , vseg_matrix);
       // foreach_seg(inter)
         //   draw_square(img,inter->mat[i][0], inter->mat[i][2], inter->mat[i][1], inter->mat[i][3]);
        print_seg(img, hseg_matrix, 'r');
        free_Dmat(hseg_matrix);
        free(hseg_matrix);
        print_seg(img, vseg_matrix, 'r');
        free_Dmat(vseg_matrix);
        free(vseg_matrix);
        print_seg(img, inter, 'g');
        free_Dmat(inter);
        free(inter);
        display_image(img);  
    }
    SDL_Quit();
    return 1;
}
