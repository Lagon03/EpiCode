/*
**  Validate QrCode Existence from Finder Patterns
**  file : FPValid.c
**  desrciption : Validates whether or not if there is a QrCode, and wether
**  it is workable, retrieves afterwards FP A, B, C
*/

# include "FPValid.h"

static inline
struct FPresults *Find_QrCodes(void)
{
    err(EXIT_FAILURE, "Segmentation not implemented : possible multiple QrCodes");
    return NULL;
}

struct FPresults *QrCode_found(struct FPat *fp)
{
    if(fp->centers->size < 3)
        return NULL;

    //Multiple Case not handled
    if(fp->centers->size > 3)
    {
        return Find_QrCodes();
    }
    
    //Check distances
    double dist1_2 = sqrt(pow(fp->centers->mat[1][0] - fp->centers->mat[0][0],2)
                        + pow(fp->centers->mat[1][1] - fp->centers->mat[0][1],2));
    double dist1_3 = sqrt(pow(fp->centers->mat[2][0] - fp->centers->mat[0][0],2)
                        + pow(fp->centers->mat[2][1] - fp->centers->mat[0][1],2));
    double dist2_3 = sqrt(pow(fp->centers->mat[1][0] - fp->centers->mat[2][0],2)
                        + pow(fp->centers->mat[1][1] - fp->centers->mat[2][1],2));
    
    int indexA;
    struct FPresults *ret = malloc(sizeof(struct FPresults));
    
    if(dist1_2 > dist1_3 && dist1_2 > dist2_3)
    {
        indexA = 2;
        ret->indexA = indexA;
        ret->dist = dist1_3;
        ret->x1 = fp->centers->mat[2][0];
        ret->x2 = fp->centers->mat[0][0];
        ret->x3 = fp->centers->mat[1][0];
        ret->y1 = fp->centers->mat[2][1];
        ret->y2 = fp->centers->mat[0][1];
        ret->y3 = fp->centers->mat[1][1];
        
    }
    else if(dist1_3 > dist1_2 && dist1_3 > dist2_3)
    {
        indexA = 1;
        ret->indexA = indexA;
        ret->dist = dist1_2;
        ret->x1 = fp->centers->mat[1][0];
        ret->x2 = fp->centers->mat[2][0];
        ret->x3 = fp->centers->mat[0][0];
        ret->y1 = fp->centers->mat[1][1];
        ret->y2 = fp->centers->mat[2][1];
        ret->y3 = fp->centers->mat[0][1];
    }
    else 
    {
        indexA = 0;
        ret->indexA = indexA;
        ret->dist = dist2_3;
        ret->x1 = fp->centers->mat[0][0];
        ret->x2 = fp->centers->mat[1][0];
        ret->x3 = fp->centers->mat[2][0];
        ret->y1 = fp->centers->mat[0][1];
        ret->y2 = fp->centers->mat[1][1];
        ret->y3 = fp->centers->mat[2][1];
    }
   
     
    return ret;    
}

/*
double *getBandC (struct FPat *fp, int A, double distB_C )
{
    
}*/
