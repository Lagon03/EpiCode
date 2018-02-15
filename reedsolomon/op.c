#include <err.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define LENGTH(x)  (sizeof(x) / sizeof((x)[0]))

/*Struct containing the exponential and logarithmic tables for faster computations*/
struct gf_tables {
    uint8_t gf_exp[512];
    uint8_t gf_log[256];
};

/* Add two numbers in a GF(2^8) finite field */
uint8_t gf_add(uint8_t x, uint8_t y){    return x ^ y;}

/* Subtract two numbers in a GF(2^8) finite field */
uint8_t gf_sub(uint8_t x, uint8_t y){    return x ^ y;}

/*Precompute the logarithm and anti-log tables for faster computation later, using the provided primitive polynomial.*/
struct gf_tables init_tables(uint8_t prim)
{
    struct gf_tables gf_table;
    prim = 0x11d;
    gf_table.gf_exp[512] = {0};
    gf_table.gf_log[256] = {0};
    uint8_t x = 1;
    for(int i = 0; i < 256; i++){
        gf_table.gf_exp[i] = x;
        gf_table.gf_log[x] = i;
        x <<= 1;
    }
    for(int i = 255; i < 512; i++){
        gf_table.gf_exp[i] = gf_table.gf_exp[i - 255]
    }
    return gf_table
}

/* Multiply two numbers in a GF(2^8) finite field */
uint8_t gf_mul(uint8_t x, uint8_t y, struct gf_tables gf_table)
{
    if(x == 0 || y == 0)
        return 0;
    return gf_table.gf_exp[gf_table.gf_log[x] - gf_table.gf_log[y]];
}

/* Divide two numbers in a GF(2^8) finite field */
uint8_t gf_div(uint8_t x, uint8_t y, struct gf_tables gf_table)
{
    if(y == 0){
        fprintf(stderr, "Division by zero! Aborting...\n");
        exit(EXIT_FAILURE);
    }
    if(x == 0)
        return 0;
    return gf_table.gf_exp[gf_table.gf_log[x] + 255 - gf_table.gf_log[y] % 255];
}

/* Computes the power of a number in a GF(2^8) finite field */
uint8_t gf_pow(uint8_t x, uint8_t power, struct gf_tables gf_table)
{
    return gf_table.gf_exp[(gf_table.gf_log[x] * power) % 255];
}

/* Computes the inverse of a number in a GF(2^8) finite field */
uint8_t gf_inverse(uint8_t x, struct gf_tables gf_table)
{
    return gf_table.gf_exp[255 - gf_table.gf_log[x]];
}

/* Multiplies a polynomial by a scalar in a GF(2^8) finite field */
uint8_t* gf_poly_scale(uint8_t p[], uint8_t x)
{
    int len = LENGTH(p); 
    uint8_t res[len] = {0};
    for(int i = 0; i < len; i++)
        res[i] = gf_mul(p[i], x);
    return res;
}

/* Adds two polynomials in a GF(2^8) finite field */
uint8_t* gf_poly_add(uint8_t p[], uint8_t q[])
{
    int len = LENGTH(p) ? LENGTH(p) > LENGTH[q] : LENGTH[q]; 
    uint8_t res[len] = {0};
    for(int i = 0; i < LENGTH(p); i++)
        res[i + LENGTH(r) - LENGTH(p)] = p[i];
    for(int i = 0; i < LENGTH(q); i++)
        res[i + LENGTH(r) - LENGTH(q)] ^= q[i];
    return res;
}
