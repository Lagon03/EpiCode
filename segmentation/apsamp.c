/*
**  Alignment pattern finder and image sampling
**  file : apsamp.c
**  description : Locates Alignment Patterns and samples image into Qr Matrix
*/

# include "apsamp.h"

/*----Static----*/

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
void __MarkAlignment(char **mat, int i, int j)
{
    //warn("sub function");
    mat[i][j] = '1';
    mat[i - 1][j] = '0';
    mat[i - 1][j - 1] = '0';
    mat[i - 1][j - 2] = '1';
    mat[i - 1][j + 1] = '0';
    mat[i - 1][j + 2] = '1';
    mat[i - 2][j - 1] = '1';
    mat[i - 2][j - 2] = '1';
    mat[i - 2][j + 1] = '1';
    mat[i - 2][j + 2] = '1';
    mat[i - 2][j] = '1';
    mat[i + 1][j] = '0';
    mat[i + 1][j - 1] = '0';
    mat[i + 1][j - 2] = '1';
    mat[i + 1][j + 1] = '0';
    mat[i + 1][j + 2] = '1';
    mat[i + 2][j] = '1';
    mat[i + 2][j - 1] = '1';
    mat[i + 2][j - 2] = '1';
    mat[i + 2][j + 1] = '1';
    mat[i + 2][j + 2] = '1';
    mat[i][j - 1] = '0';
    mat[i][j - 2] = '1';
    mat[i][j + 1] = '0';
    mat[i][j + 2] = '1';
}

static inline
int *MarkOneAlignment(char **mat, int version)
{
    int size = 4 * version + 17;
    const int *ap = Ap_coord[version-1];
    int *AP = malloc(sizeof(int) * 2);  
    
    for(int i = 1; i < 8; i++)
    {
        for(int j = 1; j < 8; j++)
        { 
            //warn("in loop");
            if((ap[i] == 6 && ap[j] == 6) || 
               (ap[i] == 6 && ap[j] == size - 7) ||
               (ap[i] == size - 7 && ap[j] == 6 ) || 
                ap[i] == 0 || ap[j] == 0)
            {       
                    //warn("bad coord %d %d", ap[i], ap[j]);
                    continue;
            }
            
            //warn("good coord %d %d", ap[i], ap[j]);
            //__MarkAlignment(mat, ap[i], ap[j]);
            AP[0] = ap[i];
            AP[1] = ap[j];
        }
    }
    return AP;  
}

int checkRatio(int *state, double msize)
{
    int totsize = 0;
    for(int i = 0; i < 3; i++)
        totsize += state[i];
    
    if (totsize < 3)
        return 0;
   
    //printf("state %d %d %d | %lf\n", state[0], state[1], state[2], msize);

    double max_var = msize/2;
    if((abs(msize - state[0]) <= max_var) &&
       (abs(msize - state[1]) <= max_var) &&
       (abs(msize - state[2]) <= max_var))
        return totsize;
    else
        return 0;
}

static inline
double VerifyCenterH(SDL_Surface *img, int center_x, int center_y, double CPA)
{
    int w = img->w;
    int state[3] = {0};
    int cr = 0;
    int cl = 0;
    int x = center_x;
    
    while((x >= 0) && (get_BW(img, x, center_y) == 0))
    {
        cl++;
        state[1]++;
        x--;
    }
    if(x < 0)
        return -CPA;
    
    while((x >= 0) && get_BW(img, x, center_y) == 1)
    {
        cl++;
        state[0]++;
        x--;
    }
    if(x < 0)
        return -CPA;
    
    x = center_x + 1;
     
    while((x < w) && (get_BW(img, x, center_y) == 0))
    {
        cr++;
        state[1]++;
        x++;
    }
    if(x >= w)
        return -CPA;
    
    while((x < w) && get_BW(img, x, center_y) == 1)
    {
        cr++;
        state[2]++;
        x++;
    }
    if(x >= w)
        return -CPA;
    
    int totsize = checkRatio(state, CPA);
    
    if(totsize == 0)
        return -CPA;
    
    double dif = abs(cr - cl);
    
    if(cl >= cr)
        return -dif/2;
    else
        return +dif/2;
}

static inline
double VerifyCenterV(SDL_Surface *img, int center_x, int center_y, double CPA)
{
    int h = img->h;
    int state[3] = {0};
    int cu = 0;
    int cd = 0;
    int y = center_y;
    
    while((y >= 0) && (get_BW(img, center_x, y) == 0))
    {
        cu++;
        state[1]++;
        y--;
    }
    if(y < 0)
        return -CPA;
    
    while((y >= 0) && get_BW(img, center_x, y) == 1)
    {
        cu++;
        state[0]++;
        y--;
    }
    if(y < 0)
        return -CPA;
    
    y = center_y + 1;
     
    while((y < h) && (get_BW(img, center_x, y) == 0))
    {
        cd++;
        state[1]++;
        y++;
    }
    if(y >= h)
        return -CPA;
    
    while((y < h) && get_BW(img, center_x, y) == 1)
    {
        cd++;
        state[2]++;
        y++;
    }
    if(y >= h)
        return -CPA;
    
    int totsize = checkRatio(state, CPA);
    
    if(totsize == 0)
        return -CPA;
    
    double dif = abs(cd - cu);
     
    if(cu >= cd)
        return -dif/2;
    else
        return +dif/2;
}

