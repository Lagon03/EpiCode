/*
** Test File for QrReader class
*/

# include "proc_code.h"
# include "extcode.h"
# include "fpfind.h"
# include "../preproc/preproc.h"
# include <stdio.h>

//----------------------------AUX Functions----------------------------------

void print_mat(char **mat, int size)
{
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            printf("%c", mat[i][j]);
        }
      printf("\n");
    }
}

//-----------------------------------------------------------------------------

int main(int argc, char* argv[]){
    init_sdl();
    if( argc <= 1)
        err(EXIT_FAILURE, "no argument");

    //Preprocessing - 2nd phase
    SDL_Surface *img = load_image(argv[1]);
    img = grayscale(img);
    img = blackAndWhite(img, 0);
    
// QRcode Extraction

    //QrCode Localization    
    struct FPat *FPs = findFP (img);
    //warn("check fpfind");
        // ADD CHECKS  - 2nd phase -> if more than 3 FPs / find A, B, C FP
    
    //Size_estimation  - 1st phase
    struct QrCode *qr = extract_QrCode_NoG(img, FPs); 
    
    print_mat(qr->mat, qr->version * 4 + 17);
    //Aligment pattern  1st/ 2nd phase
    struct PCode *code = get_code(qr);
// End Extraction
    print_mat(qr->mat, qr->version * 4 + 17);
    //Geometric rectification  - 2nd phase
    
    //Reed-Solomon  - 1st phase

    //Decode text  - 1st phase
    
    display_image(img);
  
    SDL_Quit();
    return 1;
}
