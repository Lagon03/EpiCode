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

struct gf_tables* init_tables();

uint8_t* merge(uint8_t *l1, uint8_t *l2, size_t s1, size_t s2);

uint8_t* reverse_arr(uint8_t *l);

uint8_t* copy_arr(uint8_t *l1, uint8_t *l2, size_t s);

uint8_t* pop_arr(uint8_t *l);

uint8_t* reverse_arr(uint8_t *l);

extern uint8_t gf_add(uint8_t x, uint8_t y);

extern uint8_t gf_sub(uint8_t x, uint8_t y);

uint8_t gf_pow(uint8_t x, uint8_t power, struct gf_tables *gf_table);

uint8_t gf_mul(uint8_t x, uint8_t y, struct gf_tables *gf_table);

uint8_t gf_inverse(uint8_t x, struct gf_tables *gf_table);

uint8_t gf_div(uint8_t x, uint8_t y, struct gf_tables *gf_table);

uint8_t* gf_poly_scale(uint8_t *p, uint8_t x, struct gf_tables *gf_table);

uint8_t* gf_poly_add(uint8_t *p, uint8_t *q);

uint8_t* gf_poly_mul(uint8_t *p, uint8_t *q, struct gf_tables *gf_table);

uint8_t gf_poly_eval(uint8_t *p, uint8_t x, struct gf_tables *gf_table);

struct Tuple* gf_poly_div(uint8_t *dividend, uint8_t *divisor, struct gf_tables *gf_table);


