#include "op.h"
#include "array.h"
#include "encode.h"
#include "decode.h"

int main()
{
	struct gf_tables *gf_table = malloc(sizeof(struct gf_tables));
	gf_table->gf_exp = malloc(sizeof(struct Array));
	gf_table->gf_log = malloc(sizeof(struct Array));
	initArray(gf_table->gf_exp, 512);
	initArray(gf_table->gf_log, 256);
	gf_table = init_tables();
	
	
	struct Array *msg_in = malloc(sizeof(struct Array));
	
	initArray(msg_in, 100);
	msg_in->array[0] = 0x40;
	msg_in->array[1] = 0xd2;
	msg_in->array[2] = 0x75;
	msg_in->array[3] = 0x47;
	msg_in->array[4] = 0x76;
	msg_in->array[5] = 0x17;
	msg_in->array[6] = 0x32;
	msg_in->array[7] = 0x06;
	msg_in->array[8] = 0x27;
	msg_in->array[9] = 0x26;
	msg_in->array[10] = 0x96;
	msg_in->array[11] = 0xc6;
	msg_in->array[12] = 0xc6;
	msg_in->array[13] = 0x96;
	msg_in->array[14] = 0x70;
	msg_in->array[15] = 0xec;
	msg_in->used = 16;
	
	struct Array *msg = malloc(sizeof(struct Array));
	initArray(msg, 170);
	
	struct Array *err_loc = malloc(sizeof(struct Array));
	
	struct Array *synd = malloc(sizeof(struct Array));
	
	struct Array *pos = malloc(sizeof(struct Array));
	
	msg = rs_encode_msg(msg_in, 10, gf_table);
	
	printf("Msg Encoded: [");
	for (size_t i = 0; i < msg->used; i++) {
		printf("%u,",msg->array[i]);
	}
	printf("]\n");
	
	//Tempering msg
	msg->array[0] = 0;
	msg->array[10] = 0;
	msg->array[20] = 0;
	
	printf("Msg Tempered: [");
	for (size_t i = 0; i < msg->used; i++) {
		printf("%u,",msg->array[i]);
	}
	printf("]\n");
	
	synd = rs_calc_syndromes(msg, 10, gf_table);
	err_loc = rs_find_error_locator(synd, 10, NULL, gf_table);
	pos = rs_find_errors(reverse_arr(err_loc), msg->used, gf_table);
	
	printf("Error positions: [");
	for (size_t i = 0; i < pos->used; i++) {
		printf("%u,",pos->array[i]);
	}
	printf("]\n");
	
	msg = rs_correct_errdata(msg, synd, pos, gf_table);
	
	printf("Msg Corrected: [");
	for (size_t i = 0; i < msg->used; i++) {
		printf("%u,",msg->array[i]);
	}
	
	return 0;
}
