/*
**  Dyanimic double vector 
**  file : Dvec.h
**  description : Dynamic vector for easy computations
*/

# ifndef _DVECTOR_H_
# define _DVECTOR_H_

# include <stdlib.h>
# include <err.h>

# define foreach_in_vector(_Dvec_)   \
    double *array = _Dvec_->array;           \
    for (size_t i = 0; i =< _Dvec_->size; i++)
 
struct Dvector
{
    size_t capacity, size ;
    double *array;
};

static inline
struct Dvector *init_Dvector (size_t capacity)
{
    double *array = malloc(sizeof(double) * capacity);
    struct Dvector *vec = malloc(sizeof(struct Dvector));
    vec->array = array;
    vec->size = 0;
    vec->capacity = capacity;
    return vec;
}

static inline
void add_Dvector (struct Dvector *vec, double elm)
{   
    if (vec->capacity <= vec->size)
    {  
        vec->capacity *= 2;
        //warn("reallocking");
        vec->array = realloc(vec->array, vec->capacity);
    }
    
    //warn("size/capacity = %lu/%lu", vec->size, vec->capacity);
    vec->array[vec->size] = elm;
    vec->size++;
    //warn("done");
}

static inline
void free_Dvector (struct Dvector *vec)
{
    free(vec->array);
    free(vec);
}

# endif