static inline
int VerifyCenterD(SDL_Surface *img, int center_x, int center_y, double CPA)
{
    int h = img->h;
    int w = img->w;
    int state[3] = {0};
    int i = 0;
    
    while((center_y >= i) && (center_x >= i) && (get_BW(img, center_x - i, center_y - i) == 0))
    {
        state[1]++;
        i++;
    }
    if(center_y < i || center_x < i)
        return 0;
    
    while((center_y >= i) && (center_x >= i) &&  get_BW(img, center_x - i, center_y - i) == 1)
    {
        state[0]++;
        i++;
    }
    if(center_y < i || center_x < i)
        return 0;
    
    i = 1;
     
    while((center_y + i < h) && (center_x + i < w) && (get_BW(img, center_x + i, center_y + i) == 0))
    {
        state[1]++;
        i++;
    }
    if(center_y + i >= h || center_x + i >= w)
        return 0;
    
    while((center_y + i < h) && (center_x + i < w) &&  get_BW(img, center_x + i, center_y + i) == 1)
    {
        state[2]++;
        i++;
    }
    if(center_y + i >= h || center_x + i >= w)
        return 0;
    
    int totsize = checkRatio(state, CPA);
    
    if(totsize == 0)
        return 0;
    return 1;
}

static inline
void LocateAP(SDL_Surface *img, double *P, double CPx, double CPy) //version 2 - 6 for now
{
    //printf("color at %lf/ %lf : %d\n", P[0], P[1], get_BW(img, P[0], P[1]));
    if(get_BW(img, P[0], P[1]) == 1)
    {
        err(EXIT_FAILURE, "Segmentation not implemented : x 14");
    }
    //check horizontal
    int check = VerifyCenterH(img, P[0], P[1], CPx);
    if(check == -CPx)
    {
        display_image(img);
        err(EXIT_FAILURE, "Segmentation not implemented : x11");
    }
    
    P[0] += check;   
    //check vertical
    check = VerifyCenterV(img, P[0], P[1], CPy);
    if(check == -CPy)
    {
        display_image(img);
        err(EXIT_FAILURE, "Segmentation not implemented : x12");
    }
    
    P[1] += check;
    //check diagonal
    if(VerifyCenterD(img, P[0], P[1], (CPx + CPy)/2) == 0)
    {
        display_image(img);
        err(EXIT_FAILURE, "Segmentation not implemented : x13");
    }
}

static inline
void SampleGridKernel(SDL_Surface *img, char **mat,
double XA, double YA, double XB, double YB, double XC, double YC, double XD, double YD, 
double CPxp, double CPyp, double CPx, double CPy, int AP)
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
                mat[y][x] = '1';
            }
            else
            {
                mat[y][x] = '0';
            }
            putpixel(img, X, Y, SDL_MapRGB(img->format, 255, 0, 0));
        }
    }
    //print_mat(mat, AP + 13);
    //display_image(img);
}

static inline
void SampleGridBorderLeft(SDL_Surface *img, char **mat,
double XA, double YA, double XC, double YC,
double CPyp, double CPAx, double CPCx, int AP)
{
    int X;
    int Y;
    double aX;
    double aY;
    double cX;
    double cY;
    double weightY; 
    for(int y = 8; y < AP + 5; y++)
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
                mat[y][x] = '1';
            }
            else
            {
                mat[y][x] = '0';
            }
            putpixel(img, X, Y, SDL_MapRGB(img->format, 255, 0, 0));
        }
    }
    //print_mat(mat, AP + 13);
    //display_image(img);
}

static inline
void SampleGridBorderTop(SDL_Surface *img, char **mat,
double XA, double YA, double XB, double YB,
double CPxp, double CPAy, double CPBy, int AP)
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
        for(int x = 8; x < AP + 5; x++)
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
                mat[y][x] = '1';
            }
            else
            {
                mat[y][x] = '0';
            }
            putpixel(img, X, Y, SDL_MapRGB(img->format, 255, 0, 0));
        }
    }
    //print_mat(mat, AP + 13);
    //display_image(img);
}

static inline
void SampleGridBorderBottom(SDL_Surface *img, char **mat,
double XC, double YC, double XD, double YD,
double CPx, double CPCy, double CPy, int AP)
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
        for(int x = 8; x < AP + 6; x++)
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
                mat[y][x] = '1';
            }
            else
            {
                mat[y][x] = '0';
            }
            putpixel(img, X, Y, SDL_MapRGB(img->format, 255, 0, 0));
            //display_image(img);
        }
    }
    //print_mat(mat, AP + 13);
    //display_image(img);
}

