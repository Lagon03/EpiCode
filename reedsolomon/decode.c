#include <stdbool.h>

#include "op.h"

/*Given the received codeword msg and the number of error correcting symbols (nsym), computes the
	syndromes polynomial. Mathematically, it's essentially equivalent to a Fourrier Transform 
	(Chien search being the inverse).*/

uint8_t* rs_calc_syndromes(uint8_t *msg, uint8_t nsym, struct gf_tables *gf_table)
{
  uint8_t *synd = calloc(nsym, sizeof(uint8_t));
  uint8_t *res = calloc(nsym+1, sizeof(uint8_t));
  for(uint8_t i = 0; i < nsym; i++)
    synd[i] = gf_poly_eval(msg, gf_pow(2, i, gf_table), gf_table);
  for(uint8_t i = 1; i < nsym + 1; i++)
    res[i] = synd[i-1]; //pad with one 0 for mathematical precision 
  //(else we can end up with weird calculations sometimes)
  return res;
}

/*Returns true if the message + ecc has no error of false otherwise (may not always catch a wrong
	decoding or a wrong message,particularly if there are too many errors, but it usually does)*/
bool rs_check(uint8_t msg[], uint8_t nsym, struct gf_tables *gf_table)
{
  uint8_t *synd = malloc(sizeof(uint8_t) * (nsym+1));
  synd = rs_calc_syndromes(msg, nsym, gf_table);
  bool res = true;
  for(uint8_t i = 0; i < nsym+1; i++)
    res &= synd[i] == 0;
  return res;
}

uint8_t* rs_find_errdata_locator(uint8_t e_pos[], struct gf_tables *gf_table)
{
  uint8_t *e_loc = malloc(sizeof(uint8_t) * LENGTH(e_pos));
  e_loc[0] = 1;
  uint8_t *one = malloc(sizeof(uint8_t));
  one[0] = 1;
  for(size_t i = 0; i < LENGTH(e_pos); i++){
    uint8_t pow_two = gf_pow(2, i, gf_table);
    uint8_t *arr = malloc(sizeof(uint8_t) * 2);
    arr[0] = pow_two;
    arr[1] = 0;
    e_loc = gf_poly_mul(e_loc, gf_poly_add(one, arr), gf_table);
  }
  return e_loc;
}

/*Compute the error (or erasures if you supply sigma=erasures locator polynomial, or errata) evaluator
	polynomial Omega from the syndrome and the error/erasures/errata locator Sigma*/
uint8_t* rs_find_error_evaluator(uint8_t synd[], uint8_t err_loc[], uint8_t nsym, struct gf_tables *gf_table)
{
  uint8_t *arr = calloc(nsym+2, sizeof(uint8_t));
  arr[0] = 1;
  struct Tuple *res = malloc(sizeof(struct Tuple));
  res->x = malloc(sizeof(uint8_t) * 256);
  res->y = malloc(sizeof(uint8_t) * 256);
  res = gf_poly_div(gf_poly_mul(synd, err_loc, gf_table), arr, gf_table);
  return res->y;
}

/*Forney algorithm, computes the values (error magnitude) to correct the input message.*/
uint8_t* rs_correct_errdata(uint8_t *msg_in, uint8_t *synd, uint8_t *err_pos, struct gf_tables *gf_table)
{
  size_t len = LENGTH(msg_in);
  uint8_t *coef_pos = malloc(sizeof(uint8_t) * LENGTH(err_pos));

  for(size_t i = 0; i < LENGTH(err_pos); i++)
    coef_pos[i] = len - 1 - err_pos[i];

  uint8_t *err_loc = malloc(sizeof(uint8_t)*LENGTH(coef_pos));
  err_loc= rs_find_errdata_locator(coef_pos, gf_table);
  uint8_t *rev_synd = malloc(sizeof(uint8_t) * LENGTH(synd));
  rev_synd = reverse_arr(synd);
  uint8_t *err_eval;
  err_eval = rs_find_error_evaluator(rev_synd, err_loc, LENGTH(err_loc)-1, gf_table);
  err_eval = reverse_arr(err_eval);
  uint8_t X[LENGTH(coef_pos)];

  for(size_t i = 0; i < LENGTH(coef_pos); i++){
    size_t l = 255 - coef_pos[i];
    X[i] = gf_pow(2, -l, gf_table);
  }

  uint8_t *E = calloc(len, sizeof(uint8_t));
  size_t Xlen = LENGTH(X);
  for(size_t i = 0; i < Xlen; i++){
    uint8_t Xi_inv = gf_inverse(X[i], gf_table);
    uint8_t err_loc_prime_tmp[Xlen];
    for(size_t j = 0; j < Xlen; j++){
      if(j != i)
	err_loc_prime_tmp[i] = gf_sub(1, gf_mul(Xi_inv, X[j], gf_table));
    }
    uint8_t err_loc_prime = 1;
    for(size_t j = 0; j < Xlen; j++)
      err_loc_prime = gf_mul(err_loc_prime, err_loc_prime_tmp[j], gf_table);
		
    uint8_t y = gf_poly_eval(reverse_arr(err_eval), Xi_inv, gf_table);
    y = gf_mul(gf_pow(X[i], 1, gf_table), y, gf_table);

    uint8_t magnitude = gf_div(y, err_loc_prime, gf_table);
    E[err_pos[i]] = magnitude;
  }
  msg_in = gf_poly_add(msg_in, E);
  return msg_in;
}

