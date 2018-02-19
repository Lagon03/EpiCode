#include "op.h"

uint8_t* array2(uint8_t a, uint8_t b)
{
  uint8_t *p = malloc(sizeof(uint8_t) * 2);
  p[0] = a;
  p[1] = b;
  return p;
}

/*Generate an irreducible generator polynomial (necessary to encode a message into Reed-Solomon)*/
uint8_t* rs_generator_poly(uint8_t num, struct gf_tables *gf_table)
{
    uint8_t *g = malloc(sizeof(uint8_t) * gf_pow(2, num, gf_table));
    g[0] = 1; 
    for(uint8_t i = 0; i < num; i++)
      g = gf_poly_mul(g, array2(1, gf_mul(2, i, gf_table)), gf_table);
    return g;
}

/*Reed-Solomon main encoding function*/
<<<<<<< HEAD
uint8_t* rs_encode_msg(uint8_t *msg_in, uint8_t nsym)
{
	uint8_t *gen = rs_generator_poly(nsym);//need to find the length
	uint8_t empty_l[LENGTH(gen)-1] = {0};
	uint8_t list[LENGTH(msg_in)+LENGTH(gen)-1] = merge(msg_in, empty_l, LENGTH(msg_in), LENGTH(gen)-1);
	struct Tuple res = gf_poly_div(list, gen);
	uint8_t msg_out[LENGTH(msg_in) + LENGTH(res.y)] = merge(msg_in, res.y, LENGTH(msg_in), LENGTH(res.y));
	return msg_out;
=======
uint8_t* rs_encode_msg(uint8_t *msg_in, uint8_t nsym, struct gf_tables *gf_table)
{
  uint8_t *gen = malloc(sizeof(uint8_t) * gf_pow(2, nsym, gf_table));
  gen = rs_generator_poly(nsym, gf_table);
  uint8_t *empty_l = calloc(LENGTH(gen)-1, sizeof(uint8_t));
  uint8_t *list = malloc(sizeof(uint8_t) * (LENGTH(msg_in)+LENGTH(gen)-1));
  list = merge(msg_in, empty_l, LENGTH(msg_in), LENGTH(gen)-1);
  struct Tuple *res = malloc(sizeof(struct Tuple));
  res =  gf_poly_div(list, gen, gf_table);
  uint8_t *msg_out = malloc(sizeof(uint8_t) * (LENGTH(msg_in)+LENGTH(res->y)));
  msg_out = merge(msg_in, res->y, LENGTH(msg_in), LENGTH(res->y));
  return msg_out;
>>>>>>> 18d73bae8884ade24ebfa58ccb7905f81890197f
}
