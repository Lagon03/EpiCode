//
//  decode.h
//  Epicode1
//
//  Created by Jack Chouchani on 15/03/2018.
//  Copyright © 2018 Not A Barcode. All rights reserved.
//
#include <stdbool.h>

#ifndef decode_h
#define decode_h

struct Array* rs_calc_syndromes(struct Array *msg, uint8_t nsym, struct gf_tables *gf_table);

bool rs_check(struct Array *msg, uint8_t nsym, struct gf_tables *gf_table);

struct Array* rs_find_errdata_locator(struct Array *e_pos, struct gf_tables *gf_table);

#endif /* decode_h */
