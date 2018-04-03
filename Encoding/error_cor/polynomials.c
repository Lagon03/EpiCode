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
        if(polynome->term[i].coeff != 999 && polynome->term[i].coeff != 0) {
            if(i < polynome->order)
                printf("%lix^(%li) + ", polynome->term[i].coeff, polynome->term[i].var);
            else
                printf("%lix^(%li)\n", polynome->term[i].coeff, polynome->term[i].var);
            //printf("o(%li)", i);
        }
    }
    printf("\n");
}

void print_poly_a(struct poly* polynome) {
    //printf("Order : %li\n", polynome->order);
    for(size_t i = 0; i < polynome->order + 1; ++i) {
        if(polynome->term[i].coeff != 999) {
            if(i < polynome->order)
                printf("%ix^(%li) + ", _antilog[polynome->term[i].coeff], 
                        polynome->term[i].var);
            else
                printf("%ix^(%li)\n", _antilog[polynome->term[i].coeff], 
                        polynome->term[i].var);
        }
    }
    printf("\n");
}

void change_order(struct poly* p, size_t n_order) {
    size_t old_order = p->order;
    p->order = n_order;
    p->term = realloc(p->term, (n_order + 1) * sizeof(struct term));
    for(size_t o = 0; o < n_order + 1; ++o) {
        if(o <= old_order)
            p->term[o].var = n_order - o;
        else {
            p->term[o].var = n_order - o;
            p->term[o].coeff = 999;
        }
    }
}

size_t poly_add(size_t elm1, size_t elm2) {
    return elm1 ^ elm2;
}

size_t poly_minus(size_t elm1, size_t elm2) {
    return elm1 ^ elm2;
}

size_t p_xor(size_t x, size_t y) {
    //if (x != 0 && y != 0)
    //  printf("######## xor of %li by %li\n", y, x);
    char* x_byte = convertToByte(x);
    x_byte = adjustSize(x_byte, 8);
    char* y_byte = convertToByte(y);
    y_byte = adjustSize(y_byte, 8);
    /*printf("######## x : %s = %li\n", x_byte, convertToDec(x_byte));
      printf("######## y : %s = %li\n", y_byte, convertToDec(y_byte));*/

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
    //printf("####### xor result : %s\n", xy_xor);
    //printf("####### xor result : %li\n", convertToDec(xy_xor));
    free(xy_xor);

    return xored;
}

struct poly* merge(struct poly* p1, struct poly* p2, size_t start) {
    struct poly* merged = malloc(sizeof(struct poly));
    merged->order = p2->order;
    merged->term = malloc((merged->order + 1) * sizeof(struct term));
    for(size_t i = start; i < merged->order + 1; ++i) {
        if(p2->term[i].coeff != 0)
            merged->term[i].coeff = p2->term[i].coeff;
        else if(i <= p1->order)
            merged->term[i].coeff = p1->term[i].coeff;
        merged->term[i].var = p2->term[i].var;
    }
    return merged;
}

void poly_mul_coeff(size_t elm, struct poly* polynome, size_t start) {
    for(size_t ord = start; ord < polynome->order + 1; ++ord) {
        size_t coeff = polynome->term[ord].coeff;
        if(coeff != 999) {
            //printf("-----%li by %li\n", coeff, elm);
            //printf("-----%li + %li\n", _antilog[coeff], _antilog[elm]);
            coeff = _antilog[coeff] + _antilog[elm];
            polynome->term[ord].coeff = _log[coeff];
            if(coeff > 255)
                polynome->term[ord].coeff = _log[coeff % 255];
        }
    }
}