static inline
void SampleGridBorderRight(SDL_Surface *img, char **mat,
double XB, double YB, double XD, double YD,
double CPx, double CPBx, double CPy, int AP)
{
    int X;
    int Y;
    double bX;
    double bY;
    double dX;
    double dY;
    double weightY;
    for(int y = 8; y < AP + 6; y++)
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
                mat[y][x] = '1';
            }
            else
            {
                mat[y][x] = '0';
            }
            putpixel(img, X, Y, SDL_MapRGB(img->format, 255, 0, 0));
            //display_image(img);
        }
    }
    //print_mat(mat, AP + 13);
    //display_image(img);
}

static inline
void SampleGridCorner(SDL_Surface *img, char **mat,
double XD, double YD,
double CPx, double CPy, int AP)
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
                mat[y][x] = '1';
            }
            else
            {
                mat[y][x] = '0';
            }
            putpixel(img, X, Y, SDL_MapRGB(img->format, 255, 0, 0));
            //display_image(img);
        }
    }
    //print_mat(mat, AP + 13);
    //display_image(img);
}

/*-----Main-----*/

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
    int *AP = MarkOneAlignment(mat, qr->version);
    //print_mat(mat, size);
    double CPAx = (double)WA / 7;
    double CPBx = (double)WB / 7;
    double CPCx = (double)WC / 7;
    double CPAy = (double)HA / 7;
    double CPBy = (double)HB / 7;
    double CPCy = (double)HC / 7;
    //double CPx = (CPAx + CPBx + CPCx)/3;
    //double CPy = (CPAy + CPBy + CPCy)/3;
    double XA = qrimg->coordA[0] + 3 * CPAx;
    double YA = qrimg->coordA[1] + 3 * CPAy;
    double XB = qrimg->coordB[0] - 3 * CPBx;
    double YB = qrimg->coordB[1] + 3 * CPBy;
    double XC = qrimg->coordC[0] + 3 * CPCx;
    double YC = qrimg->coordC[1] - 3 * CPCy;
    double *P = malloc(sizeof(double) * 2);
    P[0] = ((XA + CPAx * (AP[0] - 6)) + XB + (XC + CPCx * (AP[0] - 6))) / 3;
    P[1] = ((YA + CPAy * (AP[1] - 6)) + (YB + CPCy * (AP[1] - 6)) + YC) / 3;
    putpixel(qrimg->img, XA, YA, SDL_MapRGB(qrimg->img->format, 255, 0, 0));
    putpixel(qrimg->img, XB, YB, SDL_MapRGB(qrimg->img->format, 255, 0, 0));
    putpixel(qrimg->img, XC, YC, SDL_MapRGB(qrimg->img->format, 255, 0, 0));
    putpixel(qrimg->img, P[0], P[1], SDL_MapRGB(qrimg->img->format, 255, 0, 0));
    LocateAP(qrimg->img, P, CPAx, CPAy); //Do it again
    putpixel(qrimg->img, P[0], P[1], SDL_MapRGB(qrimg->img->format, 255, 0, 0));
    //display_image(qrimg->img);
    
    //AP SCANNED
    
    double Lxp = abs(XA - XB);
    double Lyp = abs(YA - YC);
    double Lx = abs(XC - P[0]);
    double Ly = abs(YB - P[1]);
    double CPxp = Lxp / distAP;
    double CPyp = Lyp / distAP;
    double CPx = Lx / distAP;
    double CPy = Ly / distAP;
    //SampleGridKernel A->P
    
    SampleGridKernel(qrimg->img, mat, XA, YA, XB, YB, XC, YC, P[0], P[1], CPxp, CPyp, CPx, CPy, distAP);
    
    //SampleGridBorders
    
    SampleGridBorderLeft(qrimg->img, mat, XA, YA, XC, YC, CPyp, CPAx, CPCx, distAP);
    SampleGridBorderTop(qrimg->img, mat, XA, YA, XB, YB, CPxp, CPAy, CPBy, distAP);
        //we should try to find CPxend and CPyend for better Sampling for
        //bot/right and corner
    SampleGridBorderBottom(qrimg->img, mat, XC, YC, P[0], P[1], CPx, CPCy, CPy, distAP);
    SampleGridBorderRight(qrimg->img, mat, XB, YB, P[0], P[1], CPx, CPBx, CPy, distAP);
    
    //Sample Grid Corner
    SampleGridCorner(qrimg->img, mat, P[0], P[1], CPx, CPy, distAP);
    qr->mat = mat;
    //display_image(qrimg->img);
}

/*void SampleCodeV7_40(SDL_Surface *qrimg, struct QrCode *qr)
{ 
    int size = qr->version * 4 + 17;
    char **mat = malloc(sizeof(char*) * size)
    for(int i = 0; i < size, i++)
    {
        mat[i] = malloc(sizeof(char) * size)
        for(int j = 0; j < size; j++) 
            mat[i][j] = '0';
    }
    
}*/
