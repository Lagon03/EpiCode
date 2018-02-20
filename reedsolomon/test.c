#include "op.h"
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
  uint8_t msg_in[16] = {0x40, 0xd2, 0x75, 0x47, 0x76, 0x17, 0x32, 0x06,
			0x27, 0x26, 0x96, 0xc6, 0xc6, 0x96,0x70, 0xec};
  uint8_t *msg = malloc(sizeof(uint8_t) * 16);
  msg = rs_encode_msg(msg_in, 10, gf_table);
  for(int i =0; i < 26;i++)
    printf("\n%u ,", msg[i]);
  
  return 0;
}
