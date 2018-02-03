/*
** Test File and Print on Image File
*/

#include "scan.h"
#include "../preproc/preproc.h"

//----------------------------Print Functions----------------------------------



//-----------------------------------------------------------------------------

int main(int argc, char* argv[]){
    init_sdl();
    if( argc > 1){
        SDL_Surface *img = load_image(argv[1]);
        img = grayscale(img);
        img = blackAndWhite(img, 0);
        struct Dmat *hseg_matrix = horizontal_scan (img);
        struct Dmat *vseg_matrix = vertical_scan (img);
        //draw_hline(img, 16, 72, 40);
        //draw_hline(img, 16, 72, 50);
        //draw_hline(img, 16, 72, 150);
        //draw_hline(img, 16, 72, 160);
        print_seg(img, hseg_matrix);
        free_Dmat(hseg_matrix);
        free(hseg_matrix);
        print_seg(img, vseg_matrix);
        free_Dmat(vseg_matrix);
        free(vseg_matrix);
        display_image(img);  
    }
    SDL_Quit();
    return 1;
}
