# ifndef POLYNOMIALS_H
# define POLYNOMIALS_H

# include "../headers/encode.h"

struct poly {
    size_t order; // order of the polynomial
    struct term* term; // coeff and order composing the polynomial
};

struct term {
    size_t coeff;
    size_t var;
};

extern const int _log[256];
extern const int _antilog[256];

//=============================================================================
//                              Tool functions
//=============================================================================

// TODO : add variable and return type
size_t poly_add(size_t elm1, size_t elm2);
size_t poly_minus(size_t elm1, size_t elm2);
size_t p_xor(size_t x, size_t y);
void poly_mul_var(size_t elm, struct poly* polynome);
struct poly* poly_mul(struct poly* poly1, struct poly* poly2);
void poly_div(void);

//====================================END======================================

//=============================================================================
//                              Main functions
//=============================================================================

// GenPolyCW:   generate a polynomial corresponding to the given set
//              of codeword
size_t* GenPolyFromCW(struct Block* codewords, size_t err_words);
// GenPolyG :   generate a generator polynomial used to divide the polynomial
//              of the codeword set
struct poly* GenPolyG(size_t order /* and probably some other var */);

//====================================END======================================

# endif