uint8_t* rs_find_error_locator(uint8_t* synd, uint8_t nsym, uint8_t* erase_loc, uint8_t erase_count, struct gf_tables *gf_table)
{
  uint8_t *err_loc = malloc(sizeof(uint8_t) * LENGTH(erase_loc));
  uint8_t *old_loc = malloc(sizeof(uint8_t) * LENGTH(erase_loc));
  if(erase_loc != NULL){
    err_loc = copy_arr(erase_loc, err_loc, LENGTH(erase_loc));
    old_loc = copy_arr(erase_loc, old_loc, LENGTH(erase_loc));
  }else{
    err_loc[0] = 1;
    old_loc[0] = 1;
  }
  int synd_shift = 0;
  if(LENGTH(synd) > nsym)
    synd_shift = LENGTH(synd) - nsym;
  for(int i = 0; i < nsym-erase_count;i++){
    int K;
    if(erase_loc != NULL)
      K = erase_count + i + synd_shift;
    else
      K = i + synd_shift;
    uint8_t delta = synd[K];
    for(size_t j = 1 ; j < LENGTH(err_loc); j++){
      uint8_t *rev = malloc(sizeof(uint8_t) * LENGTH(err_loc));
      rev = reverse_arr(err_loc);
      delta ^= gf_mul(rev[j - 1], synd[K - j], gf_table);
    }
    old_loc = realloc(old_loc, LENGTH(err_loc)+1);
    old_loc[LENGTH(err_loc)+1] = 0;
    if(delta != 0){
      if(LENGTH(old_loc) > LENGTH(err_loc)){
	uint8_t *new_loc = malloc(sizeof(uint8_t) * LENGTH(old_loc));
	new_loc = gf_poly_scale(old_loc, delta, gf_table);
	old_loc = gf_poly_scale(err_loc, gf_inverse(delta, gf_table), gf_table);
	err_loc = copy_arr(new_loc, err_loc, LENGTH(err_loc));
      }
      err_loc = gf_poly_add(err_loc, gf_poly_scale(old_loc, delta, gf_table));
    }	
  }
  while(LENGTH(err_loc) && err_loc[0] == 0)
    pop_arr(err_loc);
  size_t errs = LENGTH(err_loc) - 1;
  if(errs-erase_count * 2 + erase_count > nsym){
    fprintf(stderr, "Too many errors to correct");
    exit(EXIT_FAILURE);
  }
  return err_loc;
}

uint8_t* rs_find_errors(uint8_t *err_loc, size_t nmess, struct gf_tables *gf_table)//nmess is len(msg_in)
{
  size_t errs = LENGTH(err_loc) - 1;
  uint8_t *err_pos = malloc(sizeof(uint8_t) * errs);
  for(size_t i = 0; i < nmess; i++){
    if(gf_poly_eval(err_loc, gf_pow(2, i, gf_table), gf_table) == 0)
      err_pos[i] = nmess-1-i;
  }
  if(LENGTH(err_pos) != errs){
    fprintf(stderr, "Too many (or few) errors found by Chien Search for the errdata locator polynomial!");
    exit(EXIT_FAILURE);
  }
  return err_pos;
 }