void poly_mul_var_down(size_t elm, struct poly* polynome) {
    for(size_t ord = 0; ord < polynome->order + 1; ++ord)
        polynome->term[ord].var -= elm;
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

    for(size_t i = 0; i < poly1->order + 1; ++i) {
        for(size_t j = 0; j < poly2->order + 1; ++j) {
            // the coeff are the exponent of alpha which correspond to the log
            // table. Two exponents of the same stuff should add
            //
            size_t old = p_mul->term[i + j].coeff;

            size_t n_coeff = (_antilog[poly1->term[i].coeff] + _antilog[poly2->term[j].coeff]);

            if(n_coeff < 256) {
                n_coeff = _log[_antilog[poly1->term[i].coeff]
                    + _antilog[poly2->term[j].coeff]];
            }
            else
            {
                size_t adjust = ((n_coeff % 255) / floor(n_coeff / 255));
                n_coeff = _log[adjust];
            }
            p_mul->term[i + j].coeff = p_xor(old, n_coeff);

            if(p_mul->term[i + j].coeff > 255) {
                size_t coeff = p_mul->term[i + j].coeff;
                p_mul->term[i + j].coeff = ((coeff % 256) / floor(coeff / 256));
                coeff = p_mul->term[i + j].coeff;
            }
        }
    }

    return p_mul;
}

void poly_div(void) {
}

void checkPoly(struct poly* p)
{
    size_t k = 0;
    size_t* pos = malloc(sizeof(size_t));
    for(size_t o = 0; o < p->order; ++o)
    {
        if(p->term[o].coeff == 0)
        {
            k += 1;
            pos = realloc(pos, (k + 1) * sizeof(size_t));
            pos[k - 1] = o; 
        }
    }
    printf("\nk = %li\n", k);
    for(size_t x = 0; x < k; ++x)
    {
        size_t nxt = 0;
        size_t u = 0;
        for(size_t start = pos[x]; start > 0 ; --start)
        {
            p->term[start].coeff = p->term[start - 1].coeff;
        }
        p->term[u].coeff = 0;
        p->term[k].coeff = 0;
        size_t l = pos[k - 1];
    }
}

//===================================END=======================================

//=============================================================================
//                              Main functions
//=============================================================================

struct poly* initTmpP(size_t order) {
    struct poly* polynome = malloc(sizeof(struct poly));

    polynome->order = order;
    polynome->term = malloc((order + 1) * sizeof(struct term));

    for(size_t i = 0; i < order; ++i) {
        polynome->term[i].coeff = 0;
        polynome->term[i].var = polynome->order - i;
    }
    return polynome;
}


