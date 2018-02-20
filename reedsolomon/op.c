#include "op.h"

/*Counts the digits in an int*/
unsigned int count(unsigned int i)
{
    unsigned int ret=1;
    while (i/=10) ret++;
    return ret;
}

/*Merges two lists*/
struct Array merge(struct Array *l1, struct Array *l2)
{
  uint8_t size = l1->size + l2->size;
  struct Array *l = malloc(sizeof(struct Array*));
  initArray(l, size);
  for(size_t i = 0; i < l1->size; i++)
    l[i] = l1[i];
  for(size_t i = 0; i < l2->size; i++)
    l[l1->size+i] = l2[i];
  return l;
}

struct Array* reverse_arr(struct Array *l)
{
  int c, d;
  struct Array *res = malloc(sizeof(struct Array*));
  initArray(res, l->size);
  for (c = l->size - 1, d = 0; c >= 0; c--, d++)
    res[d] = l[c];
  return res;
}

struct Array* copy_arr(struct Array *l1, struct Array *l2)
{
  initArray(l2, l1->size);
  for(size_t i = 0; i < l1->size; i++)
    l2[i] = l1[i];
  return l2;
}

struct Array* pop_arr(struct Array *l)
{
  for(size_t i = 0; i < l->used; i++)
    l[i] = l[i+1];
  l[l->used] = 0;
  l->used -= 1;
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
    return gf_table->gf_exp->array[gf_table->gf_log->array[x] - gf_table->gf_log->array[y]];
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
    return gf_table->gf_exp->array[gf_table->gf_log->array[x] + 255 - gf_table->gf_log->array[y] % 255];
}

/* Computes the power of a number in a GF(2^8) finite field */
uint8_t gf_pow(uint8_t x, uint8_t power, struct gf_tables *gf_table)
{
    return gf_table->gf_exp[(gf_table->gf_log[x]->array * power) % 255];
}

/* Computes the inverse of a number in a GF(2^8) finite field */
uint8_t gf_inverse(uint8_t x, struct gf_tables *gf_table)
{
    return gf_table->gf_exp->array[255 - gf_table->gf_log->array[x]];
}

/*Precompute the logarithm and anti-log tables for faster computation later, using the provided primitive polynomial.*/
struct gf_tables* init_tables()
{
  struct gf_tables *gf_table = malloc(sizeof(struct gf_table*));
  struct Array *gf_expp = malloc(sizeof(struct Array*));
  struct Array *gf_logg = malloc(sizeof(struct Array*));
  initArray(gf_expp, 512);
  initArray(gf_logg, 256);
  uint32_t x = 1;
  uint32_t prim = 0x11d;
  for(int i = 0; i < 256; i++){
    gf_expp->array[i] = x;
    insertArray(gf_expp);
    gf_logg->array[x] = i;
    insertArray(gf_logg);
    x <<= 1;
    if(x & 0x100)
      x ^= prim;
  }
  for(int i = 255; i < 512; i++){
    gf_expp->array[i] = gf_expp->array[i - 255];
    insertArray(gf_expp);
  }
  gf_table->gf_exp = gf_expp;
  gf_table->gf_log = gf_logg;
  return gf_table;
}

/* Multiplies a polynomial by a scalar in a GF(2^8) finite field */
struct Array* gf_poly_scale(struct Array *p, uint8_t x, struct gf_tables *gf_table)
{
  size_t len = p->used;
  struct Array *res = malloc(sizeof(struct Array*));
	initZArray(res, len);
  for(size_t i = 0; i < len; i++){
    res->array[i] = gf_mul(p->array[i], x, gf_table);
    insertArray(res);
  }
  return res;
}

/* Adds two polynomials in a GF(2^8) finite field */
struct Array* gf_poly_add(struct Array *p, struct Array *q)
{
  size_t len = p->used ? p->used > q->used : q->used; 
  struct Array *res = malloc(sizeof(struct Array*));
	initZArray(res, len);
  for(size_t i = 0; i < p->used; i++){
	  res->array[i + res->used - p->used] = p->array[i];
    insertArray(res);
	}
  for(size_t i = 0; i < q->used; i++){
    res[i + res->used - q->used] ^= q->array[i];
	  insertArray(res);
	}
  return res;
}

/* Multiplies two polynomials in a GF(2^8) finite field */
struct Array* gf_poly_mul(struct Array *p, struct Array *q, struct gf_tables *gf_table)
{
  struct Array *res = malloc(sizeof(struct Array*));
  initZArray(res, p->size + q->size + 1);
  for(size_t j = 0; j < q->used; j++){
      for(size_t i = 0; i < p->used; i++){
        res->array[i+j] ^= gf_mul(p->array[i], q->array[j], gf_table);
        insertArray(res);
      }
  }
  return res;
  }

/*Evaluates a polynomial in GF(2^p) given the value for x. This is based on Horner's scheme for maximum efficiency.*/
uint8_t gf_poly_eval(struct Array *p, uint8_t x, struct gf_tables *gf_table)
{
    uint8_t y = p->array[0];
    for(size_t i = 1; i < p->used; i++)
        y = gf_mul(y, x, gf_table) ^ p->array[i];
    return y;
}

/*Fast polynomial division by using Extended Synthetic Division and optimized for GF(2^p) computations.*/
struct Tuple* gf_poly_div(struct Array *dividend, struct Array *divisor, struct gf_tables *gf_table)
{
  struct Tuple *result = malloc(sizeof(struct Tuple));
  size_t length = dividend->used;
  size_t separator = divisor->used -1;
  struct Array *msg_out = malloc(sizeof(struct Array*));
  initArray(msg_out, length << 1);
  struct Array *msg_out2 = malloc(sizeof(struct Array*));
  initArray(msg_out2, length);
  
  for(size_t i = 0; i < length; i++){ 
    msg_out->array[i] = dividend->array[i];
    insertArray(msg_out);
  }
  
  for(size_t i = 0; i < dividend->used - (divisor->array - 1); i++){
    uint8_t coef = msg_out->array[i];
    if(coef != 0){
      for(size_t j = 1; j < divisor->used; j++){
      	if(divisor->array[j] != 0){
      	  msg_out->array[i + j] ^= gf_mul(divisor->array[j], coef, gf_table);
          insertArray(msg_out);
        }
      }
    }
  }
  
  for(size_t i = separator; i < length << 1; i++){
    msg_out2->array[i - separator] = msg_out->array[separator];
    insertArray(msg_out2);
  }
  result->x = msg_out;
  result->y = msg_out2;
  return result;
}
