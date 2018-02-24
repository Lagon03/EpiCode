# include <stdlib.h>
# include <stdio.h>
# include <math.h>

# include "../headers/polynomials.h"
# include "../headers/encode.h"

size_t gf_exp[512];
size_t gf_log[256];

const int _log[256] = {1, 2, 4, 8, 16, 32, 64, 128, 29, 58, 116, 232, 205, 135, 19, 38, 76, 152, 45, 90, 180, 117, 234, 201, 143, 3, 6, 12, 24, 48, 96, 192, 157, 39, 78, 156, 37, 74, 148, 53, 106, 212, 181, 119, 238, 193, 159, 35, 70, 140, 5, 10, 20, 40, 80, 160, 93, 186, 105, 210, 185, 111, 222, 161, 95, 190, 97, 194, 153, 47, 94, 188, 101, 202, 137, 15, 30, 60, 120, 240, 253, 231, 211, 187, 107, 214, 177, 127, 254, 225, 223, 163, 91, 182, 113, 226, 217, 175, 67, 134, 17, 34, 68, 136, 13, 26, 52, 104, 208, 189, 103, 206, 129, 31, 62, 124, 248, 237, 199, 147, 59, 118, 236, 197, 151, 51, 102, 204, 133, 23, 46, 92, 184, 109, 218, 169, 79, 158, 33, 66, 132, 21, 42, 84, 168, 77, 154, 41, 82, 164, 85, 170, 73, 146, 57, 114, 228, 213, 183, 115, 230, 209, 191, 99, 198, 145, 63, 126, 252, 229, 215, 179, 123, 246, 241, 255, 227, 219, 171, 75, 150, 49, 98, 196, 149, 55, 110, 220, 165, 87, 174, 65, 130, 25, 50, 100, 200, 141, 7, 14, 28, 56, 112, 224, 221, 167, 83, 166, 81, 162, 89, 178, 121, 242, 249, 239, 195, 155, 43, 86, 172, 69, 138, 9, 18, 36, 72, 144, 61, 122, 244, 245, 247, 243, 251, 235, 203, 139, 11, 22, 44, 88, 176, 125, 250, 233, 207, 131, 27, 54, 108, 216, 173, 71, 142, 1};

const int _antilog[256] = { -1, 0, 1, 25, 2, 50, 26, 198, 3, 223, 51, 238, 27, 104, 199, 75, 4, 100, 224, 14, 52, 141, 239, 129, 28, 193, 105, 248, 200, 8, 76, 113, 5, 138, 101, 47, 225, 36, 15, 33, 53, 147, 142, 218, 240, 18, 130, 69, 29, 181, 194, 125, 106, 39, 249, 185, 201, 154, 9, 120, 77, 228, 114, 166, 6, 191, 139, 98, 102, 221, 48, 253, 226, 152, 37, 179, 16, 145, 34, 136, 54, 208, 148, 206, 143, 150, 219, 189, 241, 210, 19, 92, 131, 56, 70, 64, 30, 66, 182, 163, 195, 72, 126, 110, 107, 58, 40, 84, 250, 133, 186, 61, 202, 94, 155, 159, 10, 21, 121, 43, 78, 212, 229, 172, 115, 243, 167, 87, 7, 112, 192, 247, 140, 128, 99, 13, 103, 74, 222, 237, 49, 197, 254, 24, 227, 165, 153, 119, 38, 184, 180, 124, 17, 68, 146, 217, 35, 32, 137, 46, 55, 63, 209, 91, 149, 188, 207, 205, 144, 135, 151, 178, 220, 252, 190, 97, 242, 86, 211, 171, 20, 42, 93, 158, 132, 60, 57, 83, 71, 109, 65, 162, 31, 45, 67, 216, 183, 123, 164, 118, 196, 23, 73, 236, 127, 12, 111, 246, 108, 161, 59, 82, 41, 157, 85, 170, 251, 96, 134, 177, 187, 204, 62, 90, 203, 89, 95, 176, 156, 169, 160, 81, 11, 245, 22, 235, 122, 117, 44, 215, 79, 174, 213, 233, 230, 231, 173, 232, 116, 214, 244, 234, 168, 80, 88, 175};

//=============================================================================
//                              Tools functions
//============================================================================

void print_poly(struct poly* polynome) {
    //printf("Order : %li\n", polynome->order);
    for(size_t i = 0; i < polynome->order + 1; ++i) {
        if(i < polynome->order)
            printf("%lix^(%li) + ", polynome->term[i].coeff, polynome->term[i].var);
        else
            printf("%lix^(%li)\n", polynome->term[i].coeff, polynome->term[i].var);
    }
    printf("\n");
}

void print_poly_a(struct poly* polynome) {
    //printf("Order : %li\n", polynome->order);
    for(size_t i = 0; i < polynome->order + 1; ++i) {
        if(i < polynome->order)
            printf("%ix^(%li) + ", _antilog[polynome->term[i].coeff], 
                    polynome->term[i].var);
        else
            printf("%ix^(%li)\n", _antilog[polynome->term[i].coeff], 
                    polynome->term[i].var);
    }
    printf("\n");
}

size_t poly_add(size_t elm1, size_t elm2) {
    return elm1 ^ elm2;
}

size_t poly_minus(size_t elm1, size_t elm2) {
    return elm1 ^ elm2;
}

