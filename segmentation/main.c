/*
** Test File for QrReader class
*/

# include "proc_code.h"
# include "extcode.h"
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
            printf("%c", mat[i][j]);
        }
        printf(" ]\n");
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
    
    drawFP(show, f->centers, f->ems_vector);
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
    
    drawFP(show, f->centers, f->ems_vector);
     
    struct QrCode *qr = extract_QrCode_NoG(img, f);
    printf("\n\nQrCode Found : \n");
    printf("Version : V%0d \n", qr->version);
    printf("Estimated module size : %lf \n", qr->m_size);
    printf("QrCode Data Matrix : \n");
    print_mat(qr->mat, qr->version * 4 + 17);
    printf("\n");
    
    display_image(show);
    
    getchar();
    
    struct PCode *c = get_code(qr);
    printf("Format :\n");
    printf("    Mask : %d\n", c->mask);
    printf("    Error Correction Level : %c\n", c->err_cor_lvl);
    printf("    Error Correction Bits : %s\n\n", c->err_cor);
    printf("Cyphered Message Ready to be decoded : \n");
    printf("%s", c->msg);
    
    free_segmentation(f, qr, c);
}

int main(void){
    init_sdl();
    
    //Show FP location
    
    fPiter("../resources/HelloWorldv1.png");
    fPiter("../resources/HelloWorldv1skew.png");
    fPiter("../resources/HelloWorldv1skew2.png");
    fPiter("../resources/QrV2.png");
    fPiter("../resources/QrV7.png");
    fPiter("../resources/QrV8.png");
    fPiter("../resources/QrV40.png");
    fPiter("../resources/ralphl.jpg");
    fPiter("../resources/1t.jpg");
    fPiter("../resources/funeral-qr-code.jpg");
    fPiter("../resources/wall.jpg");
    fPiter("../resources/qrrltest.jpg");

    //Retrieve code
    getchar();
    full_segmentation("../resources/HelloWorldv1.png");
    getchar();
    full_segmentation("../resources/QrV2.png");
    getchar();
    full_segmentation("../resources/QrV7.png");
    getchar();
    full_segmentation("../resources/QrV8.png");
    getchar();
    full_segmentation("../resources/QrV40.png");
    getchar(); 
    //free_segmentation(FPs, qr, code);
    SDL_Quit();
    return 1;
}
