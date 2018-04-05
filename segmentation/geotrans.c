/*
**  Geometric Transformation
**  file : geotrans.c
**  description : Transforms QrCode image into a readable image using an
**  Geometric  transformations.
*/

# define SIZE 400
# define MULT 5

# include "geotrans.h"

static inline
void free_mat(double **mat, int n)
{
    for(int i = 0; i < n; i++)
        free(mat[i]);
    free(mat);
}

static inline
void white_map(SDL_Surface *img)
{
    for(int y = 0; y < img->h; y++)
    {
        for(int x = 0; x < img->w; x++)
        {
            putpixel(img, x, y, SDL_MapRGB(img->format, 255, 255, 255));
        }
    }
}


static inline
void print_mat(double **mat, int rows, int cols)
{
    for(int i = 0; i < rows; i++)
    {
        printf("[ ");
        for(int j = 0; j < cols; j++)
        {
            printf("%03lf ", mat[i][j]);
        }
        printf("]\n");
    }
}

static inline
double *GaussianElimination(double **mat, size_t rows, size_t cols)//Gauss-Jordan
{
    for(size_t i = 0; i < rows - 1; i++)
    {
        for(size_t k = i + 1; k < rows; k++)
        {
            double tmp = mat[k][i] / mat[i][i];

            for(size_t j = 0; j < cols; j++)
            {
                mat[k][j] = mat[k][j] - tmp * mat[i][j];
            }
        }
    }
    
    //printf("\n");
    //print_mat(mat, rows, cols);
    double *sol = malloc(sizeof(double)*6);
    sol[rows - 1] = mat[rows - 1][cols - 1] / mat[rows -1][rows - 1];
    for(int i = rows - 2; i >= 0; i--)
    {
        sol[i] = mat[i][cols -1];
        for(size_t j = i + 1; j < rows; j++)
        {
            sol[i] -= mat[i][j] * sol[j];
        }
        sol[i] = sol[i] / mat[i][i];
    }
 
    //printf("\n [ %lf, %lf, %lf, %lf, %lf, %lf ]\n", sol[0], sol[1], sol[2], sol[3], sol[4], sol[5]);
    //printf("\n");
    //print_mat(mat, rows, cols);
    return sol;
}

double *SolveAffineEquations(double x1, double y1, double x2, double y2, double
x3, double y3)
{
    double **AugMat = malloc(sizeof(double*)*6);
    for(size_t i = 0; i < 6; i++)
        AugMat[i] = calloc(7, sizeof(double));
    
    //val to change
    
    /*double u1 = size;
    double v1 = size;
    double u2 = size;
    double v2 = size + size*MULT;
    double u3 = size + size*MULT;
    double v3 = size;*/
    
    
    double u1 = SIZE;
    double v1 = SIZE;
    double u2 = SIZE + SIZE*2;
    double v2 = SIZE;
    double u3 = SIZE;
    double v3 = SIZE + SIZE*2;
    
    AugMat[0][0] = x1;
    AugMat[1][0] = x2;
    AugMat[2][0] = x3;
    AugMat[3][3] = x1;
    AugMat[4][3] = x2;
    AugMat[5][3] = x3;
    AugMat[0][1] = y1;
    AugMat[1][1] = y2;
    AugMat[2][1] = y3;
    AugMat[3][4] = y1;
    AugMat[4][4] = y2;
    AugMat[5][4] = y3;
    AugMat[0][6] = u1;
    AugMat[1][6] = u2;
    AugMat[2][6] = u3;
    AugMat[3][6] = v1;
    AugMat[4][6] = v2;
    AugMat[5][6] = v3;
    AugMat[0][2] = 1;
    AugMat[1][2] = 1;
    AugMat[2][2] = 1;
    AugMat[3][5] = 1;
    AugMat[4][5] = 1;
    AugMat[5][5] = 1;
    
    //print_mat(AugMat, 6, 7);
    //printf("\n"); 
    double *sol = GaussianElimination(AugMat, 6, 7);
    
    free_mat(AugMat, 6);
    return sol;
}

/* ---- MATRIX INVERSION BEGIN ---- */

