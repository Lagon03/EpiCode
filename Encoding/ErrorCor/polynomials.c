# include <stdlib.h>
# include <stdio.h>

# include "../headers/polynomials.h"
# include "../headers/encode.h"

size_t gf_exp[512];
size_t gf_log[256];

const int _log[256] = { 1, 2, 4, 8, 16, 32, 64, 128, 29, 58, 116, 232, 205, 135, 19, 38, 76, 152, 45, 90, 180, 117, 234, 201, 143, 3, 6, 12, 24, 48, 96, 192, 157, 39, 78, 156, 37, 74, 148, 53, 106, 212, 181, 119, 238, 193, 159, 35, 70, 140, 5, 10, 20, 40, 80, 160, 93, 186, 105, 210, 185, 111, 222, 161, 95, 190, 97, 194, 153, 47, 94, 188, 101, 202, 137, 15, 30, 60, 120, 240, 253, 231, 211, 187, 107, 214, 177, 127, 254, 225, 223, 163, 91, 182, 113, 226, 217, 175, 67, 134, 17, 34, 68, 136, 13, 26, 52, 104, 208, 189, 103, 206, 129, 31, 62, 124, 248, 237, 199, 147, 59, 118, 236, 197, 151, 51, 102, 204, 133, 23, 46, 92, 184, 109, 218, 169, 79, 158, 33, 66, 132, 21, 42, 84, 168, 77, 154, 41, 82, 164, 85, 170, 73, 146, 57, 114, 228, 213, 183, 115, 230, 209, 191, 99, 198, 145, 63, 126, 252, 229, 215, 179, 123, 246, 241, 255, 227, 219, 171, 75, 150, 49, 98, 196, 149, 55, 110, 220, 165, 87, 174, 65, 130, 25, 50, 100, 200, 141, 7, 14, 28, 56, 112, 224, 221, 167, 83, 166, 81, 162, 89, 178, 121, 242, 249, 239, 195, 155, 43, 86, 172, 69, 138, 9, 18, 36, 72, 144, 61, 122, 244, 245, 247, 243, 251, 235, 203, 139, 11, 22, 44, 88, 176, 125, 250, 233, 207, 131, 27, 54, 108, 216, 173, 71, 142, 1 };

const int _antilog[256] = { -1, 0, 1, 25, 2, 50, 26, 198, 3, 223, 51, 238, 27, 104, 199, 75, 4, 100, 224, 14, 52, 141, 239, 129, 28, 193, 105, 248, 200, 8, 76, 113, 5, 138, 101, 47, 225, 36, 15, 33, 53, 147, 142, 218, 240, 18, 130, 69, 29, 181, 194, 125, 106, 39, 249, 185, 201, 154, 9, 120, 77, 228, 114, 166, 6, 191, 139, 98, 102, 221, 48, 253, 226, 152, 37, 179, 16, 145, 34, 136, 54, 208, 148, 206, 143, 150, 219, 189, 241, 210, 19, 92, 131, 56, 70, 64, 30, 66, 182, 163, 195, 72, 126, 110, 107, 58, 40, 84, 250, 133, 186, 61, 202, 94, 155, 159, 10, 21, 121, 43, 78, 212, 229, 172, 115, 243, 167, 87, 7, 112, 192, 247, 140, 128, 99, 13, 103, 74, 222, 237, 49, 197, 254, 24, 227, 165, 153, 119, 38, 184, 180, 124, 17, 68, 146, 217, 35, 32, 137, 46, 55, 63, 209, 91, 149, 188, 207, 205, 144, 135, 151, 178, 220, 252, 190, 97, 242, 86, 211, 171, 20, 42, 93, 158, 132, 60, 57, 83, 71, 109, 65, 162, 31, 45, 67, 216, 183, 123, 164, 118, 196, 23, 73, 236, 127, 12, 111, 246, 108, 161, 59, 82, 41, 157, 85, 170, 251, 96, 134, 177, 187, 204, 62, 90, 203, 89, 95, 176, 156, 169, 160, 81, 11, 245, 22, 235, 122, 117, 44, 215, 79, 174, 213, 233, 230, 231, 173, 232, 116, 214, 244, 234, 168, 80, 88, 175};

//=============================================================================
//                              Tools functions
//============================================================================

size_t poly_add(size_t elm1, size_t elm2) {
    return elm1 ^ elm2;
}

size_t poly_minus(size_t elm1, size_t elm2) {
    return elm1 ^ elm2;
}

