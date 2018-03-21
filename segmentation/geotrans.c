/*
**  Geometric Transformation
**  file : geotrans.c
**  description : Transforms QrCode image into a readable image using an
**  Geometric  transformations.
*/

# define SIZE 75

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
x3, double y3, double size)
{
    double **AugMat = malloc(sizeof(double*)*6);
    for(size_t i = 0; i < 6; i++)
        AugMat[i] = calloc(7, sizeof(double));
    
    //val to change
    
    double u1 = SIZE;
    double v1 = SIZE;
    double u2 = SIZE;
    double v2 = SIZE + size;
    double u3 = SIZE + size;
    double v3 = SIZE;
    
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
        warn("Determinant == 0");
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


SDL_Surface *FrontMapping(SDL_Surface *oldimg, double *vals, double size)
{
    int newsize = SIZE + size + SIZE; 
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

SDL_Surface *BackMapping(SDL_Surface *oldimg, double *vals, double size)
//vals have to be inerted first
{
    int newsize = SIZE + size + SIZE;
    SDL_Surface *img = create_image(newsize, newsize);
    
    double a = vals[0];
    double b = vals[1];
    double c = vals[2];
    double d = vals[3];
    double e = vals[4];
    double f = vals[5];
    
    int x = 0;
    int y = 0;
    
    for(int v = 0; v < img->h ; v++)
    {
        for(int u = 0; u < img->w; u++)
        {
            x = round(a*u + b*v + c);
            y = round(d*u + e*v + f);
            
            if( x < oldimg->w && x > 0 && y < oldimg->h && y > 0)
            {
                Uint32 p = getpixel(oldimg, x, y);
                putpixel(img, u, v, p);
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

SDL_Surface *GeoTransform(SDL_Surface *img, struct FPresults *f)
{
    double *solutions = SolveAffineEquations(f->x1, f->y1, f->x2, f->y2, f->x3,
                                            f->y3, f->dist);
   
    double size = f->dist; 
    if(InvertMat(solutions) == 0)
    {
        warn("Backmapping failed");
        return FrontMapping(img, solutions, size);
    }
    
    return BackMapping(img, solutions, size);
}
