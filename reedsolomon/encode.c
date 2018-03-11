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
  initArray(g, nsym);
  g->array[0] = 1;
  for(uint8_t i = 0; i < nsym; i++){
    g = gf_poly_mul(realloc(g, gf_pow(2, i, gf_table)), array2(1, gf_pow(2, i, gf_table)), gf_table);
  }
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
  gen->array[0] = 1;
  gen->array[1] = 216;
  gen->array[2] = 194;
  gen->array[3] = 159;
  gen->array[4] = 111;
  gen->array[5] = 199;
  gen->array[6] = 94;
  gen->array[7] = 95;
  gen->array[8] = 113;
  gen->array[9] = 157;
  gen->array[10] = 193;
  gen->used = 11;
  //gen = rs_generator_poly(nsym, gf_table);
  if (msg_in->size < msg_in->used + gen->used-1)
  {
    msg_in->size *= 2;
    void *p = realloc(msg_in->array, msg_in->size * sizeof(uint8_t));
    if(p != NULL)
        msg_in->array = p;
      else
        exit(EXIT_FAILURE);
  }
  else
    msg_in->used += gen->used-1;

  struct Tuple *res = malloc(sizeof(struct Tuple));

  res =  gf_poly_div(msg_in, gen, gf_table);
  for (int i = 0; i < res->y->used; ++i)
  {
    printf("res->y->array[%d] = %u\n",i, res->y->array[i] );
  }
  printf("res->y len%u\n",res->y->used );
  for(int i = 0; i < gen->used; i++)
    printf("gen: %u, ", res->x->array[i]);
  struct Array *msg_out = malloc(sizeof(struct Array));
  initArray(msg_out, msg_in->used+res->y->used);
  msg_out = merge(msg_in, res->y);
  return msg_out;
}
