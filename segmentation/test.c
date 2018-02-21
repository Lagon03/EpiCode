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

void free_Fpat(struct FPat *f)
{
    free_Dmat(f->centers);
    free_Dvector(f->ems_vector);
    free(f);
}

void free_Finder(struct Finder *f)
{
    free(f->center);
    free(f->ul);
    free(f->dr);
    free(f);
}

void free_QrCode(struct QrCode *q)
{
    free_Finder(q->A);
    free_Finder(q->B);
    free_Finder(q->C);
    
    for(int i = 0; i < (q->version * 4 + 17); i++)
    {
        free(q->mat[i]);
    }
    free(q->mat);
    free(q);
}

void free_PCode(struct PCode *c)
{
    free(c->msg);
    free(c->err_cor);
    free(c);
}

void free_segmentation(struct FPat *f, struct QrCode *q, struct PCode *c)
{
    free_Fpat(f);
    free_QrCode(q);
    free_PCode(c);
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
       
    struct FPat *FPs = findFP (img);
    //warn("check fpfind");

    struct QrCode *qr = extract_QrCode_NoG(img, FPs); 
    
    //warn("extracted code");
    print_mat(qr->mat, qr->version * 4 + 17);
    struct PCode *code = get_code(qr);
    //warn("processed code");
    print_mat(qr->mat, qr->version * 4 + 17);
     
    //free_segmentation(FPs, qr, code);
    display_image(img);
    SDL_Quit();
    return 1;
}