static inline
void getCofactor(double **mat, double **tmp, int p, int q, int n)
{
    int i = 0;
    int j = 0;
    
    for(int row = 0; row < n; row++)
    {
        for(int col = 0; col < n; col++)
        {
            if(row != p && col != q)
            {
                tmp[i][j++] = mat[row][col];
                
                if (j == n - 1)
                {
                    j = 0;
                    i++;
                }
            }
        }
    }
}

static inline
double determinant(double **mat, int n)
{
    double ret = 0;
    
    if(n == 1)
        return mat[0][0];
    
    double **tmp = malloc(sizeof(double*)*n);
    for(int i = 0; i < n; i++)
        tmp[i] = calloc( n, sizeof(double));
    
    int sign = 1;
    
    for (int f = 0; f < n; f++)
    {
        getCofactor(mat, tmp, 0, f, n);
        ret += sign * mat[0][f] * determinant(tmp, n - 1);
        sign = -sign;
    }
    
    free_mat(tmp, n);
    return ret;
}

static inline
void adjoint(double **mat, double **adj, int n)
{
    if(n == 1)
    {
        adj[0][0] = 1;
        return;
    }

    int sign = 1;
    double **tmp = malloc(sizeof(double*)*n);
    for(int i = 0; i < n; i++)
        tmp[i] = calloc( n, sizeof(double));
    
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            getCofactor(mat, tmp, i, j, n);
            if((i + j) % 2 == 0)
                sign = 1;
            else
                sign = -1;
            
            adj[j][i] = sign * determinant(tmp, n - 1);
        }
    }
    
    free_mat(tmp, n); 
}

static inline
int inverse(double **mat, double **inv, int n)
{
    double det = determinant(mat, n);
    if (det == 0)
    {
        return 0;
    }
 
    double **adj = malloc(sizeof(double*)*n);
    for(int i = 0; i < n; i++)
        adj[i] = calloc( n, sizeof(double));
    
    adjoint(mat, adj, n);
       
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            inv[i][j] = adj[i][j]/det;
        }
    }
    
    free_mat(adj, n);
    return 1;
}

/* ---- MATRIX INVERSION END ---- */

int InvertMat(double *vals) //3*3 matrix
{
    double **mat = malloc(sizeof(double*) * 3);
    for(int i  = 0; i < 3; i++)
        mat[i] = calloc(3, sizeof(double));
    
    mat[0][0] = vals[0];
    mat[0][1] = vals[1];
    mat[0][2] = vals[2];
    mat[1][0] = vals[3];
    mat[1][1] = vals[4];
    mat[1][2] = vals[5];
    mat[2][2] = 1;
    //print_mat(mat, 3, 3);
     
    double **inv = malloc(sizeof(double*)*3);
    for(int i = 0; i < 3; i++)
        inv[i] = calloc( 3, sizeof(double));
    
    if(inverse(mat, inv, 3) == 0)
        return 0;
   
    //printf("\n"); 
    //print_mat(inv, 3, 3);
    vals[0] = inv[0][0];
    vals[1] = inv[0][1];
    vals[2] = inv[0][2];
    vals[3] = inv[1][0];
    vals[4] = inv[1][1];
    vals[5] = inv[1][2];
    free_mat(inv, 3);
    free_mat(mat, 3);
    return 1;
}

Uint32 BilinearInterpolationUnitSquareBW(SDL_Surface *img, double x, double y)
{
    int xm = floor(x);
    int ym = floor(y);
    int xM = ceil(x);
    int yM = ceil(y);
    Uint32 p1 = getpixel(img, xm, ym); //top left
    Uint32 p2 = getpixel(img, xM, ym); //top right
    Uint32 p3 = getpixel(img, xm, yM); //bottom left
    Uint32 p4 = getpixel(img, xM, yM); //bottom right
    Uint8 r1, g1, b1;
    Uint8 r2, g2, b2;
    Uint8 r3, g3, b3;
    Uint8 r4, g4, b4;
    SDL_GetRGB(p1, img->format, &r1, &g1, &b1); 
    SDL_GetRGB(p2, img->format, &r2, &g2, &b2);
    SDL_GetRGB(p3, img->format, &r3, &g3, &b3);
    SDL_GetRGB(p4, img->format, &r4, &g4, &b4);
    double weightX = x - xm;
    double weightY = y - ym;
    Uint8 r = r1 * (1 - weightY) * (1 - weightX) + r2 * (1 - weightY) * weightX
            + r3 * weightY * (1 - weightX) + r4 * weightY * weightX;
    Uint8 g = g1 * (1 - weightY) * (1 - weightX) + g2 * (1 - weightY) * weightX
            + g3 * weightY * (1 - weightX) + g4 * weightY * weightX;
    Uint8 b = b1 * (1 - weightY) * (1 - weightX) + b2 * (1 - weightY) * weightX
            + b3 * weightY * (1 - weightX) + b4 * weightY * weightX;
    return SDL_MapRGB(img->format, r, g, b);
    
}

