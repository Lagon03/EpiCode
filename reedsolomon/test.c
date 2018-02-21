#include "op.h"
#include "array.h"
#include "encode.h"

int main()
{
  struct gf_tables *gf_table = malloc(sizeof(struct gf_tables));
  gf_table = init_tables();
  //for(int i = 0; i < 50;i++){
    // printf("exp %d: %u\n",i,gf_table->gf_log[i]);
    //}
  uint8_t x = 12;
  uint8_t y = 10;
  uint8_t res1 = gf_mul(x,y,gf_table);
  uint8_t res2 = gf_div(x, y, gf_table);
  printf("res1: %u", res1);
  printf("res2: %u", res2);
  //for(int i =0; i < LENGTH(gf_table->gf_log); i++)
  //  printf("elem %d: %u", i, gf_table->gf_log[i]);
  struct Array *msg_in = malloc(sizeof(struct Array));
  initArray(msg_in, 17);
  msg_in->array = {0x40, 0xd2, 0x75, 0x47, 0x76, 0x17, 0x32, 0x06,
			0x27, 0x26, 0x96, 0xc6, 0xc6, 0x96,0x70, 0xec};
  msg_in->used = 16;
  struct Array *msg = malloc(sizeof(struct Array));
  initArray(msg, 17);
  
  msg = rs_encode_msg(msg_in, 10, gf_table);
  for(int i =0; i < msg->used; i++)
    printf("\n%u ,", msg->array[i]);
  
  return 0;
}
