#include <stdbool.h>
#include <stdio.h>
#include <string.h>


#include "array.h"
#include "op.h"

/*Given the received codeword msg and the number of error correcting symbols (nsym), computes the
  syndromes polynomial. Mathematically, it's essentially equivalent to a Fourrier Transform 
  (Chien search being the inverse).*/

struct Array* rs_calc_syndromes(struct Array *msg, uint8_t nsym, struct gf_tables *gf_table)
{
    struct Array *synd = malloc(sizeof(struct Array));
        initZArray(synd, nsym+1);
        struct Array *res = malloc(sizeof(struct Array));
        initZArray(res, nsym+2);
        for(uint8_t i = 0; i < nsym; i++){
            synd->array[i] = gf_poly_eval(msg, gf_pow(2, i, gf_table), gf_table);
                insertArray(synd);
        }
    insertArray(res);
        for(uint8_t i = 1; i < nsym + 1; i++){
            res->array[i] = synd->array[i-1]; //pad with one 0 for mathematical precision
            insertArray(res);
        }
    //(else we can end up with weird calculations sometimes)
    free(synd);
    return res;
}

/*Returns true if the message + ecc has no error of false otherwise (may not always catch a wrong
  decoding or a wrong message,particularly if there are too many errors, but it usually does)*/
bool rs_check(struct Array *msg, uint8_t nsym, struct gf_tables *gf_table)
{
    struct Array *synd = malloc(sizeof(struct Array));
        initArray(synd, nsym+1);
        synd = rs_calc_syndromes(msg, nsym, gf_table);
        bool res = true;
        for(uint8_t i = 0; i < nsym+1; i++)
            res &= synd->array[i] == 0;
                return res;
}

struct Array* rs_find_errdata_locator(struct Array *e_pos, struct gf_tables *gf_table)
{
    struct Array *e_loc = malloc(sizeof(struct Array));
        initZArray(e_loc, e_pos->used+1);
        e_loc->array[0] = 1;
        insertArray(e_loc);
        struct Array *one = malloc(sizeof(struct Array));
        initZArray(one, 2);
        one->array[0] = 1;
        insertArray(one);
        for(size_t i = 0; i < e_pos->used; i++){
            uint8_t pow_two = gf_pow(2, e_pos->array[i], gf_table);
                struct Array *arr = malloc(sizeof(struct Array));
                initArray(arr, 2);
                arr->array[0] = pow_two;
                insertArray(arr);
                arr->array[1] = 0;
                insertArray(arr);
            struct Array *add = gf_poly_add(one, arr);
                e_loc = gf_poly_mul(e_loc, add, gf_table);
        }
    return e_loc;}
    
    /*Compute the error (or erasures if you supply sigma=erasures locator polynomial, or errata) evaluator
      polynomial Omega from the syndrome and the error/erasures/errata locator Sigma*/
    struct Array* rs_find_error_evaluator(struct Array *synd, struct Array *err_loc, uint8_t nsym, struct gf_tables *gf_table)
{
    struct Array *res = malloc(sizeof(struct Array));
    initArray(res, nsym+2);
    res = gf_poly_mul(synd, err_loc, gf_table);
    size_t len = res->used - (nsym+1);
    memmove(res->array, res->array+len, len);
    res->used = res->used - len;
    return res;
}

/*Forney algorithm, computes the values (error magnitude) to correct the input message.*/
struct Array* rs_correct_errdata(struct Array *msg_in, struct Array *synd, struct Array *err_pos, struct gf_tables *gf_table)
{
	size_t len = msg_in->size;
	struct Array *coef_pos = malloc(sizeof(struct Array));
	initArray(coef_pos, err_pos->used+1);
	for(size_t i = 0; i < err_pos->used; i++){
		coef_pos->array[i] = len - 1 - err_pos->array[i];
		insertArray(coef_pos);
	}

	struct Array *err_loc = malloc(sizeof(struct Array));
	initArray(err_loc, coef_pos->used+1);
	err_loc= rs_find_errdata_locator(coef_pos, gf_table);
	
	struct Array *rev_synd = reverse_arr(synd);
	struct Array *err_eval = malloc(sizeof(struct Array));
	uint8_t nsym = err_loc->used - 1;
	err_eval = rs_find_error_evaluator(rev_synd, err_loc, nsym, gf_table);
	err_eval = reverse_arr(err_eval);

