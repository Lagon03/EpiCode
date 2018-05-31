/*
**  Alignment pattern finder and image sampling
**  file : apsamp.c
**  description : Locates Alignment Patterns and samples image into Qr Matrix
*/

# include "apsamp.h"
# include "aploc.h"

/*----Static----*/
static inline
void print_AP(double ***mat, int size)
{
    printf("\n");
    for(int i = 0; i < size; i++)
    {
        printf("[ ");
        for(int j = 0; j < size; j++)
        {
            printf("{ %lf | %lf } ", mat[j][i][0], mat[j][i][1]);                
        }
        printf("]\n");
    }
    printf("\n");
}

static inline
void draw_AP(double ***mat, int size, SDL_Surface *img)
{
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            Draw_point(img, mat[j][i][0], mat[j][i][1]);                
        }
    }
}

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
void print_matd(double **mat, int sizex, int sizey)
{
    printf("\n");
    for(int i = 0; i < sizex; i++)
    {
        printf("[ ");
        for(int j = 0; j < sizey; j++)
        {
            printf("%lf ", mat[i][j]);
        }
        printf("]\n");
    }
    printf("\n");

}

static inline                                                                   
int get_BW(SDL_Surface *img, int x, int y) //returns 0 if black, 1 if white     
{                                                                               
     Uint32 pixel = getpixel(img, x, y);                                         
     Uint8 r, g, b;                                                              
     SDL_GetRGB(pixel, img->format, &r, &g, &b);
     //printf("color at %d %d: %d \n", x, y, g);
     //putpixel(img, x, y, SDL_MapRGB(img->format, 255, 0, 0)); 
     if(g == 0)                                                                  
        return 0;                                                               
     else                                                                        
        return 1; 
}

static inline
char get_CL(SDL_Surface *img, int x, int y)
{
    Uint32 pixel = getpixel(img, x, y);
    Uint8 r, g, b;
    SDL_GetRGB(pixel, img->format, &r, &g, &b);
    if(b == 255 && g == 255 && r == 255)
    {
        return '0';
    }
    else if(g == 0 && r == 0 && b == 0)
    {
        return '1';
    }
    else if( g >= b && g >= r)
    {
        return 'g';
    }
    else if( r >= b && r >= g)
    {
        return 'r';
    }
    else if( b >= r && b >= g)
    {
        return 'b';
    }
    else
        return '0';
}

int checkRatio(int *state, double msize)
{
    int totsize = 0;
    for(int i = 0; i < 3; i++)
        totsize += state[i];
    
    if (totsize < 3)
        return 0;
   
    //printf("state %d %d %d | %lf\n", state[0], state[1], state[2], msize);

    double max_var = msize/1.5;
    if((abs(msize - state[0]) <= max_var) &&
       (abs(msize - state[1]) <= max_var) &&
       (abs(msize - state[2]) <= max_var))
        return totsize;
    else
        return 0;
}

static inline
void SampleGridKernelE(char **mat,
double XA, double YA, double XB, double YB, double XC, double YC, double XD, double YD, 
double CPxp, double CPyp, double CPx, double CPy,
int AP, int pX, int pY, SDL_Surface *co)
{
    int X;
    int Y;
    double aX;
    double aY;
    double bX;
    double bY;
    double cX;
    double cY;
    double dX;
    double dY;
    double weightX;
    double weightY; 
    for(int y = 6; y < AP + 6; y++)
    {
        for(int x = 6; x < AP + 6; x++)
        {
            weightX = ((double)AP - (x - 6))/(double)AP;
            weightY = ((double)AP - (y - 6))/(double)AP;
            aX = (XA + CPxp * (x - 6)) * weightY * weightX;
            bX = (XB - CPxp * (AP - (x - 6))) * weightY * (1-weightX);
            cX = (XC + CPx * (x - 6)) * (1 - weightY) * weightX;
            dX = (XD - CPx * (AP - (x - 6))) * (1 - weightY) * (1 - weightX);
            X = round(aX + bX + cX + dX);
            aY = (YA + CPyp * (y - 6)) * weightY * weightX;
            bY = (YB + CPy * (y - 6)) * weightY * (1-weightX);
            cY = (YC - CPyp * (AP - (y - 6))) * (1 - weightY) * weightX;
            dY = (YD - CPy * (AP - (y - 6))) * (1 - weightY) * (1 - weightX);
            Y = round(aY + bY + cY + dY);
            mat[y + AP * pY][x + AP * pX] = get_CL(co, X, Y);
            //putpixel(img, X, Y, SDL_MapRGB(img->format, 255, 0, 0));
        }
    }
    //print_mat(mat, AP + 13);
    //display_image(img);
}

static inline
void SampleGridBorderLeftE(char **mat,
double XA, double YA, double XC, double YC,
double CPyp, double CPAx, double CPCx, int AP, int pY, SDL_Surface *co)
{
    int X;
    int Y;
    double aX;
    double aY;
    double cX;
    double cY;
    double weightY; 
    for(int y = 7; y < AP + 7; y++)
    {
        for(int x = 5; x >= 0; x--)
        {
            weightY = ((double)AP - (y - 6))/(double)AP;
            aX = (XA + CPAx * (x - 6)) * weightY;;
            cX = (XC + CPCx * (x - 6)) * (1 - weightY);
            X = round(aX + cX);
            aY = (YA + CPyp * (y - 6)) * weightY;
            cY = (YC - CPyp * (AP - (y - 6))) * (1 - weightY);
            Y = round(aY + cY);
            mat[y + AP * pY][x] = get_CL(co, X, Y);
            //putpixel(img, X, Y, SDL_MapRGB(img->format, 255, 0, 0));
        }
    }
    //print_mat(mat, AP + 13);
    //display_image(img);
}

