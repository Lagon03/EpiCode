#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "op.h"

/*Generate an irreducible generator polynomial (necessary to encode a message into Reed-Solomon)*/
uint8_t* rs_generator_poly(uint8_t num)
{
    uint8_t g[] = [1];
    for(uint8_t i = 0; i < num; i++)
        g = gf_poly_mul(g, [1, gf_pow(2, i)]);
    return g;
}

