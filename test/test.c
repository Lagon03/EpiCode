/*
** Test File for QrReader class
*/

# include "fpfind.h"
# include "../preproc/preproc.h"

//----------------------------AUX Functions----------------------------------

//-----------------------------------------------------------------------------

int main(int argc, char* argv[]){
    init_sdl();
    if( argc > 1){
        SDL_Surface *img = load_image(argv[1]);
        img = grayscale(img);
        img = blackAndWhite(img, 0);
        
        struct FPat *FPs = findFP (img);
        warn("end"); 
        display_image(img);  
    }
    SDL_Quit();
    return 1;
}