static inline
void SampleGridBorderTopE(char **mat,
double XA, double YA, double XB, double YB,
double CPxp, double CPAy, double CPBy, int AP, int pX, SDL_Surface *co)
{
    int X;
    int Y;
    double aX;
    double aY;
    double bX;
    double bY;
    double weightX;
    for(int y = 5; y >= 0; y--)
    {
        for(int x = 7; x < AP + 7; x++)
        {
            weightX = ((double)AP - (x - 6))/(double)AP;
            aX = (XA + CPxp * (x - 6)) * weightX;
            bX = (XB - CPxp * (AP - (x - 6))) * (1-weightX);
            X = round(aX + bX);
            aY = (YA + CPAy * (y - 6)) * weightX;
            bY = (YB + CPBy * (y - 6)) * (1-weightX);
            Y = round(aY + bY);
            mat[y][x + AP * pX] = get_CL(co, X, Y);
            //putpixel(img, X, Y, SDL_MapRGB(img->format, 255, 0, 0));
        }
    }
    //print_mat(mat, AP + 13);
    //display_image(img);
}

static inline
void SampleGridBorderBottomE(char **mat,
double XC, double YC, double XD, double YD,
double CPx, double CPCy, double CPy, int AP, int pX, int SNB, SDL_Surface *co)
{
    int X;
    int Y;
    double cX;
    double cY;
    double dX;
    double dY;
    double weightX;
    for(int y = AP + 6; y < AP + 13; y++)
    {
        for(int x = 7; x < AP + 7; x++)
        {
            //warn("%d %d", x, y);
            weightX = ((double)AP - (x - 6))/(double)AP;
            cX = (XC + CPx * (x - 6)) * weightX;
            dX = (XD - CPx * (AP - (x - 6))) * (1-weightX);
            X = round(cX + dX);
            cY = (YC - CPCy * (AP - (y - 6))) * weightX;
            dY = (YD - CPy * (AP - (y - 6))) * (1-weightX);
            Y = round(cY + dY);
            //warn("%d %d", X, Y);
            mat[y + AP * SNB][x + AP * pX] = get_CL(co, X, Y);
            //putpixel(img, X, Y, SDL_MapRGB(img->format, 255, 0, 0));
            //display_image(img);
        }
    }
    //print_mat(mat, AP + 13);
    //display_image(img);
}

static inline
void SampleGridBorderRightE(char **mat,
double XB, double YB, double XD, double YD,
double CPx, double CPBx, double CPy, int AP, int pY, int SNB, SDL_Surface *co)
{
    int X;
    int Y;
    double bX;
    double bY;
    double dX;
    double dY;
    double weightY;
    for(int y = 7; y < AP + 7; y++)
    {
        for(int x = AP + 6; x < AP + 13; x++)
        {
            //warn("%d %d", x, y);
            weightY = ((double)AP - (y - 6))/(double)AP;
            bX = (XB - CPBx * (AP - (x - 6))) * weightY;
            dX = (XD - CPx * (AP - (x - 6))) * (1-weightY);
            X = round(bX + dX);
            bY = (YB + CPy * (y - 6)) * weightY;
            dY = (YD - CPy * (AP - (y - 6))) * (1-weightY);
            Y = round(bY + dY);
            //warn("%d %d", X, Y);
            mat[y + AP * pY][x + AP * SNB] = get_CL(co, X, Y);
            //putpixel(img, X, Y, SDL_MapRGB(img->format, 255, 0, 0));
            //display_image(img);
        }
    }
    //print_mat(mat, AP + 13);
    //display_image(img);
}

static inline
void SampleGridCornerE(char **mat,
double XD, double YD,
double CPx, double CPy, int AP, int SNB, SDL_Surface *co)
{
    int X;
    int Y;
    for(int y = AP + 6; y < AP + 13; y++)
    {
        for(int x = AP + 6; x < AP + 13; x++)
        {
            //warn("%d %d", x, y);
            X = round(XD - CPx * (AP - (x - 6)));
            Y = round(YD - CPy * (AP - (y - 6)));
            //warn("%d %d", X, Y);
            mat[y + AP * SNB][x + AP * SNB] = get_CL(co, X, Y);
            //putpixel(img, X, Y, SDL_MapRGB(img->format, 255, 0, 0));
            //display_image(img);
        }
    }
    //print_mat(mat, AP + 13);
    //display_image(img);
}

