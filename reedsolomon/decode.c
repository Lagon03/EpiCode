#include <stdbool.h>

#include "op.h"

/*Given the received codeword msg and the number of error correcting symbols (nsym), computes the
	syndromes polynomial. Mathematically, it's essentially equivalent to a Fourrier Transform 
	(Chien search being the inverse).*/

struct Array* rs_calc_syndromes(struct Array *msg, uint8_t nsym, struct gf_tables *gf_table)
{
  struct Array *synd = malloc(sizeof(struct Array*));
  initZArray(synd, nsym);
  struct Array *res = malloc(sizeof(struct Array*));
  initZArray(res, nsym+1);
  for(uint8_t i = 0; i < nsym; i++){
    synd->array[i] = gf_poly_eval(msg, gf_pow(2, i, gf_table), gf_table);
    insertArray(synd);
  }
  for(uint8_t i = 1; i < nsym + 1; i++){
    res->array[i] = synd->array[i-1]; //pad with one 0 for mathematical precision 
    insertArray(res);
  }
  //(else we can end up with weird calculations sometimes)
  return res;
}

/*Returns true if the message + ecc has no error of false otherwise (may not always catch a wrong
	decoding or a wrong message,particularly if there are too many errors, but it usually does)*/
bool rs_check(struct Array *msg, uint8_t nsym, struct gf_tables *gf_table)
{
  struct Array *synd = malloc(sizeof(struct Array*));
  initArray(synd, nsym+1);
  synd = rs_calc_syndromes(msg, nsym, gf_table);
  bool res = true;
  for(uint8_t i = 0; i < nsym+1; i++)
    res &= synd->array[i] == 0;
  return res;
}

struct Array* rs_find_errdata_locator(struct Array *e_pos, struct gf_tables *gf_table)
{
  struct Array *e_loc = malloc(sizeof(struct Array*));
  initZArray(e_loc, e_pos->size);
  e_loc[0] = 1;
  insertArray(e_loc);
  struct Array *one = malloc(sizeof(struct Array*));
  initZArray(one, 1);
  one[0] = 1;
  for(size_t i = 0; i < e_pos->used; i++){
    uint8_t pow_two = gf_pow(2, i, gf_table);
    struct Array *arr = malloc(sizeof(struct Array*));
    initArray(arr, 2);
    arr->array[0] = pow_two;
    insertArray(arr);
    arr->array[1] = 0;
    insertArray(arr);
    e_loc = gf_poly_mul(e_loc, gf_poly_add(one, arr), gf_table);
  }
  return e_loc;
}

/*Compute the error (or erasures if you supply sigma=erasures locator polynomial, or errata) evaluator
	polynomial Omega from the syndrome and the error/erasures/errata locator Sigma*/
struct Array* rs_find_error_evaluator(struct Array *synd, struct Array *err_loc, uint8_t nsym, struct gf_tables *gf_table)
{
  struct Array *arr = malloc(sizeof(struct Array*));
  initZArray(arr, nsym+2);
  arr[0] = 1;
  insertArray(arr);
  struct Tuple *res = malloc(sizeof(struct Tuple));
  res->x = malloc(sizeof(struct Array*));
  initArray(res->x, 256);
  res->y = malloc(sizeof(struct Array*));
  initArray(res->y, 256);
  res = gf_poly_div(gf_poly_mul(synd, err_loc, gf_table), arr, gf_table);
  return res->y;
}

/*Forney algorithm, computes the values (error magnitude) to correct the input message.*/
struct Array* rs_correct_errdata(struct Array *msg_in, struct Array *synd, struct Array *err_pos, struct gf_tables *gf_table)
{
  size_t len = msg_in->size;
  struct Array *coef_pos = malloc(sizeof(struct Array*));
  initArray(coef_pos, err_pos->size);
  for(size_t i = 0; i < err_pos->used; i++){
    coef_pos[i] = len - 1 - err_pos[i];
    insertArray(coef_pos);
  }

  struct Array *err_loc = malloc(sizeof(struct Array*));
  initArray(err_loc, coef_pos->size);
  err_loc= rs_find_errdata_locator(coef_pos, gf_table);

  struct Array *rev_synd = malloc(sizeof(struct Array*));
  initArray(synd, synd->size);
  rev_synd = reverse_arr(synd);
  struct Array *err_eval = malloc(sizeof(struct Array*));
  initArray(err_eval, 256);
  err_eval = rs_find_error_evaluator(rev_synd, err_loc, err_loc->used-1, gf_table);
  err_eval = reverse_arr(err_eval);
  struct Array *X = malloc(sizeof(struct Array*));
  initArray(X, coef_pos->size);

