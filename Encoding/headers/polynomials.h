# ifndef POLYNOMIALS_H
# define POLYNOMIALS_H

# include "../headers/encode.h"

struct poly {
    size_t order;
    size_t* coeff;
};


//=============================================================================
//                              Tool functions
//=============================================================================

// TODO : add variable and return type
void poly_add(void);
void poly_minus(void);
void poly_mul(void);
void poly_div(void);

//====================================END======================================

//=============================================================================
//                              Main functions
//=============================================================================

void init_tables(size_t prim);
// GenPolyCW:   generate a polynomial corresponding to the given set
//              of codeword
struct poly* GenPolyCW(struct Codewords);
// GenPolyG :   generate a generator polynomial used to divide the polynomial
//              of the codeword set
struct poly* GenPolyG(size_t order /* and probably some other var */);

//====================================END======================================

# endif