static inline
void SampleGridKernel(SDL_Surface *img, char **mat,
double XA, double YA, double XB, double YB, double XC, double YC, double XD, double YD, 
double CPxp, double CPyp, double CPx, double CPy,
int AP, int pX, int pY)
{
    int X;
    int Y;
    double aX;
    double aY;
    double bX;
    double bY;
    double cX;
    double cY;
    double dX;
    double dY;
    double weightX;
    double weightY; 
    for(int y = 6; y < AP + 6; y++)
    {
        for(int x = 6; x < AP + 6; x++)
        {
            weightX = ((double)AP - (x - 6))/(double)AP;
            weightY = ((double)AP - (y - 6))/(double)AP;
            aX = (XA + CPxp * (x - 6)) * weightY * weightX;
            bX = (XB - CPxp * (AP - (x - 6))) * weightY * (1-weightX);
            cX = (XC + CPx * (x - 6)) * (1 - weightY) * weightX;
            dX = (XD - CPx * (AP - (x - 6))) * (1 - weightY) * (1 - weightX);
            X = round(aX + bX + cX + dX);
            aY = (YA + CPyp * (y - 6)) * weightY * weightX;
            bY = (YB + CPy * (y - 6)) * weightY * (1-weightX);
            cY = (YC - CPyp * (AP - (y - 6))) * (1 - weightY) * weightX;
            dY = (YD - CPy * (AP - (y - 6))) * (1 - weightY) * (1 - weightX);
            Y = round(aY + bY + cY + dY);
            if(get_BW(img, X, Y) == 0)
            {
                mat[y + AP * pY][x + AP * pX] = '1';
            }
            else
            {
                mat[y + AP * pY][x + AP * pX] = '0';
            }
            //putpixel(img, X, Y, SDL_MapRGB(img->format, 255, 0, 0));
        }
    }
    //print_mat(mat, AP + 13);
    //display_image(img);
}

static inline
void SampleGridBorderLeft(SDL_Surface *img, char **mat,
double XA, double YA, double XC, double YC,
double CPyp, double CPAx, double CPCx, int AP, int pY)
{
    int X;
    int Y;
    double aX;
    double aY;
    double cX;
    double cY;
    double weightY; 
    for(int y = 7; y < AP + 7; y++)
    {
        for(int x = 5; x >= 0; x--)
        {
            weightY = ((double)AP - (y - 6))/(double)AP;
            aX = (XA + CPAx * (x - 6)) * weightY;;
            cX = (XC + CPCx * (x - 6)) * (1 - weightY);
            X = round(aX + cX);
            aY = (YA + CPyp * (y - 6)) * weightY;
            cY = (YC - CPyp * (AP - (y - 6))) * (1 - weightY);
            Y = round(aY + cY);
            if(get_BW(img, X, Y) == 0)
            {
                mat[y + AP * pY][x] = '1';
            }
            else
            {
                mat[y + AP * pY][x] = '0';
            }
            //putpixel(img, X, Y, SDL_MapRGB(img->format, 255, 0, 0));
        }
    }
    //print_mat(mat, AP + 13);
    //display_image(img);
}

static inline
void SampleGridBorderTop(SDL_Surface *img, char **mat,
double XA, double YA, double XB, double YB,
double CPxp, double CPAy, double CPBy, int AP, int pX)
{
    int X;
    int Y;
    double aX;
    double aY;
    double bX;
    double bY;
    double weightX;
    for(int y = 5; y >= 0; y--)
    {
        for(int x = 7; x < AP + 7; x++)
        {
            weightX = ((double)AP - (x - 6))/(double)AP;
            aX = (XA + CPxp * (x - 6)) * weightX;
            bX = (XB - CPxp * (AP - (x - 6))) * (1-weightX);
            X = round(aX + bX);
            aY = (YA + CPAy * (y - 6)) * weightX;
            bY = (YB + CPBy * (y - 6)) * (1-weightX);
            Y = round(aY + bY);
            if(get_BW(img, X, Y) == 0)
            {
                mat[y][x + AP * pX] = '1';
            }
            else
            {
                mat[y][x + AP * pX] = '0';
            }
            //putpixel(img, X, Y, SDL_MapRGB(img->format, 255, 0, 0));
        }
    }
    //print_mat(mat, AP + 13);
    //display_image(img);
}

static inline
void SampleGridBorderBottom(SDL_Surface *img, char **mat,
double XC, double YC, double XD, double YD,
double CPx, double CPCy, double CPy, int AP, int pX, int SNB)
{
    int X;
    int Y;
    double cX;
    double cY;
    double dX;
    double dY;
    double weightX;
    for(int y = AP + 6; y < AP + 13; y++)
    {
        for(int x = 7; x < AP + 7; x++)
        {
            //warn("%d %d", x, y);
            weightX = ((double)AP - (x - 6))/(double)AP;
            cX = (XC + CPx * (x - 6)) * weightX;
            dX = (XD - CPx * (AP - (x - 6))) * (1-weightX);
            X = round(cX + dX);
            cY = (YC - CPCy * (AP - (y - 6))) * weightX;
            dY = (YD - CPy * (AP - (y - 6))) * (1-weightX);
            Y = round(cY + dY);
            //warn("%d %d", X, Y);
            if(get_BW(img, X, Y) == 0)
            {
                mat[y + SNB * AP][x + AP * pX] = '1';
            }
            else
            {
                mat[y + SNB * AP][x + AP * pX] = '0';
            }
            //putpixel(img, X, Y, SDL_MapRGB(img->format, 255, 0, 0));
            //display_image(img);
        }
    }
    //print_mat(mat, AP + 13);
    //display_image(img);
}

