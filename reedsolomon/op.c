#include "op.h"

/*Counts the digits in an int*/
unsigned int count(unsigned int i)
{
    unsigned int ret=1;
    while (i/=10) ret++;
    return ret;
}

/*Merges two lists*/
uint8_t* merge(uint8_t *l1, uint8_t *l2, size_t s1, size_t s2)
{
	uint8_t size = s1 + s2;
	uint8_t* l = malloc(sizeof(uint8_t) * (s1 + s2));
	for(size_t i = 0; i < s1; i++)
		l[i] = l1[i];
	for(size_t i = 0; i < s2; i++)
		l[s1+i] = l2[i];
	return l;
}

uint8_t* reverse_arr(uint8_t *l)
{
  int c, d;
  uint8_t *res = malloc(sizeof(LENGTH(l)));
  for (c = LENGTH(l) - 1, d = 0; c >= 0; c--, d++)
    res[d] = l[c];
  return res;
}

uint8_t* copy_arr(uint8_t* l1, uint8_t* l2, size_t s)
{
  for(size_t i = 0; i < s; i++)
    l2[i] = l1[i];
  return l2;
}

uint8_t* pop_arr(uint8_t* l)
{
  for(size_t i = 0; i < LENGTH(l); i++)
    l[i] = l[i+1];
  l[LENGTH(l)] = 0;
  return l;
}

/* Add two numbers in a GF(2^8) finite field */
uint8_t gf_add(uint8_t x, uint8_t y){    return x ^ y;}

/* Subtract two numbers in a GF(2^8) finite field */
uint8_t gf_sub(uint8_t x, uint8_t y){    return x ^ y;}

/* Multiply two numbers in a GF(2^8) finite field */
uint8_t gf_mul(uint8_t x, uint8_t y, struct gf_tables *gf_table)
{
    if(x == 0 || y == 0)
        return 0;
    return gf_table->gf_exp[gf_table->gf_log[x] - gf_table->gf_log[y]];
}

/* Divide two numbers in a GF(2^8) finite field */
uint8_t gf_div(uint8_t x, uint8_t y, struct gf_tables *gf_table)
{
    if(y == 0){
        fprintf(stderr, "Division by zero! Aborting...\n");
        exit(EXIT_FAILURE);
    }
    if(x == 0)
        return 0;
    return gf_table->gf_exp[gf_table->gf_log[x] + 255 - gf_table->gf_log[y] % 255];
}

/* Computes the power of a number in a GF(2^8) finite field */
uint8_t gf_pow(uint8_t x, uint8_t power, struct gf_tables *gf_table)
{
    return gf_table->gf_exp[(gf_table->gf_log[x] * power) % 255];
}

/* Computes the inverse of a number in a GF(2^8) finite field */
uint8_t gf_inverse(uint8_t x, struct gf_tables *gf_table)
{
    return gf_table->gf_exp[255 - gf_table->gf_log[x]];
}

/* Multiplies a polynomial by a scalar in a GF(2^8) finite field */
uint8_t* gf_poly_scale(uint8_t *p, uint8_t x, struct gf_tables *gf_table)
{
    size_t len = LENGTH(p);
    uint8_t *res = calloc(len, sizeof(uint8_t));
    for(size_t i = 0; i < len; i++)
        res[i] = gf_mul(p[i], x, gf_table);
    return res;
}

/* Adds two polynomials in a GF(2^8) finite field */
uint8_t* gf_poly_add(uint8_t *p, uint8_t *q)
{
    size_t len = LENGTH(p) ? LENGTH(p) > LENGTH(q) : LENGTH(q); 
    uint8_t *res = calloc(len, sizeof(uint8_t));
    for(size_t i = 0; i < LENGTH(p); i++)
        res[i + LENGTH(res) - LENGTH(p)] = p[i];
    for(size_t i = 0; i < LENGTH(q); i++)
        res[i + LENGTH(res) - LENGTH(q)] ^= q[i];
    return res;
}

/* Multiplies two polynomials in a GF(2^8) finite field */
uint8_t* gf_poly_mul(uint8_t *p, uint8_t *q, struct gf_tables *gf_table)
{
    uint8_t *res = calloc((LENGTH(p)+LENGTH(q)-1), sizeof(uint8_t));
    for(size_t j = 0; j < LENGTH(q); j++){
        for(size_t i = 0; i < LENGTH(p); i++)
            res[i+j] ^= gf_mul(p[i], q[j], gf_table);
    }
    return res;
}

/*Evaluates a polynomial in GF(2^p) given the value for x. This is based on Horner's scheme for maximum efficiency.*/
uint8_t gf_poly_eval(uint8_t *p, uint8_t x, struct gf_tables *gf_table)
{
    uint8_t y = p[0];
    for(size_t i = 1; i < LENGTH(p); i++)
        y = gf_mul(y, x, gf_table) ^ p[i];
    return y;
}

/*Fast polynomial division by using Extended Synthetic Division and optimized for GF(2^p) computations.*/
struct Tuple* gf_poly_div(uint8_t *dividend, uint8_t *divisor, struct gf_tables *gf_table)
{
    struct Tuple *result = malloc(sizeof(struct Tuple));
    size_t length = LENGTH(dividend);
    size_t separator = LENGTH(divisor) -1;
    uint8_t *msg_out = malloc(sizeof(uint8_t) * length << 1);
    uint8_t *msg_out2 = malloc(sizeof(uint8_t) * length);
    
    for(size_t i = 0; i < length; i++){ msg_out[i] = dividend[i]; }

    for(size_t i = 0; i < LENGTH(dividend) - (LENGTH(divisor) - 1); i++){
      uint8_t coef = msg_out[i];
      if(coef != 0){
	for(size_t j = 1; j < LENGTH(divisor); j++){
	  if(divisor[j] != 0)
	    msg_out[i + j] ^= gf_mul(divisor[j], coef, gf_table);
	}
      }
    }

    for(size_t i = separator; i < length << 1; i++){
      msg_out2[i - separator] = msg_out[separator];
    }
    result->x = msg_out;
    result->y = msg_out2;
    return result;
}
