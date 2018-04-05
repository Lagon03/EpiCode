/*
** MAIN FILE FOR SEGMENTATION
** - Free Functions
** - Main Segmentation Function
** - Main Preprocessing Function
*/

# include "segmentation.h"

/*---Free Functions---*/

static inline
void print_mat(char **mat, int size)
{
    printf("\n");
    for(int i = 0; i < size; i++)
    {
        printf("[ ");
        for(int j = 0; j < size; j++)
        {
            printf("%c ", mat[i][j]);
        }
        printf("]\n");
    }
    printf("\n");
}

static inline
void free_FPat(struct FPat *f)
{
    if(f == NULL)
        return;
    free_Dmat(f->centers);
    free_Dvector(f->ems_vector);
    free(f);
}

static inline
void free_FPresults(struct FPresults *f)
{
    if(f == NULL)
        return;
    free(f);
}

static inline
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

static inline
void free_GeoImg(struct GeoImg *g)
{
    free(g->coordA);
    free(g->coordB);
    free(g->coordC);
    SDL_FreeSurface(g->img);
    free(g);
}

static inline
void free_segmentation(struct FPat *f, struct FPresults *fp, struct GeoImg *g, struct QrCode *q)
{
    free_FPat(f);
    free_FPresults(fp);
    free_GeoImg(g);
    free_QrCode(q);
}

void free_PCode(struct PCode *c)
{
    if(c == NULL)
        return;
    free(c->BStream);
    free(c);
}

/*---Main Functions---*/

static inline
void ImageProcessing(SDL_Surface *img)
{
    grayscale(img);
    int threshold = otsu(img);
    if(threshold == 0)
        threshold = 1;
    binarize(img, threshold);
}

static inline 
void ImageProcessingDemo(SDL_Surface *img)
{
    grayscale(img);
    display_image(img);
    int threshold = otsu(img);
    if(threshold == 0)
        threshold = 1;
    binarize(img, threshold);
    display_image(img);
}

struct PCode *Segmentation(SDL_Surface *img)
{
    ImageProcessing(img);
    struct FPat *f = findFP(img);
    struct FPresults *fp = QrCode_found(f);
    if(fp == NULL)
        err(EXIT_FAILURE, "Segmentation error : No Valid QrCode found");
    struct GeoImg *g = GeoTransform(img, fp);
    ImageProcessing(g->img);
    struct QrCode *qr = extract_QrCode(g);
    struct PCode *c = get_code(qr);
    free_segmentation(f, fp, g, qr);
    SDL_FreeSurface(img);
    return c;
}

struct PCode *SegmentationDemo(SDL_Surface *img, SDL_Surface *demo)
{
    printf("--------------------------------------------------------------------------------------\n");
    printf("| Welcome to the demonstration of the segmentation of a QrCode by the Team NotABarcode\n");
    printf("|\n|\n");
    printf("|       Process Starting Now:\n");
    ImageProcessingDemo(img);
    printf("|                   + Image preprocessing Done\n");
    struct FPat *f = findFP(img);
    struct FPresults *fp = QrCode_found(f);
    if(fp == NULL)
    {   
        printf("| No QrCode were found, exit...\n");
        err(EXIT_FAILURE, "Segmentation error : No Valid QrCode found");
    }
    drawFP(demo, f->centers, f->ems_vector, fp->indexA);
    display_image(demo);
    printf("|                   + Finder Pattern and QrCode found and validated\n");
    struct GeoImg *g = GeoTransform(img, fp);
    ImageProcessingDemo(g->img);
    printf("|                   + Affine transformation done\n");
    struct QrCode *qr = extract_QrCode(g);
    display_image(g->img);
    printf("|                   + Version determined -> V = %d\n", qr->version);
    printf("|                   + Alignement Patterns found and image sampled into bit matrix \n");
    printf("| Bit Matrix :\n");
    print_mat(qr->mat, qr->version * 4 + 17);
    struct PCode *c = get_code(qr);
    printf("|                   + Error correction level determined -> ECL = %c\n", c->ECL);
    printf("|                   + Mask determined and unveiled -> Mask = %d\n", c->Mask);
    //print_mat(qr->mat, qr->version * 4 + 17);
    printf("|                   + Bit stream extracted from bit matrix\n");
    printf("Bit Stream : %s\n|\n", c->BStream);
    printf("| The information has been recuperated from the image and now is ready to go through\n");
    printf("| the Reed-Solomon process in order to get corrected and decrypted.\n|\n");
    printf("|            The Segmentation Demo has ended, thank you for watching!\n");
    printf("----------------------------------------------------------------------------------------\n");
    free_segmentation(f, fp, g, qr);
    SDL_FreeSurface(img);
    SDL_FreeSurface(demo);
    return c;
}

struct PCode *SegmentationFromFile(char *File, int Demo)
{
    init_sdl();
    SDL_Surface *img = load_image(File);
    struct PCode *c;
    if(Demo == 1)
    {
        SDL_Surface *demo = load_image(File);
        display_image(img);
        c = SegmentationDemo(img, demo);
    }
    else
    {
        c = Segmentation(img);
    }
    
    SDL_Quit();
    return c;
}

/*int main(int argc, char *argv[])
{
    if(argc > 2 && strcmp(argv[2], "-d") == 0)
        SegmentationFromFile(argv[1], 1);
    else if(argc > 1)
        SegmentationFromFile(argv[1], 0);
    else
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}*/