static inline
void SampleGridBorderRight(SDL_Surface *img, char **mat,
double XB, double YB, double XD, double YD,
double CPx, double CPBx, double CPy, int AP, int pY, int SNB)
{
    int X;
    int Y;
    double bX;
    double bY;
    double dX;
    double dY;
    double weightY;
    for(int y = 7; y < AP + 7; y++)
    {
        for(int x = AP + 6; x < AP + 13; x++)
        {
            //warn("%d %d", x, y);
            weightY = ((double)AP - (y - 6))/(double)AP;
            bX = (XB - CPBx * (AP - (x - 6))) * weightY;
            dX = (XD - CPx * (AP - (x - 6))) * (1-weightY);
            X = round(bX + dX);
            bY = (YB + CPy * (y - 6)) * weightY;
            dY = (YD - CPy * (AP - (y - 6))) * (1-weightY);
            Y = round(bY + dY);
            //warn("%d %d", X, Y);
            if(get_BW(img, X, Y) == 0)
            {
                mat[y + AP * pY ][x + SNB * AP] = '1';
            }
            else
            {
                mat[y + AP * pY][x + SNB * AP] = '0';
            }
            //putpixel(img, X, Y, SDL_MapRGB(img->format, 255, 0, 0));
            //display_image(img);
        }
    }
    //print_mat(mat, AP + 13);
    //display_image(img);
}

static inline
void SampleGridCorner(SDL_Surface *img, char **mat,
double XD, double YD,
double CPx, double CPy, int AP, int SNB)
{
    int X;
    int Y;
    for(int y = AP + 6; y < AP + 13; y++)
    {
        for(int x = AP + 6; x < AP + 13; x++)
        {
            //warn("%d %d", x, y);
            X = round(XD - CPx * (AP - (x - 6)));
            Y = round(YD - CPy * (AP - (y - 6)));
            //warn("%d %d", X, Y);
            if(get_BW(img, X, Y) == 0)
            {
                mat[y + SNB * AP][x + SNB * AP] = '1';
            }
            else
            {
                mat[y + SNB * AP][x + SNB * AP] = '0';
            }
            //putpixel(img, X, Y, SDL_MapRGB(img->format, 255, 0, 0));
            //display_image(img);
        }
    }
    //print_mat(mat, AP + 13);
    //display_image(img);
}

/*-----Main-----*/

void SampleCodeV2_6E(struct GeoImg *qrimg, struct QrCode *qr, int WA, int WB,
int WC, int HA, int HB, int HC, SDL_Surface *co)
{
    int size = qr->version * 4 + 17;
    char **mat = malloc(sizeof(char*) * size);
    for(int i = 0; i < size; i++)
    {
        mat[i] = malloc(sizeof(char) * size);
        for(int j = 0; j < size; j++) 
            mat[i][j] = '0';
    }
    
    int distAP = Ap_coord[qr->version - 1][2] - Ap_coord[qr->version - 1][1]; 
    int AP[2] = {Ap_coord[qr->version - 1][2], Ap_coord[qr->version - 1][2]}; 
    double CPAx = (double)WA / 7;
    double CPBx = (double)WB / 7;
    double CPCx = (double)WC / 7;
    double CPAy = (double)HA / 7;
    double CPBy = (double)HB / 7;
    double CPCy = (double)HC / 7;
    double XA = qrimg->coordA[0] + 3 * CPAx;
    double YA = qrimg->coordA[1] + 3 * CPAy;
    double XB = qrimg->coordB[0] - 3 * CPBx;
    double YB = qrimg->coordB[1] + 3 * CPBy;
    double XC = qrimg->coordC[0] + 3 * CPCx;
    double YC = qrimg->coordC[1] - 3 * CPCy;
    double Px = ((XA + CPAx * (AP[0] - 6)) + XB + (XC + CPCx * (AP[0] - 6))) / 3;
    double Py = ((YA + CPAy * (AP[1] - 6)) + (YB + CPCy * (AP[1] - 6)) + YC) / 3;
    Draw_point(qrimg->img, XA, YA);
    Draw_point(qrimg->img, XB, YB);
    Draw_point(qrimg->img, XC, YC);
    ScanAP(qrimg->img, &Px, &Py); 
    //AP SCANNED
    
    double Lxp = abs(XA - XB);
    double Lyp = abs(YA - YC);
    double Lx = abs(XC - Px);
    double Ly = abs(YB - Py);
    double CPxp = Lxp / distAP;
    double CPyp = Lyp / distAP;
    double CPx = Lx / distAP;
    double CPy = Ly / distAP;
    //SampleGridKernel A->P
    SampleGridKernelE(mat, XA, YA, XB, YB, XC, YC, Px, Py, CPxp,
CPyp, CPx, CPy, distAP, 0, 0, co);
    //SampleGridBorders
    SampleGridBorderLeftE(mat, XA, YA, XC, YC, CPyp, CPAx, CPCx,
distAP, 0, co);
    SampleGridBorderTopE(mat, XA, YA, XB, YB, CPxp, CPAy, CPBy,
distAP, 0, co);
        //we should try to find CPxend and CPyend for better Sampling for
        //bot/right and corner
    SampleGridBorderBottomE(mat, XC, YC, Px, Py, CPx, CPCy, CPy,
distAP, 0, 0 ,co);
    SampleGridBorderRightE(mat, XB, YB, Px, Py, CPx, CPBx, CPy,
distAP, 0, 0, co);
    //Sample Grid Corner
    SampleGridCornerE(mat, Px, Py, CPx, CPy, distAP, 0, co);
    qr->mat = mat;
    Draw_point(qrimg->img, Px, Py);
}