size_t* GenPolyFromCW(struct Block* codewords, size_t err_words) {
    struct poly* polynome = malloc(sizeof(struct poly));

    polynome->order = codewords->size - 1;
    polynome->term = malloc(codewords->size * sizeof(struct term));

    printf("\n\nMessage polynomial : \n");
    for(size_t i = 0; i < codewords->size; ++i) {
        printf("%s ", codewords->words[i]);
        polynome->term[i].coeff = convertToDec(codewords->words[i]);
        polynome->term[i].var = polynome->order - i;
        /*if (i < codewords->size - 1)
          printf("%lix^(%li) + ", polynome->term[i].coeff, polynome->term[i].var);
          else
          printf("%lix^(%li)\n", polynome->term[i].coeff, polynome->term[i].var);*/
    }
    //printf("\n\n");

    checkPoly(polynome);
    printf("We need %li codewords.\n", err_words);
    print_poly(polynome);
    poly_mul_var(err_words , polynome);
    change_order(polynome, polynome->term[0].var);

    printf("Message polynomial after multiplication : \n");
    print_poly(polynome);
    printf("\n");

    struct poly* p_gen = GenPolyG(err_words - 1);

    /*printf("Polynomial Generator : \n");
      print_poly_a(p_gen);*/

    //printf("Polynomial Generator after multiplication: \n");
    poly_mul_var(polynome->term[0].var - p_gen->term[0].var, p_gen);
    //print_poly_a(p_gen);
    //poly_mul_coeff(0, p_gen, 0);
    poly_mul_coeff(polynome->term[0].coeff, p_gen, 0);
    change_order(p_gen ,p_gen->term[0].var);

    printf("\n");
    //print_poly_a(p_gen);
    print_poly(p_gen);


    struct poly* fix_gen = GenPolyG(err_words - 1); // This poly MUST not be
    // modified
    poly_mul_var(polynome->term[0].var - fix_gen->term[0].var, fix_gen);


    struct poly* initial_gen = GenPolyG(err_words -1);
    poly_mul_var(polynome->term[0].var - initial_gen->term[0].var, initial_gen);
    //poly_mul_coeff(polynome->term[0].coeff, initial_gen, 0);
    //print_poly(initial_gen);

    struct poly* xor = initTmpP(p_gen->order);

    printf("\n");
    for(size_t i = 0; i < codewords->size; ++i) {
        if (i == 0) 
        { // step 1 : special treatment with the message poly
            for(size_t o_g = 0; o_g < p_gen->order + 1; ++o_g)
            {
                size_t t_gen = p_gen->term[o_g].coeff;
                size_t p_term = polynome->term[o_g].coeff;
                //printf("Term of p_gen : %li\n", p_gen->term[o_g].coeff);
                if(t_gen != 999 && p_term != 999)
                    xor->term[o_g].coeff = p_xor(t_gen,p_term);
                else if(t_gen == 999 && p_term != 999)
                    xor->term[o_g].coeff = p_xor(0, p_term);
                else if(t_gen != 999 && p_term == 999)
                    xor->term[o_g].coeff = p_xor(t_gen, 0);
            }

            //xor = merge(polynome, xor, i + 1);

            poly_mul_var_down(1, initial_gen);
            poly_mul_var_down(1, fix_gen);
            poly_mul_coeff(xor->term[1].coeff, initial_gen, i);
        }
        else 
        {
            if(xor->term[i].coeff != 0)
            {
                // general case for step > 1
                //printf("\t Step %li\n", i + 1);
                for(size_t o_g = 0; o_g < fix_gen->order + 1; ++o_g)
                {
                    size_t t_gen = initial_gen->term[o_g].coeff;
                    size_t p_term = xor->term[o_g + i].coeff;
                    if(t_gen != 999 && p_term != 999)
                        xor->term[o_g + i].coeff = p_xor(t_gen, p_term);
                    else if(t_gen == 999 && p_term != 999)
                        xor->term[o_g + i].coeff = p_xor(0, p_term);
                    else if(t_gen != 999 && p_term == 999)
                        xor->term[o_g + i].coeff = p_xor(t_gen, 0);
                }
            }
            poly_mul_var_down(1, fix_gen);  
            // we need to lower the power of
            // the fixed generator
            size_t coeff_lead = xor->term[i + 1].coeff;
            for(size_t k = 0; k < fix_gen->order + 1; ++k)
            {
                size_t coeff = fix_gen->term[k].coeff;
                if(coeff != 999) 
                {
                    coeff = _antilog[coeff] + _antilog[coeff_lead];
                    if(coeff > 255)
                        initial_gen->term[k].coeff = _log[coeff % 255];
                    else
                        initial_gen->term[k].coeff = _log[coeff];
                }
            }
            //poly_mul_coeff(xor->term[i + 1].coeff, initial_gen, i);
        }
        printf("\n Xor %li result:\n", i + 1);
        print_poly(xor);
        printf("\n");
        printf("\n Generator polynomial :\n");
        print_poly(initial_gen);
        printf("\n");
    }
    //free(xor);

    printf("\nFinal Xor Polynomial : \n");
    print_poly(xor);
    printf("\n");

    size_t* err_cw = malloc(err_words * sizeof(size_t));
    for(size_t c = 0; c < err_words; ++c)
        err_cw[err_words - 1 - c] = xor->term[xor->order - c].coeff;
    free(xor);
    free(initial_gen);
    free(polynome);
    free(fix_gen);

    /*for(size_t i = 0; i < err_words; ++i)
      printf("%li ", err_cw[i]);
      printf("\n");*/

    return err_cw;
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
        //printf("Generator multiplication %li:\n", r);
        struct poly* mul = initVarMul(r + 1);
        p = poly_mul(p, mul);
        //print_poly_a(p);
    }

    return p;
}

//=============================================================================
