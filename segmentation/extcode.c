/*
**  QrCode Extractor
**  file : extcode.c
**  description : takes in the found centers of the Finder patterns, returns the
**  code in a matrix. For now, it DOESN'T take in account any geometric
**  rectification(NoG functions) and The FPs are already ideally placed. (INCOMMING IN PHASE 2)
*/

# include "extcode.h"

# define PRINT_STATE(_state_)       \
    for (int i = 0 ; i < 5 ; i++)   \
        warn("State %d , size %d", i, _state_[i]);

# define foreach_state(_state_)     \
    for (int i = 0 ; i < 5 ; i++)

// STATIC FUNCTIONS

static inline
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

static inline                                                                   
int get_BW(SDL_Surface *img, int x, int y) //returns 0 if black, 1 if white     
{                                                                               
     Uint32 pixel = getpixel(img, x, y);                                         
     Uint8 r, g, b;                                                              
     SDL_GetRGB(pixel, img->format, &r, &g, &b);                                 
                                                                                 
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
    if(g == 255 && r == 255 && b == 255)
    {
        return '0';
    }
    else if (g == 0 && r == 0 && b == 0)
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

static inline
int check_ratio(int *state)
{
    //PRINT_STATE(state);
    int totalsize = 0;
    foreach_state(state)
    {
        totalsize += state[i];
    }
    
    if (totalsize < 7)
        return 0;

    int module_size = totalsize / 7; 
    int max_var = module_size / 2;   //pixel error correction
    
    if((abs(module_size - (state[0])) <= max_var) &&
       (abs(module_size - (state[1])) <= max_var) &&
       (abs(module_size * 3 - (state[2])) <= max_var * 3) &&
       (abs(module_size - (state[3])) <= max_var) &&
       (abs(module_size - (state[4])) <= max_var))
        return totalsize;
    else
        return 0;
}

static inline
double GetHeightFP(SDL_Surface *img, int center_x, int center_y)
{
    int h = img->h;
    int state_check[5] = {0};
    int y = center_y;
    
    //upwards check from center
 
    while((y >= 0) && (get_BW(img, center_x, y) == 0))
    {
        state_check[2]++ ;
        y--;
    }
    if(y < 0)
        return 0;

    while((y >= 0) && (get_BW(img, center_x, y) == 1))
    {
        state_check[1]++ ;
        y--;
    }
    if(y < 0)
        return 0;

    while((y >= 0) && (get_BW(img, center_x, y) == 0))
    {
        state_check[0]++ ;
        y--;
    }
    if(y < 0)
        return 0;
    
    //downwards check from center
    
    y = center_y + 1; 

    while((y < h) && (get_BW(img, center_x, y) == 0))
    {
        state_check[2]++ ;
        y++;
    }
    if(y > h)
        return 0;

    while((y < h) && (get_BW(img, center_x, y) == 1))
    {
        state_check[3]++ ;
        y++;
    }
    if(y > h)
        return 0;

    while((y < h) && (get_BW(img, center_x, y) == 0))
    {
        state_check[4]++ ;
        y++;
    }
    if(y > h)
        return 0;
    
    return check_ratio(state_check);    
}

static inline
double GetWidthFP(SDL_Surface *img, int center_x, int center_y)
{
    int w = img->w;
    int state_check[5] = {0};
    int x = center_x;
    
    //leftwards check from center
    
    while((x >= 0) && (get_BW(img, x, center_y) == 0))
    {
        state_check[2]++ ;
        x--;
    }
    if(x < 0)
        return 0;
 
    while((x >= 0) && (get_BW(img, x, center_y) == 1))
    {
        state_check[1]++ ;
        x--;
    }
    if(x < 0)
        return 0;

    while((x >= 0) && (get_BW(img, x, center_y) == 0))
    {
        state_check[0]++ ;
        x--;
    }
    if(x < 0)
        return 0;
    
    //rightwards check from center
    
    x = center_x + 1; 
    
    while((x < w) && (get_BW(img, x, center_y) == 0))
    {
        state_check[2]++ ;
        x++;
    }
    if(x >= w)
        return 0;
 
    while((x < w) && (get_BW(img, x, center_y) == 1))
    {
        state_check[3]++ ;
        x++;
    }
    if(x >= w)
        return 0;

    while((x < w) && (get_BW(img, x, center_y) == 0))
    {
        state_check[4]++ ;
        x++;
    }
    if(x >= w)
        return 0;
    
    return check_ratio(state_check);    
}

static inline
double UpdateX(SDL_Surface *img, int center_x , int center_y, double X, int bin)
{
    int w = img->w;
    int cleft = 0;
    int cright = 0;
    int x = center_x;
    
    while((x >= 0) && get_BW(img, x, center_y) == bin)
    {
        cleft++;
        x--;
    }
    
    x = center_x + 1;
    
    while((x < w) && get_BW(img, x, center_y) == bin)
    {
        cright++;
        x++;
    }
    //if( x >= w)
       //err(EXIT_FAILURE, "Segmentation error x07");
   
    //printf("N1 val of cleft crigtht and X: %d %d %lf\n", cleft, cright, X);
    
    int modl = cleft / (int)round(X);
    int modr = cright / (int)round(X);
    
    //printf("modl and modr : %d %d \n", modl, modr);
    
    cleft = cleft - X * modl;
    cright = cright - (X * modr);
    //printf("N2 val of cleft crigtht and X: %d %d %lf\n", cleft, cright, X);
    
      
    int dif = abs(cleft - cright) / 3;
    
    if(cleft >= cright)
        return -dif;
    else
        return dif;
    
}

static inline
double UpdateY(SDL_Surface *img, int center_x , int center_y, double Y, int bin)
{
    int h = img->h;
    int cup = 0;
    int cdown = 0;
    int y = center_y;
    
    while((y >= 0) && get_BW(img, center_x, y) == bin)
    {
        cup++;
        y--;
    }
    
    y = center_y + 1;
    
    while((y < h) && get_BW(img, center_x, y) == bin)
    {
        cdown++;
        y++;
    }
    //if( y >= h)
       //err(EXIT_FAILURE, "Segmentation error x09");
   
    //printf("N1 val of cleft crigtht and X: %d %d %lf\n", cleft, cright, X);
    
    int modu = cup / (int)round(Y);
    int modd = cdown / (int)round(Y);
    
    //printf("modl and modr : %d %d \n", modl, modr);
    
    cup = cup - Y * modu;
    cdown = cdown - (Y * modd);
    //printf("N2 val of cleft crigtht and X: %d %d %lf\n", cleft, cright, X);
    
      
    int dif = abs(cup - cdown) / 3;
    
    if(cup >= cdown)
        return -dif;
    else
        return dif;
    
}
// MAIN FUNCTION
void SampleCodeV1E(struct GeoImg *qrimg, struct QrCode *qr, double X)
{
    int HA = GetHeightFP(qrimg->img, qrimg->coordA[0], qrimg->coordA[1]);
    int HC = GetHeightFP(qrimg->img, qrimg->coordC[0], qrimg->coordC[1]);
    
    if(HA == 0 || HC == 0)
        err(EXIT_FAILURE, "Segmentation error : Corrupted Geometry");
    
    double Y = (HA + HC) / 14;
    
    // initialize matrix

    char **mat = malloc(sizeof(char*) * 21);
    for(int i = 0; i < 21; i++)
    {
        mat[i] = calloc(21, sizeof(char));
        for(int j = 0; j < 21; j ++)
            mat[i][j] = '0';
    }
    
    // coord upper timing pattern
    
    double UT1y = qrimg->coordA[1] + 3 * Y;
    double UT1x = qrimg->coordA[0] + 5 * X;
    //double UTxEnd = qrimg->coordB[0] - 5 * X;
    
    // coord left timing pattern
    
    double LT1x = qrimg->coordA[0] + 3 * X;
    double LT1y = qrimg->coordA[1] + 5 * Y;
    
    // above upper timing pattern
    int ux = 0;
    int uy = 0;
    for(int y = 7; y >= 0; y--)
    {
        for(int x = 8; x < 13; x++)
        {
            mat[y][x] = get_CL(qrimg->img, UT1x + (x - 8) * X, UT1y + (y - 6) *
Y);
        }
    }
    
    // left of left timing pattern
    
    for(int x = 7; x >= 0; x--)
    {
        for(int y = 8; y < 13; y++)
        {
            mat[y][x] = get_CL(qrimg->img, LT1x + (x - 6) * X, LT1y + (y - 8) *
Y);
        }
    }
    
    // under right of timing patterns
  
    for(int y = 8; y < 21; y++)
    {
        for(int x = 8; x < 21 ; x++)
        {
            int cX = round(LT1x + (x - 6) * X + ux);
            int cY = round(UT1y + (y - 6) * Y + uy);
            mat[y][x] = get_CL(qrimg->img, cX, cY);
            //display_image(qrimg->img);
        }
        ux = 0;
        //uy = 0;
    }
    
    qr->mat = mat;
    qr->version = 1;
    //print_mat(mat, 21);
}


void SampleCodeV1(struct GeoImg *qrimg, struct QrCode *qr, double X)
{
    int HA = GetHeightFP(qrimg->img, qrimg->coordA[0], qrimg->coordA[1]);
    int HC = GetHeightFP(qrimg->img, qrimg->coordC[0], qrimg->coordC[1]);
    
    if(HA == 0 || HC == 0)
        err(EXIT_FAILURE, "Segmentation error : Corrupted Geometry");
    
    double Y = (HA + HC) / 14;
    
    // initialize matrix

    char **mat = malloc(sizeof(char*) * 21);
    for(int i = 0; i < 21; i++)
    {
        mat[i] = malloc(sizeof(char) * 21);
        for(int j = 0; j < 21; j ++)
            mat[i][j] = '0';
    }
    
    // coord upper timing pattern
    
    double UT1y = qrimg->coordA[1] + 3 * Y;
    double UT1x = qrimg->coordA[0] + 5 * X;
    //double UTxEnd = qrimg->coordB[0] - 5 * X;
    
    // coord left timing pattern
    
    double LT1x = qrimg->coordA[0] + 3 * X;
    double LT1y = qrimg->coordA[1] + 5 * Y;
    
    // above upper timing pattern
    int ux = 0;
    int uy = 0;
    for(int y = 7; y >= 0; y--)
    {
        for(int x = 8; x < 13; x++)
        {
            if(get_BW(qrimg->img, UT1x + (x - 8) * X, UT1y + (y - 6) * Y) == 0)
                mat[y][x] = '1';
            else
                mat[y][x] = '0';
       }
    }
    
    // left of left timing pattern
    
    for(int x = 7; x >= 0; x--)
    {
        for(int y = 8; y < 13; y++)
        {
            if(get_BW(qrimg->img, LT1x + (x - 6) * X, LT1y + (y - 8) * Y) == 0)
                mat[y][x] = '1';
            else
                mat[y][x] = '0';
        }
    }
    
    // under right of timing patterns
  
    for(int y = 8; y < 21; y++)
    {
        for(int x = 8; x < 21 ; x++)
        {
            int cX = round(LT1x + (x - 6) * X + ux);
            int cY = round(UT1y + (y - 6) * Y + uy);
            if(get_BW(qrimg->img, cX, cY) == 0)
            {
                mat[y][x] = '1';
                ux += UpdateX(qrimg->img, cX, cY, X, 0);
                uy += UpdateY(qrimg->img, cX, cY, Y, 0);
                //printf("val of ux : %d \n", ux);
                putpixel(qrimg->img, cX, cY, SDL_MapRGB(qrimg->img->format, 255, 0, 0));
            }
            else
            {
                mat[y][x] = '0';
                ux += UpdateX(qrimg->img, cX, cY, X, 1);
                uy += UpdateY(qrimg->img, cX, cY, Y, 1);
                //printf("val of ux : %d \n", ux);
                putpixel(qrimg->img, cX, cY, SDL_MapRGB(qrimg->img->format, 255, 0, 0));
            }
            //display_image(qrimg->img);
        }
        ux = 0;
        //uy = 0;
    }
    
    qr->mat = mat;
    qr->version = 1;
    //print_mat(mat, 21);
}

static inline
int CorrectVersion(char *fmt)
{
    int besti = 0;
    int bestdiff = 4;
    int diff = 0;
    
    for(int i = 0; i < 34; i++)
    {
        diff = 0;
        for(int j = 0; j < 18; j++)
        {
            if(fmt[j] != V_bits[i][j])
                diff++; 
        }
        
        //printf("%s %s %d\n", fmt, F_bits[i], diff);
        
        if(diff < bestdiff)
        {
            if(diff == 0)
                return i;
            bestdiff = diff;    
            besti = i;
            
        }
            
    }
    
    if(bestdiff > 3)
        return -1;
    free(fmt);
    return besti;    
}

static inline
int GetVersionV7_40N1(struct GeoImg *qrimg, int HB, int WB)
{
    char *fmt = calloc(18, sizeof(char));
    double CPx = (double)WB / 7;
    double CPy = (double)HB / 7;
    double Vx0 = qrimg->coordB[0] - 7 * CPx;
    double Vy0 = qrimg->coordB[1] - 3 * CPy;
    int X;
    int Y;
    for(int y = 0; y < 6; y++)
    {
        for(int x = 0; x < 3; x++)
        {
            X = round(Vx0 + CPx * x);
            Y = round(Vy0 + CPy * y);
            if(get_BW(qrimg->img, X, Y) == 0)
                fmt[17 - (x + y * 3)] = '1';
            else
                fmt[17 - (x + y * 3)] = '0';
            //putpixel(qrimg->img, X, Y, SDL_MapRGB(qrimg->img->format, 255, 0, 0));
            //display_image(qrimg->img);
        }
    }
    //warn("Version String : %s", fmt);
    return CorrectVersion(fmt) + 7; 
}

static inline
int GetVersionV7_40N2(struct GeoImg *qrimg, int HC, int WC)
{ 
    char *fmt = calloc(18, sizeof(char));
    double CPx = (double)WC / 7;
    double CPy = (double)HC / 7;
    double Vx0 = qrimg->coordC[0] - 3 * CPx;
    double Vy0 = qrimg->coordC[1] - 7 * CPy;
    int X;
    int Y;
    for(int x = 0; x < 6; x++)
    {
        for(int y = 0; y < 3; y++)
        {
            X = round(Vx0 + CPx * x);
            Y = round(Vy0 + CPy * y);
            if(get_BW(qrimg->img, X, Y) == 0)
                fmt[17 - (y + x * 3)] = '1';
            else
                fmt[17 - (y + x * 3)] = '0';
            //putpixel(qrimg->img, X, Y, SDL_MapRGB(qrimg->img->format, 255, 0, 0));
        }
    }
    //warn("Version String : %s", fmt);
    //display_image(qrimg->img);
    return CorrectVersion(fmt) + 7;
}


//Extracts QrCode struct for further computations
struct QrCode *extract_QrCode (struct GeoImg *qrimg)
{
    struct QrCode *qr = malloc(sizeof(struct QrCode));
    
    double D = sqrt(pow(qrimg->coordA[0] - qrimg->coordB[0], 2) +
    pow(qrimg->coordA[1] - qrimg->coordB[1], 2));
    
    int WA = GetWidthFP(qrimg->img, qrimg->coordA[0], qrimg->coordA[1]);
    int WB = GetWidthFP(qrimg->img, qrimg->coordB[0], qrimg->coordB[1]);
    int WC = GetWidthFP(qrimg->img, qrimg->coordC[0], qrimg->coordC[1]);
    int HA = GetHeightFP(qrimg->img, qrimg->coordA[0], qrimg->coordA[1]);
    int HB = GetHeightFP(qrimg->img, qrimg->coordB[0], qrimg->coordB[1]);
    int HC = GetHeightFP(qrimg->img, qrimg->coordC[0], qrimg->coordC[1]);
    
    if(WA == 0 || WB == 0 || WC == 0 || HA == 0 || HB == 0 || HC == 0)
        err(EXIT_FAILURE, "Segmentation error : Bad Geometry");
    
    double X = (WA + WB) / 14;

    int V = round((D / X - 10) / 4);
    if( V >= 7 && V <= 40)
    {
        //printf("%i\n", V);
        //V = GetVersionV7_40N2(qrimg, HB, WB);
        //printf("%i\n", V);
        /*if( V == 6)
        {
            V = GetVersionV7_40N1(qrimg, HC, WB);
            if( V == 6)
                err(EXIT_FAILURE, "Segmentation error : Version Corrupted");
                SampleCodeV7_40(qrimg, qr, WA, WB, WC, HA, HB, HC);
        }*/
        qr->version = V;
        SampleCodeV7_40(qrimg, qr, WA, WB, WC, HA, HB, HC);
    }
    else if( V >= 2 && V <= 6)
    {
        qr->version = V;
        SampleCodeV2_6(qrimg, qr, WA, WB, WC, HA, HB, HC);
    }
    else if( V == 1)
    {
        SampleCodeV1(qrimg, qr, X);  
    }
    else
    {
        warn("V = %d", V);
        err(EXIT_FAILURE, "Segmentation error : Corrupted QrCode size");
    }
    
    return qr;
}

struct QrCode *extract_EpCode (struct GeoImg *qrimg, struct GeoImg *coimg)
{
    struct QrCode *qr = malloc(sizeof(struct QrCode));
    
    double D = sqrt(pow(qrimg->coordA[0] - qrimg->coordB[0], 2) +
    pow(qrimg->coordA[1] - qrimg->coordB[1], 2));
    
    int WA = GetWidthFP(qrimg->img, qrimg->coordA[0], qrimg->coordA[1]);
    int WB = GetWidthFP(qrimg->img, qrimg->coordB[0], qrimg->coordB[1]);
    int WC = GetWidthFP(qrimg->img, qrimg->coordC[0], qrimg->coordC[1]);
    int HA = GetHeightFP(qrimg->img, qrimg->coordA[0], qrimg->coordA[1]);
    int HB = GetHeightFP(qrimg->img, qrimg->coordB[0], qrimg->coordB[1]);
    int HC = GetHeightFP(qrimg->img, qrimg->coordC[0], qrimg->coordC[1]);
    
    if(WA == 0 || WB == 0 || WC == 0 || HA == 0 || HB == 0 || HC == 0)
        err(EXIT_FAILURE, "Segmentation error : Bad Geometry");
    
    double X = (WA + WB) / 14;

    int V = round((D / X - 10) / 4);
    
    if( V >= 7 && V <= 40)
    {
        V = GetVersionV7_40N2(qrimg, HB, WB);
        /*if( V == 6)
        {
            V = GetVersionV7_40N1(qrimg, HC, WB);
            if( V == 6)
            {
                //warn("wrong version maybe");
                //err(EXIT_FAILURE, "Segmentation error : Version Corrupted");
                SampleCodeV7_40E(qrimg, qr, WA, WB, WC, HA, HB, HC, coimg->img);
            }
        }*/
        qr->version = V;
        SampleCodeV7_40E(qrimg, qr, WA, WB, WC, HA, HB, HC, coimg->img);
    }
    else if( V >= 2 && V <= 6)
    {
        qr->version = V;
        SampleCodeV2_6E(qrimg, qr, WA, WB, WC, HA, HB, HC, coimg->img);
    }
    else if( V == 1)
    {
        SampleCodeV1E(coimg, qr, X);  
    }
    else
    {
        warn("V = %d", V);
        err(EXIT_FAILURE, "Segmentation error : Corrupted QrCode size");
    }
    
    return qr;
}