void SampleCodeV7_40E(struct GeoImg *qrimg, struct QrCode *qr, int WA, int WB,
int WC, int HA, int HB, int HC, SDL_Surface *co)
{ 
    int size = qr->version * 4 + 17;
    char **mat = malloc(sizeof(char*) * size);
    for(int i = 0; i < size; i++)
    {
        mat[i] = malloc(sizeof(char) * size);
        for(int j = 0; j < size; j++) 
            mat[i][j] = '0';
    }
    
    int distAP = Ap_coord[qr->version - 1][2] - Ap_coord[qr->version - 1][1];
    int SampleKNb = (int)sqrt(Ap_coord[qr->version - 1][0] + 3);
    double CPAx = (double)WA / 7;
    double CPBx = (double)WB / 7;
    double CPCx = (double)WC / 7;
    double CPAy = (double)HA / 7;
    double CPBy = (double)HB / 7;
    double CPCy = (double)HC / 7;

    double XA = qrimg->coordA[0] + 3 * CPAx;
    double YA = qrimg->coordA[1] + 3 * CPAy;
    double XB = qrimg->coordB[0] - 3 * CPBx;
    double YB = qrimg->coordB[1] + 3 * CPBy;
    double XC = qrimg->coordC[0] + 3 * CPCx;
    double YC = qrimg->coordC[1] - 3 * CPCy;
    double ***APStack = malloc(sizeof(double) * SampleKNb);
    
    for(int i = 0; i < SampleKNb; i++)
    {
        APStack[i] = malloc(sizeof(double*) * SampleKNb);
        for(int j = 0; j < SampleKNb; j++)
        {
            APStack[i][j] = calloc(2, sizeof(double));
        }
    }
    
    double **CPxs = malloc(sizeof(double*) * (SampleKNb));
    for(int i = 0; i < SampleKNb; i++)
    {
        CPxs[i] = calloc(SampleKNb, sizeof(double)); 
    }
    
    double **CPys = malloc(sizeof(double*) * (SampleKNb - 1));
    for(int i = 0; i < SampleKNb - 1; i++)
    {
        CPys[i] = calloc(SampleKNb, sizeof(double)); 
    }
    APStack[0][0][0] = XA;
    APStack[0][0][1] = YA;
    APStack[SampleKNb-1][0][0] = XB;
    APStack[SampleKNb-1][0][1] = YB;
    APStack[0][SampleKNb-1][0] = XC;
    APStack[0][SampleKNb-1][1] = YC;
    double CPxp = CPAx;
    double CPy;
    double CPx;
    double CPyp = CPAy;
    double Px1, Px2, Px3, Py1, Py2, Py3;
    double Lxp, Lyp, Lx, Ly;
    int y = 0;
    int x = 0;
    while(y < SampleKNb - 1)
    {
        // We Have P0 -> finding P2
        if(APStack[x][y + 1][0] == 0)
        {
            Px2 = APStack[x][y][0];
            Py2 = APStack[x][y][1] + CPyp * distAP;
            ScanAP(qrimg->img, &Px2, &Py2);
            APStack[x][y + 1][0] = Px2;
            APStack[x][y + 1][1] = Py2;
        }
        while(x < SampleKNb - 1)
        {
            if(APStack[x + 1][y][0] == 0)
            {
                //We Have P0 -> finding P1
                Px1 = APStack[x][y][0] + CPxp * distAP;
                Py1 = APStack[x][y][1];
                //assigning Alignement Patterns
                ScanAP(qrimg->img, &Px1, &Py1);
                APStack[x + 1][y][0] = Px1;
                APStack[x + 1][y][1] = Py1;
            }
            // -> finding P3
            Px3 = APStack[x + 1][y][0];
            Py3 = APStack[x][y + 1][1];
            //assignimg P3 to AP
            ScanAP(qrimg->img, &Px3, &Py3);
            APStack[x + 1][y + 1][0] = Px3;
            APStack[x + 1][y + 1][1] = Py3;
            //Updating module size
            Lxp = abs(APStack[x][y][0] - APStack[x + 1][y][0]);
            Lyp = abs(APStack[x][y][1] - APStack[x][y + 1][1]);
            Lx = abs(APStack[x][y + 1][0] - APStack[x + 1][y + 1][0]);
            Ly = abs(APStack[x + 1][y][1] - APStack[x + 1][y + 1][1]);
            //warn("Lxp, Lyp, Lx, Ly : %lf %lf %lf %lf", Lxp, Lyp, Lx, Ly); 
            CPxp = Lxp / distAP;
            CPyp = Lyp / distAP;
            CPx = Lx / distAP;
            CPy = Ly / distAP;
            CPxs[y][x] = CPxp;
            CPxs[y + 1][x] = CPx;
            CPys[y][x] = CPy;
            CPys[y][x + 1] = CPy;
            SampleGridKernelE(mat, APStack[x][y][0],
            APStack[x][y][1], APStack[x+1][y][0], APStack[x+1][y][1],
            APStack[x][y+1][0], APStack[x][y+1][1], APStack[x + 1][y + 1][0],
            APStack[x + 1][y + 1][1], CPxs[y][x], CPys[y][x], CPxs[y + 1][x], 
            CPys[y][x + 1], distAP, x, y,co);
            x++;
        }
        CPxp = CPxs[y][0];
        CPyp = CPys[y][0]; 
        x = 0;
        y++;
    }
    
    //Left Border
    for(int y = 0; y < SampleKNb - 1; y++)
    {
        if(y == 0)
        {
            SampleGridBorderLeftE(mat,
            APStack[0][0][0], APStack[0][0][1], APStack[0][1][0],
            APStack[0][1][1], CPys[0][0], CPAx, CPxs[1][0], distAP, 0,co);
        }
        else if(y == SampleKNb - 2)
        { 
            SampleGridBorderLeftE(mat,
            APStack[0][y][0], APStack[0][y][1], APStack[0][y + 1][0],
            APStack[0][y + 1][1], CPys[y][0], CPxs[y][0], CPCx, distAP, y,co);   
        }
        else
        {
            SampleGridBorderLeftE(mat,
            APStack[0][y][0], APStack[0][y][1], APStack[0][y + 1][0],
            APStack[0][y + 1][1], CPys[y][0], CPxs[y][0], CPxs[y + 1][0],
distAP, y,co);
        }
    }
    
    //Top Border
    for(int x = 0; x < SampleKNb - 1; x++)
    {
        if(x == 0)
        {
            SampleGridBorderTopE(mat,
            APStack[0][0][0], APStack[0][0][1], APStack[1][0][0],
            APStack[1][0][1], CPxs[0][0], CPAy, CPys[0][1], distAP, 0,co);
        }
        else if(x == SampleKNb - 2)
        { 
            SampleGridBorderTopE(mat,
            APStack[x][0][0], APStack[x][0][1], APStack[x + 1][0][0],
            APStack[x + 1][0][1], CPxs[0][x], CPys[0][x], CPBy, distAP, x,co);   
        }
        else
        {
            SampleGridBorderTopE(mat,
            APStack[x][0][0], APStack[x][0][1], APStack[x + 1][0][0],
            APStack[x + 1][0][1], CPxs[0][x], CPys[0][x], CPys[0][x + 1],
distAP, x,co);
        }
    }
    
    //Bottom Border
    int yend = SampleKNb - 1;
    for(int x = 0; x < SampleKNb - 1; x++)
    {
        if(x == 0)
        {
            SampleGridBorderBottomE(mat,
            APStack[0][yend][0], APStack[0][yend][1], APStack[1][yend][0],
            APStack[1][yend][1], CPxs[yend][0], CPCy, CPys[yend - 1][1], distAP,
0, yend - 1,co);
        }
        else
        {
            SampleGridBorderBottomE(mat,
            APStack[x][yend][0], APStack[x][yend][1], APStack[x + 1][yend][0],
            APStack[x + 1][yend][1], CPxs[yend][x], CPys[yend - 1][x], CPys[yend
- 1][x + 1], distAP, x, yend - 1, co);
        }
    }
    
    //Right Border
    int xend = SampleKNb - 1;
    for(int y = 0; y < SampleKNb - 1; y++)
    {
        if(y == 0)
        {
            SampleGridBorderRightE(mat,
            APStack[xend][0][0], APStack[xend][0][1], APStack[xend][1][0],
            APStack[xend][1][1], CPxs[1][xend - 1], CPBx, CPys[0][xend], distAP,
0, xend - 1,co);
        }
        else
        {
            SampleGridBorderRightE(mat,
            APStack[xend][y][0], APStack[xend][y][1], APStack[xend][y + 1][0],
            APStack[xend][y + 1][1], CPxs[y + 1][xend - 1], CPxs[y][xend - 1],
CPys[y][xend], distAP, y, xend - 1, co);
        }
    }
    
    SampleGridCornerE(mat, APStack[SampleKNb - 1][SampleKNb - 1][0],
    APStack[SampleKNb - 1][SampleKNb - 1][1], CPxs[yend][xend - 1], 
    CPys[yend -1][xend], distAP, xend - 1, co);
    
    //print_mat(mat, size);
    draw_AP(APStack, SampleKNb, qrimg->img);
    //display_image(qrimg->img);
    //print_AP(APStack, SampleKNb);
    qr->mat = mat; 
}

