/*
**  Validate QrCode Existence from Finder Patterns
**  file : FPValid.c
**  desrciption : Validates whether or not if there is a QrCode, and wether
**  it is workable, retrieves afterwards FP A, B, C
*/

# include "FPValid.h"

static inline
int Find_QrCodes(void)
{
    return -1;
}

int QrCode_found(struct FPat *fp)
{
    if(fp->centers->size < 3)
        return -1;

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
    
    int indexA = 0;

    if(dist1_2 > dist1_3 && dist1_2 > dist2_3)
        indexA = 2;
    else if(dist1_3 > dist1_2 && dist1_3 > dist2_3)
        indexA = 1;
    
    return indexA;    
}

double *get_Dcoord (struct FPat *fp, int A) // for projection transform later
{
    double *ret = malloc(sizeof(double));
    double *Aco = fp->centers->mat[A];
    double *Bco;
    double *Cco;
    if(A == 0)
    {
        Bco = fp->centers->mat[1];
        Cco = fp->centers->mat[2]; 
    }
    else if(A == 1)
    { 
        Bco = fp->centers->mat[0];
        Cco = fp->centers->mat[2]; 
    }
    else
    { 
        Bco = fp->centers->mat[0];
        Cco = fp->centers->mat[1];
    }
    
    double dBx = (Bco[0] - Aco[0]);
    double dCx = abs(Cco[0] - Aco[0]);
    double dBy = abs(Bco[1] - Aco[1]);
    double dCy = abs(Cco[1] - Aco[1]);
    
    double Bx = Bco[0];
    double Cx = Cco[0];
    double By = Bco[1];
    double Cy = Cco[1];
    
    double Ba = dBy / dBx;
    double Ca = dCy / dCx;
    
    if(dCx == 0)
        Ca = 0;
    if(dBx == 0)
        Ba = 0;
    double Bb = By - Ba * Bx;
    double Cb = Cy - Ca * Cx;
    
    warn("%lf %lf %lf %lf", Aco[0], Bco[0], dCx, dCy);
    double Dx = (Cb - Bb)/(Ba - Ca);
    double Dy = Ba*Bx + Bb;
    
    
    ret[0] = Dx;
    ret[1] = Dy;

    return ret;
    
}
