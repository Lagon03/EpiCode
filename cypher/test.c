#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  char string[256];
  printf("Please enter a string :\n");
  scanf("%s", &string);

  printf("%s\n", string);
}