void SampleCodeV2_6(struct GeoImg *qrimg, struct QrCode *qr, int WA, int WB, int WC, int HA, int HB, int HC)
{
    int size = qr->version * 4 + 17;
    char **mat = malloc(sizeof(char*) * size);
    for(int i = 0; i < size; i++)
    {
        mat[i] = malloc(sizeof(char) * size);
        for(int j = 0; j < size; j++) 
            mat[i][j] = '0';
    }
    
    int distAP = Ap_coord[qr->version - 1][2] - Ap_coord[qr->version - 1][1]; 
    int AP[2] = {Ap_coord[qr->version - 1][2], Ap_coord[qr->version - 1][2]}; 
    double CPAx = (double)WA / 7;
    double CPBx = (double)WB / 7;
    double CPCx = (double)WC / 7;
    double CPAy = (double)HA / 7;
    double CPBy = (double)HB / 7;
    double CPCy = (double)HC / 7;
    double XA = qrimg->coordA[0] + 3 * CPAx;
    double YA = qrimg->coordA[1] + 3 * CPAy;
    double XB = qrimg->coordB[0] - 3 * CPBx;
    double YB = qrimg->coordB[1] + 3 * CPBy;
    double XC = qrimg->coordC[0] + 3 * CPCx;
    double YC = qrimg->coordC[1] - 3 * CPCy;
    double Px = ((XA + CPAx * (AP[0] - 6)) + XB + (XC + CPCx * (AP[0] - 6))) / 3;
    double Py = ((YA + CPAy * (AP[1] - 6)) + (YB + CPCy * (AP[1] - 6)) + YC) / 3;
    Draw_point(qrimg->img, XA, YA);
    Draw_point(qrimg->img, XB, YB);
    Draw_point(qrimg->img, XC, YC);
    ScanAP(qrimg->img, &Px, &Py); 
    //AP SCANNED
    
    double Lxp = abs(XA - XB);
    double Lyp = abs(YA - YC);
    double Lx = abs(XC - Px);
    double Ly = abs(YB - Py);
    double CPxp = Lxp / distAP;
    double CPyp = Lyp / distAP;
    double CPx = Lx / distAP;
    double CPy = Ly / distAP;
    //SampleGridKernel A->P
    SampleGridKernel(qrimg->img, mat, XA, YA, XB, YB, XC, YC, Px, Py, CPxp, CPyp, CPx, CPy, distAP, 0, 0);
    //SampleGridBorders
    SampleGridBorderLeft(qrimg->img, mat, XA, YA, XC, YC, CPyp, CPAx, CPCx, distAP, 0);
    SampleGridBorderTop(qrimg->img, mat, XA, YA, XB, YB, CPxp, CPAy, CPBy, distAP, 0);
        //we should try to find CPxend and CPyend for better Sampling for
        //bot/right and corner
    SampleGridBorderBottom(qrimg->img, mat, XC, YC, Px, Py, CPx, CPCy, CPy, distAP, 0, 0);
    SampleGridBorderRight(qrimg->img, mat, XB, YB, Px, Py, CPx, CPBx, CPy, distAP, 0, 0);
    //Sample Grid Corner
    SampleGridCorner(qrimg->img, mat, Px, Py, CPx, CPy, distAP, 0);
    qr->mat = mat;
    Draw_point(qrimg->img, Px, Py);
}

