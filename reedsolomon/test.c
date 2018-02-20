#include "op.h"

int main()
{
  uint8_t x = 0101;
  uint8_t y = 0110;
  uint8_t res1 = gf_add(x,y);
  uint8_t res2 = gf_sub(x, y);
  printf("res1: %u", res1);
  printf("res2: %u", res2);
  return 0;
}
