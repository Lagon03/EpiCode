#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>

#define LENGTH(type) ((char*)(&type+1) - (char*)(&type))

/*Struct containing the exponential and logarithmic tables for faster computations*/
struct gf_tables {
  uint8_t *gf_exp;
  uint8_t *gf_log;
};

/*Struct used to store two numbers in a struct.*/
struct Tuple {
  uint8_t *x;
  uint8_t *y;
};


/*Precompute the logarithm and anti-log tables for faster computation later, using the provided primitive polynomial.*/
struct gf_tables* init_tables(uint16_t prim)
{
    struct gf_tables *gf_table = malloc(sizeof(struct gf_table*));
    prim = 0x11d;
    uint8_t *gf_expp = malloc(sizeof(uint8_t) * 512);
    uint8_t *gf_logg = malloc(sizeof(uint8_t) * 256);
    uint8_t x = 1;
    for(int i = 0; i < 256; i++){
        gf_expp[i] = x;
        gf_logg[x] = i;
        x <<= 1;
    }
    for(int i = 255; i < 512; i++){
        gf_expp[i] = gf_expp[i - 255];
    }
    gf_table->gf_exp = gf_expp;
    gf_table->gf_log = gf_logg;
    return gf_table;
}

uint8_t* merge(uint8_t *l1, uint8_t *l2, size_t s1, size_t s2);

uint8_t* reverse_arr(uint8_t *l);

uint8_t gf_pow(uint8_t x, uint8_t power, struct gf_tables *gf_table);

uint8_t gf_mul(uint8_t x, uint8_t y, struct gf_tables *gf_table);

uint8_t* gf_poly_scale(uint8_t *p, uint8_t x, struct gf_tables *gf_table);

uint8_t* gf_poly_add(uint8_t *p, uint8_t *q);

uint8_t* gf_poly_mul(uint8_t *p, uint8_t *q, struct gf_tables *gf_table);

uint8_t gf_poly_eval(uint8_t *p, uint8_t x, struct gf_tables *gf_table);

struct Tuple* gf_poly_div(uint8_t *dividend, uint8_t *divisor, struct gf_tables *gf_table);


