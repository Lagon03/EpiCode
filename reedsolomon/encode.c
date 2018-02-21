#include "array.h"
#include "op.h"

struct Array* array2(uint8_t a, uint8_t b)
{
  struct Array *p = malloc(sizeof(struct Array));
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
  struct Array *g = malloc(sizeof(struct Array));
  initArray(g, gf_pow(2, nsym, gf_table));
  g->array[0] = 1;
  for(uint8_t i = 0; i < nsym; i++)
    g = gf_poly_mul(g, array2(1, gf_pow(2, i, gf_table)), gf_table);
  return g;
}

/*Reed-Solomon main encoding function*/
struct Array* rs_encode_msg(struct Array* msg_in, uint8_t nsym, struct gf_tables *gf_table)
{
  if(msg_in->used + nsym > 255){
    fprintf(stderr, "Message too long, %lu is the size when 255 is the max", msg_in->used + nsym);
    exit(EXIT_FAILURE);
  }
  size_t len_gen = nsym * 2;
  struct Array *gen = malloc(sizeof(struct Array));
  initArray(gen, len_gen);
  gen = rs_generator_poly(nsym, gf_table);
  for(int i = 0; i < 12; i++)
    printf("%u, ",gen->array[i]);
  struct Array *empty_l = malloc(sizeof(struct Array));
  initZArray(empty_l, len_gen-1);
  struct Array *list = malloc(sizeof(struct Array));
  initArray(list, msg_in->used + len_gen - 1);
  list = merge(msg_in, empty_l);
  struct Tuple *res = malloc(sizeof(struct Tuple));
  res =  gf_poly_div(list, gen, gf_table);
  struct Array *msg_out = malloc(sizeof(struct Array));
  initArray(msg_out, msg_in->used+res->y->size);
  msg_out = merge(msg_in, res->y);
  return msg_out;
}