	struct Array *X = malloc(sizeof(struct Array));
	initArray(X, coef_pos->used);
	for(size_t t = 0; t < coef_pos->used; t++){
		uint8_t q = coef_pos->array[t];
		X->array[t] = gf_pow(2, q, gf_table);
		insertArray(X);
	}

	struct Array *E = malloc(sizeof(struct Array));
	initZArray(E, len);
	E->used = len;
	for(size_t i = 0; i < X->used; i++){
		uint8_t Xi_inv = gf_inverse(X->array[i], gf_table);
		struct Array *err_loc_prime_tmp = malloc(sizeof(struct Array));
		initArray(err_loc_prime_tmp, X->used+1);
		for(size_t j = 0; j < X->used; j++){
			if(j != i){
				err_loc_prime_tmp->array[err_loc_prime_tmp->used] = gf_sub(1, gf_mul(Xi_inv, X->array[j], gf_table));
				insertArray(err_loc_prime_tmp);
		  	}
    	}
		
		uint8_t err_loc_prime = 1;
		for(size_t k = 0; k < err_loc_prime_tmp->used; k++)
			err_loc_prime = gf_mul(err_loc_prime, err_loc_prime_tmp->array[k], gf_table);

		//err_eval = reverse_arr(err_eval);
		uint8_t y = gf_poly_eval(reverse_arr(err_eval), Xi_inv, gf_table);
		y = gf_mul(gf_pow(X->array[i], 1, gf_table), y, gf_table);
		uint8_t magnitude = gf_div(y, err_loc_prime, gf_table);
		E->array[err_pos->array[i]] = magnitude;
	}
	msg_in = gf_poly_add(msg_in, E);
	return msg_in;
}

struct Array* rs_find_error_locator(struct Array* synd, uint8_t nsym, uint8_t erase_count, struct gf_tables *gf_table)
{
	struct Array *err_loc = malloc(sizeof(struct Array*));
	initZArray(err_loc, 10);
	struct Array *old_loc = malloc(sizeof(struct Array*));
	initZArray(old_loc, 10);
	err_loc->array[0] = 1;
	insertArray(err_loc);
	old_loc->array[0] = 1;
	insertArray(old_loc);
	
	unsigned int synd_shift = 0;
	if(synd->used > nsym)
		synd_shift = synd->used - nsym;
	
	for(int i = 0; i < nsym-erase_count;i++){
		int K = i + synd_shift;
		uint8_t delta = synd->array[K];
		for(size_t j = 1 ; j < err_loc->used; j++){
			delta ^= gf_mul(err_loc->array[err_loc->used - (j+1)], synd->array[K - j], gf_table);
		}
		old_loc->array[old_loc->used + 1] = 0;
		insertArray(old_loc);
		if(delta != 0){
			if(old_loc->used > err_loc->used){
				struct Array *new_loc = malloc(sizeof(struct Array*));
				new_loc = gf_poly_scale(old_loc, delta, gf_table);
				old_loc = gf_poly_scale(err_loc, gf_inverse(delta, gf_table), gf_table);
				memmove(err_loc->array, new_loc->array, err_loc->used);
				err_loc->used = new_loc->used;
			}
			struct Array *scale = malloc(sizeof(struct Array*));
			scale = gf_poly_scale(old_loc, delta, gf_table);
			err_loc = gf_poly_add(err_loc, scale);
		}
	}
	while(err_loc->used && err_loc->array[0] == 0)
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
	size_t counter = 0;
	struct Array *err_pos = malloc(sizeof(struct Array*));
	initArray(err_pos, errs);
	for(size_t i = 0; i < nmess; i++){
		if(gf_poly_eval(err_loc, gf_pow(2, i, gf_table), gf_table) == 0){
			err_pos->array[counter] = nmess - 1 - i;
			insertArray(err_pos);
			counter++;
		}
	}
	if(err_pos->used != errs){
		fprintf(stderr, "Too many (or few) errors found by Chien Search for the errdata locator polynomial!");
		exit(EXIT_FAILURE);
	}
	return err_pos;
 }