void poly_mul_var(size_t elm, struct poly* polynome) {
    for(size_t ord = 0; ord < polynome->order; ++ord)
        polynome->var[ord] += elm;
}

struct poly* poly_mul(struct poly* poly1, struct poly* poly2) {
    struct poly* poly_ret = malloc(sizeof(struct poly));
    size_t max_ord = poly1->order + 1;
    if(poly1->order == poly2->order)
        poly_ret->order = poly1->order + 1;
    else {
        if (poly1->order > poly2->order)
            poly_ret->order = poly1 -> order;
        else {
            poly_ret->order = poly2 -> order;
            max_ord = poly2->order;
        }
    }

    poly_ret->coeff = calloc((poly_ret->order), sizeof(size_t));
    poly_ret->var = calloc((poly_ret->order), sizeof(size_t));


    size_t* coeff = calloc(max_ord, sizeof(size_t));
    for(size_t ord1 = 0; ord1 < poly1->order + 1; ++ord1)
        for(size_t ord2 = 0; ord2 < poly2->order + 1; ++ord2) {
            size_t _coeff = 0;   // tmp var
            size_t _var = 0;     // tmp var

            _coeff = _log[poly1->coeff[ord1] + poly2->coeff[ord2]];
            _var = poly1->var[ord1] + poly2->var[ord2];

            coeff[_var] += _coeff;
        }

    for(size_t i = 0; i < max_ord + 1; ++i)
    {
        printf("Coeff %li : %li\n", i, coeff[i]);
    }
    free(poly1);
    free(poly2);

    return poly_ret;
}

void poly_div(void) {
}

//===================================END=======================================

//=============================================================================
//                              Main functions
//=============================================================================

/*struct tables* initTables(size_t prim) {
    size_t gf_exp[512];
    size_t gf_log[256];

    size_t x = 1;
    for(size_t i = 0; i < 256; ++i) {
        gf_exp[i] = x; // compute antilog
        gf_log[x] = i; // compute log
        x =  5;
    }
}*/

struct poly* GenPolyFromCW(struct Block* codewords, size_t err_words) {
    struct poly* polynome = malloc(sizeof(struct poly));

    polynome->order = codewords->size;
    polynome->coeff = malloc((codewords->size - 1) * sizeof(size_t));
    polynome->var = malloc((codewords->size - 1) * sizeof(size_t));

    printf("Order is %li\n", polynome->order);

    printf("Polynomial of the block : \n");
    for(size_t i = 0; i < codewords->size; ++i) {
        polynome->coeff[i] = convertToDec(codewords->words[i]);
        polynome->var[i] = polynome->order - 1 - i;
        if (i < codewords->size - 1)
            printf("%lix^(%li) + ", polynome->coeff[i], polynome->var[i]);
        else
            printf("%li\n", polynome->coeff[i]);
    }

    poly_mul_var(err_words ,polynome);

    printf("Polynomial of the block after multiplication : \n");
    for(size_t i = 0; i < codewords->size; ++i) {
        if (i < codewords->size - 1)
            printf("%lix^(%li) + ", polynome->coeff[i], polynome->var[i]);
        else
            printf("%lix^(%li)\n\n", polynome->coeff[i], polynome->var[i]);
    }

    struct poly* p_gen = GenPolyG(1);

    printf("Polynomial Generator : \n");
    for(size_t i = 0; i < p_gen->order; ++i) {
        if (i < 2 - 1)
            printf("%lix^(%li) + ", p_gen->coeff[i], p_gen->var[i]);
        else
            printf("%lix^(%li)\n", p_gen->coeff[i], p_gen->var[i]);
    }

    return polynome;
}

static struct poly* initVarMul(size_t r) 
{
    struct poly* p = malloc(sizeof(struct poly));
    p->order = 1;
    p->coeff = malloc(2 * sizeof(size_t));
    p->var = malloc(2 * sizeof(size_t));

    p->coeff[0] = 0;
    p->var[0] = 1;

    p->coeff[1] = r;
    p->var[1] = 0;

    return p;
}

struct poly* GenPolyG(size_t err_words) {
    struct poly* p = malloc(sizeof(struct poly));
    p->order = 1;
    p->coeff = malloc(2 * sizeof(size_t));
    p->var = malloc(2 * sizeof(size_t));

    p->coeff[0] = 0;
    p->var[0] = 1;

    p->coeff[1] = 0;
    p->var[1] = 0;

    for(size_t r = 0; r < err_words; ++r) {
        struct poly* mul = initVarMul(r + 1);
        p = poly_mul(p, mul);
    }

    return p;
}

//=============================================================================