void SampleCodeV7_40(struct GeoImg *qrimg, struct QrCode *qr, int WA, int WB, int WC, int HA, int HB, int HC)
{ 
    int size = qr->version * 4 + 17;
    char **mat = malloc(sizeof(char*) * size);
    for(int i = 0; i < size; i++)
    {
        mat[i] = malloc(sizeof(char) * size);
        for(int j = 0; j < size; j++) 
            mat[i][j] = '0';
    }
    
    int distAP = Ap_coord[qr->version - 1][2] - Ap_coord[qr->version - 1][1];
    int SampleKNb = (int)sqrt(Ap_coord[qr->version - 1][0] + 3);
    double CPAx = (double)WA / 7;
    double CPBx = (double)WB / 7;
    double CPCx = (double)WC / 7;
    double CPAy = (double)HA / 7;
    double CPBy = (double)HB / 7;
    double CPCy = (double)HC / 7;

    double XA = qrimg->coordA[0] + 3 * CPAx;
    double YA = qrimg->coordA[1] + 3 * CPAy;
    double XB = qrimg->coordB[0] - 3 * CPBx;
    double YB = qrimg->coordB[1] + 3 * CPBy;
    double XC = qrimg->coordC[0] + 3 * CPCx;
    double YC = qrimg->coordC[1] - 3 * CPCy;
    double ***APStack = malloc(sizeof(double) * SampleKNb);
    
    for(int i = 0; i < SampleKNb; i++)
    {
        APStack[i] = malloc(sizeof(double*) * SampleKNb);
        for(int j = 0; j < SampleKNb; j++)
        {
            APStack[i][j] = calloc(2, sizeof(double));
        }
    }
    
    double **CPxs = malloc(sizeof(double*) * (SampleKNb));
    for(int i = 0; i < SampleKNb; i++)
    {
        CPxs[i] = calloc(SampleKNb, sizeof(double)); 
    }
    
    double **CPys = malloc(sizeof(double*) * (SampleKNb - 1));
    for(int i = 0; i < SampleKNb - 1; i++)
    {
        CPys[i] = calloc(SampleKNb, sizeof(double)); 
    }
    APStack[0][0][0] = XA;
    APStack[0][0][1] = YA;
    APStack[SampleKNb-1][0][0] = XB;
    APStack[SampleKNb-1][0][1] = YB;
    APStack[0][SampleKNb-1][0] = XC;
    APStack[0][SampleKNb-1][1] = YC;
    double CPxp = CPAx;
    double CPy;
    double CPx;
    double CPyp = CPAy;
    double Px1, Px2, Px3, Py1, Py2, Py3;
    double Lxp, Lyp, Lx, Ly;
    int y = 0;
    int x = 0;
    while(y < SampleKNb - 1)
    {
        // We Have P0 -> finding P2
        if(APStack[x][y + 1][0] == 0)
        {
            Px2 = APStack[x][y][0];
            Py2 = APStack[x][y][1] + CPyp * distAP;
            ScanAP(qrimg->img, &Px2, &Py2);
            APStack[x][y + 1][0] = Px2;
            APStack[x][y + 1][1] = Py2;
        }
        while(x < SampleKNb - 1)
        {
            if(APStack[x + 1][y][0] == 0)
            {
                //We Have P0 -> finding P1
                Px1 = APStack[x][y][0] + CPxp * distAP;
                Py1 = APStack[x][y][1];
                //assigning Alignement Patterns
                ScanAP(qrimg->img, &Px1, &Py1);
                APStack[x + 1][y][0] = Px1;
                APStack[x + 1][y][1] = Py1;
            }
            // -> finding P3
            Px3 = APStack[x + 1][y][0];
            Py3 = APStack[x][y + 1][1];
            //assignimg P3 to AP
            ScanAP(qrimg->img, &Px3, &Py3);
            APStack[x + 1][y + 1][0] = Px3;
            APStack[x + 1][y + 1][1] = Py3;
            //Updating module size
            Lxp = abs(APStack[x][y][0] - APStack[x + 1][y][0]);
            Lyp = abs(APStack[x][y][1] - APStack[x][y + 1][1]);
            Lx = abs(APStack[x][y + 1][0] - APStack[x + 1][y + 1][0]);
            Ly = abs(APStack[x + 1][y][1] - APStack[x + 1][y + 1][1]);
            //warn("Lxp, Lyp, Lx, Ly : %lf %lf %lf %lf", Lxp, Lyp, Lx, Ly); 
            CPxp = Lxp / distAP;
            CPyp = Lyp / distAP;
            CPx = Lx / distAP;
            CPy = Ly / distAP;
            CPxs[y][x] = CPxp;
            CPxs[y + 1][x] = CPx;
            CPys[y][x] = CPy;
            CPys[y][x + 1] = CPy;
            SampleGridKernel(qrimg->img, mat, APStack[x][y][0],
            APStack[x][y][1], APStack[x+1][y][0], APStack[x+1][y][1],
            APStack[x][y+1][0], APStack[x][y+1][1], APStack[x + 1][y + 1][0],
            APStack[x + 1][y + 1][1], CPxs[y][x], CPys[y][x], CPxs[y + 1][x], 
            CPys[y][x + 1], distAP, x, y);
            x++;
        }
        CPxp = CPxs[y][0];
        CPyp = CPys[y][0]; 
        x = 0;
        y++;
    }
    //Left Border
    for(int y = 0; y < SampleKNb - 1; y++)
    {
        if(y == 0)
        {
            SampleGridBorderLeft(qrimg->img, mat,
            APStack[0][0][0], APStack[0][0][1], APStack[0][1][0],
            APStack[0][1][1], CPys[0][0], CPAx, CPxs[1][0], distAP, 0);
        }
        else if(y == SampleKNb - 2)
        { 
            SampleGridBorderLeft(qrimg->img, mat,
            APStack[0][y][0], APStack[0][y][1], APStack[0][y + 1][0],
            APStack[0][y + 1][1], CPys[y][0], CPxs[y][0], CPCx, distAP, y);   
        }
        else
        {
            SampleGridBorderLeft(qrimg->img, mat,
            APStack[0][y][0], APStack[0][y][1], APStack[0][y + 1][0],
            APStack[0][y + 1][1], CPys[y][0], CPxs[y][0], CPxs[y + 1][0], distAP, y);
        }
    }
    
    //Top Border
    for(int x = 0; x < SampleKNb - 1; x++)
    {
        if(x == 0)
        {
            SampleGridBorderTop(qrimg->img, mat,
            APStack[0][0][0], APStack[0][0][1], APStack[1][0][0],
            APStack[1][0][1], CPxs[0][0], CPAy, CPys[0][1], distAP, 0);
        }
        else if(x == SampleKNb - 2)
        { 
            SampleGridBorderTop(qrimg->img, mat,
            APStack[x][0][0], APStack[x][0][1], APStack[x + 1][0][0],
            APStack[x + 1][0][1], CPxs[0][x], CPys[0][x], CPBy, distAP, x);   
        }
        else
        {
            SampleGridBorderTop(qrimg->img, mat,
            APStack[x][0][0], APStack[x][0][1], APStack[x + 1][0][0],
            APStack[x + 1][0][1], CPxs[0][x], CPys[0][x], CPys[0][x + 1], distAP, x);
        }
    }
    
    //Bottom Border
    int yend = SampleKNb - 1;
    for(int x = 0; x < SampleKNb - 1; x++)
    {
        if(x == 0)
        {
            SampleGridBorderBottom(qrimg->img, mat,
            APStack[0][yend][0], APStack[0][yend][1], APStack[1][yend][0],
            APStack[1][yend][1], CPxs[yend][0], CPCy, CPys[yend - 1][1], distAP, 0, yend - 1);
        }
        else
        {
            SampleGridBorderBottom(qrimg->img, mat,
            APStack[x][yend][0], APStack[x][yend][1], APStack[x + 1][yend][0],
            APStack[x + 1][yend][1], CPxs[yend][x], CPys[yend - 1][x], CPys[yend - 1][x + 1], distAP, x, yend - 1);
        }
    }
    
    //Right Border
    int xend = SampleKNb - 1;
    for(int y = 0; y < SampleKNb - 1; y++)
    {
        if(y == 0)
        {
            SampleGridBorderRight(qrimg->img, mat,
            APStack[xend][0][0], APStack[xend][0][1], APStack[xend][1][0],
            APStack[xend][1][1], CPxs[1][xend - 1], CPBx, CPys[0][xend], distAP, 0, xend - 1);
        }
        else
        {
            SampleGridBorderRight(qrimg->img, mat,
            APStack[xend][y][0], APStack[xend][y][1], APStack[xend][y + 1][0],
            APStack[xend][y + 1][1], CPxs[y + 1][xend - 1], CPxs[y][xend - 1], CPys[y][xend], distAP, y, xend - 1);
        }
    }
    
    SampleGridCorner(qrimg->img, mat, APStack[SampleKNb - 1][SampleKNb - 1][0],
    APStack[SampleKNb - 1][SampleKNb - 1][1], CPxs[yend][xend - 1], 
    CPys[yend -1][xend], distAP, xend - 1);
    //print_mat(mat, size);
    //draw_AP(APStack, SampleKNb, qrimg->img);
    //display_image(qrimg->img);
    //print_AP(APStack, SampleKNb);
    qr->mat = mat; 
}
