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
	
	struct Array *synd = malloc(sizeof(struct Array));
	printf("gf_exp : %u and %u and %u", gf_table->gf_exp->array[25], gf_table->gf_log->array[2] * -230, (gf_table->gf_log->array[2] * -230)%255);
	printf("GF_POW = %u", gf_pow(2, -230, gf_table));
  
  msg = rs_encode_msg(msg_in, 10, gf_table);
	for(size_t i = 0; i < msg->used; i++){
		printf("msg_out[%u] = %x\n", i, msg->array[i]);
	}
	msg->array[0] = 0;
	synd = rs_calc_syndromes(msg, 10, gf_table);
	int a = rs_check(msg,10, gf_table);
	printf("a = %d",a);
  for(size_t i = 0; i < msg->used; i++){
    printf("msg_out[%u] = %x\n", i, msg->array[i]);
  }
	for(size_t i = 0; i < synd->used; i++){
		printf("synd[%u] = %u\n", i, synd->array[i]);
	}
	struct Array *list = malloc(sizeof(struct Array));
	initArray(list,2);
	list->array[0] = 0;
	list->used = 1;

	msg = rs_correct_errdata(msg, synd, list, gf_table);
	for(size_t i = 0; i < msg->used; i++){
		printf("msg_out[%u] = %x\n", i, msg->array[i]);
	}
  return 0;
}
