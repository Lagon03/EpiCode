#include "../../headers/array.h"
#include "../../headers/op.h"
#include "../../headers/encode.h"

#include <string.h>

struct Array* array2(uint8_t a, uint8_t b)
{
    struct Array *p = malloc(sizeof(struct Array));
    initArray(p, 3);
    p->array[0] = a;
    p->array[1] = b;
    p->used = 2;
    return p;
}

/*Generate an irreducible generator polynomial (necessary to encode a message into Reed-Solomon)*/
struct Array* rs_generator_poly(uint8_t nsym, struct gf_tables *gf_table)
{
    struct Array *g = malloc(sizeof(struct Array));
    initArray(g, nsym);
    g->array[0] = 1;
    insertArray(g);
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

    struct Array *msg_out = malloc(sizeof(struct Array));

    initZArray(msg_out, msg_in->used + gen->used -1);
    memmove(msg_out->array, msg_in->array, msg_in->used);

    for(size_t i = 0; i < msg_in->used; i++){
        uint8_t coef = msg_out->array[i];
        if (coef != 0){
            for(size_t j = 1; j < gen->used; j++){
                msg_out->array[i+j] ^= gf_mul(gen->array[j], coef, gf_table);
            }
        }
    }
    msg_out->used = gen->used + msg_in->used-1;
    //struct Tuple *res = malloc(sizeof(struct Tuple));
    //res =  gf_poly_div(msg_in, gen, gf_table);
    memmove(msg_out->array, msg_in->array, msg_in->used);
    return msg_out;
}

size_t* JtoL(char** words, size_t size, size_t err_words)
{    
    struct gf_tables *gf_table = malloc(sizeof(struct gf_tables));
    gf_table->gf_exp = malloc(sizeof(struct Array));
    gf_table->gf_log = malloc(sizeof(struct Array));
    initArray(gf_table->gf_exp, 512);
    initArray(gf_table->gf_log, 256);
    gf_table = init_tables();

    struct Array* msg_in = malloc(sizeof(struct Array));
    initArray(msg_in, size);
    for(size_t i = 0; i < size; ++i)
    {
        uint8_t cur = (uint8_t)convertToDec(words[i]);
        msg_in->array[i] = cur;
        insertArray(msg_in);
    }
    struct Array* msg = malloc(sizeof(struct Array));
    initArray(msg, 2);

    msg = rs_encode_msg(msg_in, err_words, gf_table);
    size_t * output = malloc(sizeof(size_t) * err_words);
    for(size_t i = 0; i < err_words; ++i)
    {
        output[i] = msg->array[i + msg_in->used];
    }

    free(gf_table->gf_exp);
    free(gf_table->gf_log);
    free(gf_table);
    freeArray(msg);
    freeArray(msg_in);
    return output;
}
