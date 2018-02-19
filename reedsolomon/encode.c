#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "op.h"

/*Generate an irreducible generator polynomial (necessary to encode a message into Reed-Solomon)*/
uint8_t* rs_generator_poly(uint8_t num)
{
    uint8_t g[1] = {1};
    for(uint8_t i = 0; i < num; i++)
        g = gf_poly_mul(g, [1, gf_pow(2, i)]);
    return g;
}

/*Reed-Solomon main encoding function*/
uint8_t* rs_encode_msg(uint8_t *msg_in, uint8_t nsym)
{
	uint8_t *gen = rs_generator_poly(nsym);//need to find the length
	uint8_t empty_l[LENGTH(gen)-1] = {0};
	uint8_t list[LENGTH(msg_in)+LENGTH(gen)-1] = merge(msg_in, empty_l, LENGTH(msg_in), LENGTH(gen)-1);
	struct Tuple res = gf_poly_div(list, gen);
	uint8_t msg_out[LENGTH(msg_in) + LENGTH(res.y)] = merge(msg_in, res.y, LENGTH(msg_in), LENGTH(res.y));
	return msg_out;
}
