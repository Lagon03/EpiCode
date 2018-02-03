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
 
struct Dmat
{
    size_t lines, cols, size ;
    int **mat;
};

static inline
struct Dmat *init_Dmat (size_t lines, size_t cols)
{
    int **mat = malloc(sizeof(int*) * lines);
    for(size_t i = 0; i < lines; i++)
        mat[i] = calloc(cols, sizeof(int));
    
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
void add_Dmat (struct Dmat *mat, int *elm)
{   
    if (mat->size >= mat->lines)
    {  
        struct Dmat *tmp = init_Dmat(mat->lines*2, mat->cols);
        copy_Dmat(mat, tmp);
        *mat = *tmp;
        free(tmp);
    }
    
    for(size_t i = 0; i < mat->cols; i++)
        mat->mat[mat->size][i] = elm[i];
    mat->size++;
}

static inline
void free_Dmat (struct Dmat *mat)
{
    for(size_t i = 0; i < mat->lines; i++)
        free(mat->mat[i]);
    free(mat->mat);
}

# endif
