# ifndef ENCODING_H
# define ENCODING_H

# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>

# include "op.h"

struct Array* rs_encode_msg(struct Array *msg_in, uint8_t nsym, struct gf_tables *gf_tables);
size_t* JtoL(char** words, size_t size, size_t err_words);

#endif