size_t p_xor(size_t x, size_t y) {
    char* x_byte = convertToByte(x);
    x_byte = adjustSize(x_byte, 8);
    char* y_byte = convertToByte(y);
    y_byte = adjustSize(y_byte, 8);

    char* xy_xor = calloc(8, sizeof(char));
    for(size_t i = 0; i < 8; ++i) {
        if(x_byte[i] == y_byte[i])
            xy_xor[i] = '0';
        else
            xy_xor[i] = '1';
    }
    free(x_byte);
    free(y_byte);
    size_t xored = convertToDec(xy_xor);
    free(xy_xor);

    return xored;
}

void poly_mul_var(size_t elm, struct poly* polynome) {
    for(size_t ord = 0; ord < polynome->order + 1; ++ord)
        polynome->term[ord].var += elm;
}

struct poly* poly_mul(struct poly* poly1, struct poly* poly2) {
    struct poly* p_mul = malloc(sizeof(struct poly));
    p_mul->order = poly1->order + poly2->order;
    p_mul->term = malloc((p_mul->order + 1) * sizeof(struct term));
    for(size_t i = 0; i < p_mul->order + 1; ++i) {
        p_mul->term[i].coeff = 0;
        p_mul->term[i].var = p_mul->order - i;
    }

    // to multiply (a0x1 + a0x0) by (a0x1 + a1x0)
    // 1 - (a0x1 * a0x1) + (a0x1 * a1x0) + (a0x0 * a0x1) + (a0x0 * a1x0)
    // 2 - ((a0)x2) + (a1x1) + (a0x1) + ((a1)x0)
    // 3 - a0x2 + (a1+a0)x1 + a1x0

    printf("\nPoly1 : ");
    print_poly_a(poly1);
    printf("\nPoly2 : ");
    print_poly_a(poly2);
    printf("\n");

    for(size_t i = 0; i < poly1->order + 1; ++i) {
        for(size_t j = 0; j < poly2->order + 1; ++j) {
            // the coeff are the exponent of alpha which correspond to the log
            // table. Two exponents of the same stuff should add
            //
            size_t old = p_mul->term[i + j].coeff;
            printf("Old coeff : %li\n", old);

            size_t n_coeff = (_antilog[poly1->term[i].coeff] + _antilog[poly2->term[j].coeff]);
            printf("Operation : (%li + %li) + %li\n", 
                    poly1->term[i].coeff,
                    poly2->term[j].coeff,
                    old);
            printf("n_coeff : %i\n", n_coeff);
            if(n_coeff < 256) {
                n_coeff = _log[_antilog[poly1->term[i].coeff]
                    + _antilog[poly2->term[j].coeff]];
            }
            else
            {
                size_t adjust = ((n_coeff % 256) / floor(n_coeff / 256));
                n_coeff = _log[adjust];
            }
            p_mul->term[i + j].coeff = p_xor(old, n_coeff);
            printf("\tNew coeff : %i | %li\n", _antilog[p_mul->term[i + j].coeff], p_mul->order - (i + j));


            if(p_mul->term[i + j].coeff > 255) {
                size_t coeff = p_mul->term[i + j].coeff;
                printf("Coeff : %li\n", coeff);
                p_mul->term[i + j].coeff = ((coeff % 256) / floor(coeff / 256));
                coeff = p_mul->term[i + j].coeff;
                printf("Coeff : %li\n", coeff);
            }
        }
    }

    /*for(size_t i = 0; i < p_mul->order + 1; ++i)
      p_mul->term[i].coeff = _antilog[p_mul->term[i].coeff];
      printf("\n");*/
    return p_mul;
}

void poly_div(void) {
}

//===================================END=======================================

//=============================================================================
//                              Main functions
//=============================================================================

struct poly* GenPolyFromCW(struct Block* codewords, size_t err_words) {
    struct poly* polynome = malloc(sizeof(struct poly));

    polynome->order = codewords->size - 1;
    polynome->term = malloc(codewords->size * sizeof(struct term));

    printf("Order is %li\n", polynome->order);

    printf("Polynomial of the block : \n");
    for(size_t i = 0; i < codewords->size; ++i) {
        polynome->term[i].coeff = convertToDec(codewords->words[i]);
        polynome->term[i].var = polynome->order - i;
        if (i < codewords->size - 1)
            printf("%lix^(%li) + ", polynome->term[i].coeff, polynome->term[i].var);
        else
            printf("%lix^(%li)\n", polynome->term[i].coeff, polynome->term[i].var);
    }

    poly_mul_var(err_words ,polynome);

    printf("Polynomial of the block after multiplication : \n");
    print_poly(polynome); 

    struct poly* p_gen = GenPolyG(8);

    printf("Polynomial Generator : \n");
    print_poly_a(p_gen);

    return polynome;
}

static struct poly* initVarMul(size_t r) 
{
    struct poly* p = malloc(sizeof(struct poly));
    p->order = 1;
    p->term = malloc(2 * sizeof(struct term));

    p->term[0].coeff = _log[0];
    p->term[0].var = 1;

    p->term[1].coeff = _log[r];
    p->term[1].var = 0;

    return p;
}

struct poly* GenPolyG(size_t err_words) {
    struct poly* p = malloc(sizeof(struct poly));
    p->order = 1;
    p->term = malloc(2 * sizeof(struct term));
    p->term[0].coeff = _log[0];
    p->term[0].var = 1;

    p->term[1].coeff = _log[0];
    p->term[1].var = 0;

    for(size_t r = 0; r < err_words; ++r) {
        printf("Generator multiplication %li:\n", r);
        struct poly* mul = initVarMul(r + 1);
        p = poly_mul(p, mul);
        print_poly_a(p);
    }

    return p;
}

//=============================================================================
