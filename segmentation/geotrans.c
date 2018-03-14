/*
**  Geometric Transformation
**  file : geotrans.c
**  description : Transforms QrCode image into a readable image using an
**  Geometric  transformations.
*/

# include "geotrans.h"

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
double *GaussianElimination(double **mat, size_t rows, size_t cols)
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
    
    print_mat(mat, rows, cols - 1);
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
 
    printf("\n [ %lf, %lf, %lf, %lf, %lf, %lf ]\n", sol[0], sol[1], sol[2], sol[3], sol[4], sol[5]);
    return sol;
}

double *SolveAffineEquations(double x1, double y1, double x2, double y2, double
x3, double y3, size_t size)
{
    double **AugMat = malloc(sizeof(double*)*6);
    for(size_t i = 0; i < 6; i++)
        AugMat[i] = calloc(7, sizeof(double));
    
    //val to change
    
    double u1 = 40;
    double v1 = 40;
    double u2 = 40;
    double v2 = 40 + size;
    double u3 = 40 + size;
    double v3 = 40;
    
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
    
    print_mat(AugMat, 6, 7);
    printf("\n"); 
    double *sol = GaussianElimination(AugMat, 6, 7);
    
    return sol;
}

/*
double GaussianEliminations(double **mat, size_t rows, size_t cols)
{

}
*/