  for(size_t i = 0; i < coef_pos->used; i++){
    size_t l = 255 - coef_pos->array[i];
    X->array[i] = gf_pow(2, -l, gf_table);
    insertArray(X);
  }

  struct Array *E = malloc(sizeof(struct Array*));
  initZArray(E, len);
  for(size_t i = 0; i < X->used; i++){
    uint8_t Xi_inv = gf_inverse(X->array[i], gf_table);
    uint8_t err_loc_prime_tmp[X->used];
    for(size_t j = 0; j < X->used; j++){
      if(j != i){
        err_loc_prime_tmp->array[i] = gf_sub(1, gf_mul(Xi_inv, X->array[j], gf_table));
        insertArray(err_loc_prime_tmp);
      }
    }
    uint8_t err_loc_prime = 1;
    for(size_t j = 0; j < X->used; j++)
      err_loc_prime = gf_mul(err_loc_prime, err_loc_prime_tmp->array[j], gf_table);
		
    uint8_t y = gf_poly_eval(reverse_arr(err_eval), Xi_inv, gf_table);
    y = gf_mul(gf_pow(X->array[i], 1, gf_table), y, gf_table);

    uint8_t magnitude = gf_div(y, err_loc_prime, gf_table);
    E->array[err_pos->array[i]] = magnitude;
    insertArray(E);
  }
  msg_in = gf_poly_add(msg_in, E);
  return msg_in;
}

struct Array* rs_find_error_locator(struct Array* synd, uint8_t nsym, struct Array* erase_loc, uint8_t erase_count, struct gf_tables *gf_table)
{
  struct Array *err_loc = malloc(sizeof(struct Array*));
  initZArray(err_loc, 10);
  struct Array *old_loc = malloc(sizeof(struct Array*));
  initZArray(old_loc, 10);
  err_loc[0] = 1;
  insertArray(err_loc);
  old_loc[0] = 1;
  insertArray(old_loc);

  int synd_shift = 0;
  if(synd->used > nsym)
    synd_shift = synd->used - nsym;
  for(int i = 0; i < nsym-erase_count;i++){
    int K = i + synd_shift;
    uint8_t delta = synd->array[K];
    for(size_t j = 1 ; j < err_loc->used; j++){
      struct Array *rev = malloc(sizeof(struct Array*));
      initArray(rev, err_loc->size);
      rev = reverse_arr(err_loc);
      delta ^= gf_mul(rev->array[j - 1], synd->array[K - j], gf_table);
    }
    old_loc[old_loc->used + 1] = 0;
    insertArray(old_loc);
    if(delta != 0){
      if(old_loc->used > err_loc->used){
        struct Array *new_loc = malloc(sizeof(struct Array*));
        initArray(new_loc, old_loc->size);
      	new_loc = gf_poly_scale(old_loc, delta, gf_table);
      	old_loc = gf_poly_scale(err_loc, gf_inverse(delta, gf_table), gf_table);
      	err_loc = copy_arr(new_loc, err_loc);
      }
      err_loc = gf_poly_add(err_loc, gf_poly_scale(old_loc, delta, gf_table));
    }
  }
  while(err_loc->used && err_loc[0] == 0)
    err_loc = pop_arr(err_loc);
  size_t errs = err_loc->used - 1;
  if(errs-erase_count * 2 + erase_count > nsym){
    fprintf(stderr, "Too many errors to correct");
    exit(EXIT_FAILURE);
  }
  return err_loc;
}

struct Array* rs_find_errors(struct Array *err_loc, size_t nmess, struct gf_tables *gf_table)//nmess is len(msg_in)
{
  size_t errs = err_loc->used - 1;
  struct Array *err_pos = malloc(sizeof(struct Array*));
  initArray(err_pos, errs);
  for(size_t i = 0; i < nmess; i++){
    if(gf_poly_eval(err_loc, gf_pow(2, i, gf_table), gf_table) == 0){
      err_pos[i] = nmess-1-i;
      insertArray(err_pos);
    }
  }
  if(err_pos->used != errs){
    fprintf(stderr, "Too many (or few) errors found by Chien Search for the errdata locator polynomial!");
    exit(EXIT_FAILURE);
  }
  return err_pos;
 }