SDL_Surface *FrontMapping(SDL_Surface *oldimg, double *vals, double size)
{
    int newsize = size + size*2 + size; 
    SDL_Surface *img = create_image(newsize, newsize);
    double a = vals[0];
    double b = vals[1];
    double c = vals[2];
    double d = vals[3];
    double e = vals[4];
    double f = vals[5];
    
    int u = 0;
    int v = 0;
    
    for(int y = 0; y < oldimg->h ; y++)
    {
        for(int x = 0; x < oldimg->w; x++)
        {
            u = round(a*x + b*y + c);
            v = round(d*x + e*y + f);
            
            if( u < img->w && u > 0 && v < img->h && v > 0)
            {
                Uint32 p = getpixel(oldimg, x, y);
                putpixel(img, u, v, p);
            } 
        }
    }
    free(vals); 
    return img;
}

SDL_Surface *BackMapping(SDL_Surface *oldimg, double *vals) //add size later
//vals have to be inverted first
{
    int newsize = SIZE + SIZE*2 + SIZE;
    SDL_Surface *img = create_image(newsize, newsize);
    
    double a = vals[0];
    double b = vals[1];
    double c = vals[2];
    double d = vals[3];
    double e = vals[4];
    double f = vals[5];
    
    double x = 0;
    double y = 0;
    
    for(int v = 0; v < img->h ; v++)
    {
        for(int u = 0; u < img->w; u++)
        {
            x = a*u + b*v + c;
            y = d*u + e*v + f;
            
            if( x < oldimg->w && x > 0 && y < oldimg->h && y > 0)
            {
                
                putpixel(img, u, v, BilinearInterpolationUnitSquareBW(oldimg, x, y)); 
                //Uint32 p = getpixel(oldimg, x, y);
                //putpixel(img, u, v, p);
            }
            else
            {
                putpixel(img, u, v, SDL_MapRGB(img->format, 255, 255, 255));
            }
            
        }
    }
    free(vals);
    return img;
}

struct GeoImg *GeoTransform(SDL_Surface *img, struct FPresults *f)
{
    double *solutions = SolveAffineEquations(f->x1, f->y1, f->x2, f->y2, f->x3,
                                            f->y3);
    
    struct GeoImg *ret = malloc(sizeof(struct GeoImg));  

    ret->coordA = malloc(sizeof(int) * 2);
    ret->coordB = malloc(sizeof(int) * 2);
    ret->coordC = malloc(sizeof(int) * 2);
    
    ret->coordA[0] = round(solutions[0] * f->x1 + solutions[1] * f->y1 +
    solutions[2]); 
    ret->coordA[1] = round(solutions[3] * f->x1 + solutions[4] * f->y1 +
    solutions[5]); 
    ret->coordB[0] = round(solutions[0] * f->x2 + solutions[1] * f->y2 +
    solutions[2]); 
    ret->coordB[1] = round(solutions[3] * f->x2 + solutions[4] * f->y2 +
    solutions[5]); 
    ret->coordC[0] = round(solutions[0] * f->x3 + solutions[1] * f->y3 +
    solutions[2]); 
    ret->coordC[1] = round(solutions[3] * f->x3 + solutions[4] * f->y3 +
    solutions[5]);

    //double size = f->dist; 
    if(InvertMat(solutions) == 0)
    {
        //ret->img FrontMapping(img, solutions, size);
        err(EXIT_FAILURE, "Segmentation error : Transformation Inversion");
    }
 
    ret->img = BackMapping(img, solutions);
    //display_image(ret->img);
    return ret;
}
