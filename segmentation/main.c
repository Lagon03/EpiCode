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

//---------------------------Debug Functions----------------------------------

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

void show_seg(SDL_Surface *show, struct FPat *f, int A)
{
    Draw_point(show, f->centers->mat[0][0], f->centers->mat[0][1]);
    Draw_point(show, f->centers->mat[1][0], f->centers->mat[1][1]);
    Draw_point(show, f->centers->mat[2][0], f->centers->mat[2][1]);
    
    Draw_line(show,f->centers->mat[0][0], f->centers->mat[0][1],
                    f->centers->mat[1][0], f->centers->mat[1][1]);
    Draw_line(show,f->centers->mat[0][0], f->centers->mat[0][1],
                    f->centers->mat[2][0], f->centers->mat[2][1]);
    Draw_line(show,f->centers->mat[1][0], f->centers->mat[1][1],
                    f->centers->mat[2][0], f->centers->mat[2][1]);
    drawFP(show, f->centers, f->ems_vector, A);

    display_image(show);
}

//----------------------------Free Functions----------------------------------

void free_FPat(struct FPat *f)
{
    if(f == NULL)
        return;
    free_Dmat(f->centers);
    free_Dvector(f->ems_vector);
    free(f);
}

void free_FPresults(struct FPresults *f)
{
    if(f == NULL)
        return;
    free(f);
}

void free_QrCode(struct QrCode *q)
{
    if(q == NULL)
        return;
  
    for(int i = 0; i < (q->version * 4 + 17); i++)
    {
        free(q->mat[i]);
    }
    free(q->mat);
    free(q);
}

void free_PCode(struct PCode *c)
{
    if(c == NULL)
        return;
    free(c->BStream);
    free(c);
}

void free_segmentation(struct FPat *f, struct FPresults *fp,  struct QrCode *q, struct PCode *c)
{
    free_FPat(f);
    free_FPresults(fp);
    free_QrCode(q);
    free_PCode(c);
}

//----------------------------------------------------------------------------

void imageProcessing(SDL_Surface *img)
{
    grayscale(img);
    int threshold = otsu(img);
    if (threshold == 0)
        threshold = 1;
    binarize(img, threshold);
}

void fPiter(char *file)
{
    SDL_Surface *img = load_image(file);
    SDL_Surface *show = load_image(file);
    imageProcessing(img);

    display_image(show);
    struct FPat *f = findFP(img);
    struct FPresults *fp = QrCode_found(f);
    if(fp == NULL)
        err(EXIT_FAILURE, "Segmentation error : x00"); 
   
    struct GeoImg *geo = GeoTransform(img, fp);
    display_image(geo->img);
    free_FPat(f);
    free_FPresults(fp);
    struct QrCode *qr = extract_QrCode(geo);
    Draw_point(geo->img, geo->coordA[0], geo->coordA[1]);
    Draw_point(geo->img, geo->coordB[0], geo->coordB[1]);
    Draw_point(geo->img, geo->coordC[0], geo->coordC[1]);
    display_image(geo->img);
    struct PCode *c = get_code(qr);
    printf("QrCode found :\n");
    printf("    Version : %d\n", c->Version); 
    printf("    Mask : %d\n", c->Mask);
    printf("    Error Correction Level : %c\n", c->ECL);
    printf("Bit Stream Ready to be decoded : \n");
    printf("%s \n", c->BStream);
}

/*
void full_segmentation(char *file)
{
    SDL_Surface *img = load_image(file);
    SDL_Surface *show = load_image(file);
    imageProcessing(img);
    
    //struct FPat *f = findFP(img);
    
    //drawFP(show, f->centers, f->ems_vector, 0);
     
    //struct QrCode *qr = extract_QrCode(img, f);
    printf("\n\nQrCode Found : \n");
    printf("Version : V%0d \n", qr->version);
    printf("Estimated module size : %lf \n", qr->m_size);
    //printf("QrCode Data Matrix : \n");
    //print_mat(qr->mat, qr->version * 4 + 17);
    printf("\n");
    
    display_image(show);
    
    //struct PCode *c = get_code(qr);
    printf("Format :\n");
    printf("    Mask : %d\n", c->mask);
    printf("    Error Correction Level : %c\n", c->err_cor_lvl);
    printf("    Error Correction Bits : %s\n\n", c->err_cor);
    printf("Cyphered Message Ready to be decoded : \n");
    printf("%s \n", c->msg);
    
    free_segmentation(f, NULL, qr, c);
}
*/

int main(int argc, char *argv[]){
    init_sdl();
   
    if (argc > 1)
        fPiter(argv[1]);
    else
        fPiter("../resources/HelloWorldv1.png"); 

    SDL_Quit();
    return 1;
}
