/*
** Test File for QrReader class
*/

# include "geotrans.h"
# include "proc_code.h"
# include "extcode.h"
# include "FPValid.h"
# include "fpfind.h"
# include "../preproc/preproc.h"
# include "surfdraw.h"
# include <stdio.h>

//----------------------------AUX Functions----------------------------------

void print_mat(char **mat, int size)
{
    for(int i = 0; i < size; i++)
    {
        printf("[ ");
        for(int j = 0; j < size; j++)
        {
            printf("%c ", mat[i][j]);
        }
        printf("]\n");
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

//----------------------------------------------------------------------------

void fPiter(char *file)
{
    SDL_Surface *img = load_image(file);
    SDL_Surface *show = load_image(file);
    
    img = grayscale(img);
    img = blackAndWhite(img, 0);
    
    struct FPat *f = findFP(img);
    struct FPresults *tmp = QrCode_found(f);
    if(tmp == NULL)
        err(EXIT_FAILURE, "Sorry, i didn't find any QrCodes"); 
    
    printf("Finder pattern A is : %d \n ", tmp->indexA);
   
    Draw_point(show, f->centers->mat[0][0], f->centers->mat[0][1]);
    Draw_point(show, f->centers->mat[1][0], f->centers->mat[1][1]);
    Draw_point(show, f->centers->mat[2][0], f->centers->mat[2][1]);
    
    Draw_line(show,f->centers->mat[0][0], f->centers->mat[0][1],
                    f->centers->mat[1][0], f->centers->mat[1][1]);
    Draw_line(show,f->centers->mat[0][0], f->centers->mat[0][1],
                    f->centers->mat[2][0], f->centers->mat[2][1]);
    Draw_line(show,f->centers->mat[1][0], f->centers->mat[1][1],
                    f->centers->mat[2][0], f->centers->mat[2][1]);
    printf("Affine Transformation \n");
    double *sol = SolveAffineEquations(tmp->x1, tmp->y1, tmp->x2, tmp->y2, tmp->x3, tmp->y3, tmp->dist);
    SDL_Surface *trans = AffineTransformation(show, sol, tmp->dist);
    //drawFP(show, f->centers, f->ems_vector, A);
    display_image(show);
    free_Fpat(f);
}

void full_segmentation(char *file)
{
    SDL_Surface *img = load_image(file);
    SDL_Surface *show = load_image(file);
    img = grayscale(img);
    img = blackAndWhite(img, 0);
    
    struct FPat *f = findFP(img);
    
    drawFP(show, f->centers, f->ems_vector, 0);
     
    struct QrCode *qr = extract_QrCode_NoG(img, f);
    printf("\n\nQrCode Found : \n");
    printf("Version : V%0d \n", qr->version);
    printf("Estimated module size : %lf \n", qr->m_size);
    printf("QrCode Data Matrix : \n");
    print_mat(qr->mat, qr->version * 4 + 17);
    printf("\n");
    
    display_image(show);
    
    struct PCode *c = get_code(qr);
    printf("Format :\n");
    printf("    Mask : %d\n", c->mask);
    printf("    Error Correction Level : %c\n", c->err_cor_lvl);
    printf("    Error Correction Bits : %s\n\n", c->err_cor);
    printf("Cyphered Message Ready to be decoded : \n");
    printf("%s \n", c->msg);
    
    free_segmentation(f, qr, c);
}

int main(int argc, char *argv[]){
    init_sdl();
   
    if (argc > 1)
        fPiter(argv[1]);
    else
        fPiter("../resources/wall.jpg"); 
    //full_segmentation("../resources/QrV8.png");
 
    //free_segmentation(FPs, qr, code);
    SDL_Quit();
    return 1;
}
