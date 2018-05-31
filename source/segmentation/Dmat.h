/*
**  Dyanimic lines matrix 
**  file : Dmat.h
**  description : Dynamic matrices for easy computations, lines are dynamic,
**  columns are static
*/

# ifndef _DMAT_H_
# define _DMAT_H_

# include <stdlib.h>
# include <err.h>

# define foreach_line(_Dmat_)   \
    double **mat = _Dmat_->mat;           \
    for (size_t i = 0; i < _Dmat_->size; i++)
 
struct Dmat
{
    size_t lines, cols, size ;
    double **mat;
};

static inline
struct Dmat *init_Dmat (size_t lines, size_t cols)
{
    double **mat = malloc(sizeof(double*) * lines);
    for(size_t i = 0; i < lines; i++)
        mat[i] = calloc(cols, sizeof(double));
    
    struct Dmat *nov = malloc(sizeof(struct Dmat));
    nov->lines = lines;
    nov->cols = cols;
    nov->size = 0;
    nov->mat = mat;
    return nov;
}

static inline
void copy_Dmat (struct Dmat *src , struct Dmat *dst)
{
    if ( src->lines > dst->lines &&
         src->cols != dst->cols )
    {
        warn("_DMAT_ERROR_ARG");
        return;
    }
    
    for(size_t i = 0; i < src->lines ; i++)
    {
        for(size_t j = 0; j < src->cols ; j++)
        {
            dst->mat[i][j] = src->mat[i][j];
        }
    }
    dst->size = src->size;  
}

static inline
void free_Dmat (struct Dmat *mat)
{
    for(size_t i = 0; i < mat->lines; i++)
        free(mat->mat[i]);
    free(mat->mat);
    free(mat);

}
static inline
void add_Dmat (struct Dmat *mat, double *elm)
{   
    //warn("1");
    if (mat->size >= mat->lines)
    {  
        struct Dmat *tmp = init_Dmat(mat->lines*2, mat->cols);
        copy_Dmat(mat, tmp);
        *mat = *tmp;
        free(tmp);
    }
    //warn("2");
    
    for(size_t i = 0; i < mat->cols; i++)
        mat->mat[mat->size][i] = elm[i];
    mat->size++;
    //free(elm);
    //warn("3");
}

# endif
