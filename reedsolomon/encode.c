#include "op.h"

struct Array* array2(uint8_t a, uint8_t b)
{
  struct Array *p = malloc(sizeof(struct Array*));
  initArray(p, 2);
  p->array[0] = a;
  insertArray(p);
  p->array[1] = b;
  insertArray(p);
  return p;
}

/*Generate an irreducible generator polynomial (necessary to encode a message into Reed-Solomon)*/
struct Array* rs_generator_poly(uint8_t nsym, struct gf_tables *gf_table)
{
  struct Array*g = malloc(sizeof(uint8_t) * gf_pow(2, nsym, gf_table));
  g[0] = 1; 
  for(uint8_t i = 0; i < nsym; i++)
    g = gf_poly_mul(g, array2(1, gf_pow(2, i, gf_table)), gf_table);
  return g;
}

/*Reed-Solomon main encoding function*/
struct Array* rs_encode_msg(struct Array*msg_in, uint8_t nsym, size_t msg_size, struct gf_tables *gf_table)
{
  size_t len_gen = gf_pow(2, nsym, gf_table);
  struct Array*gen = malloc(sizeof(uint8_t) * len_gen);
  gen = rs_generator_poly(nsym, gf_table);
  for(int i = 0; i < 12; i++)
    printf("%u, ",gen[i]);
  struct Array*empty_l = calloc(len_gen-1, sizeof(uint8_t));
  struct Array*list = malloc(sizeof(uint8_t) * (msg_size + len_gen - 1)));
  list = merge(msg_in, empty_l, msg_size, len_gen-1);
  struct Tuple *res = malloc(sizeof(struct Tuple));
  res =  gf_poly_div(list, gen, gf_table);
  struct Array*msg_out = malloc(sizeof(uint8_t) * (msg_size+LENGTH(res->y)));
  msg_out = merge(msg_in, res->y, msg_size, LENGTH(res->y));
  return msg_out;
}
