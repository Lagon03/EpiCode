# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# include "encode.h"
# include "analysis.h"

// Simple pow function
size_t sPow(size_t x, int n)
{
  size_t res = 1;
  if(n > 0)
    while(n > 0) {
      res *= x;
      --n;
    }
  else
    while(n < 0) {
      res >>= x;
      ++n;
    }
  return res;
}

// Function used to retrieve the last inferior power of two
size_t getLastInf(size_t x)
{
  size_t pow = 0;
  while(x >= sPow(2, pow))
    ++pow;
  return pow;
}

// Function used to convert size_t to bit array
char* convertToByte(size_t input)
{
  size_t len = (getLastInf(input) * sizeof(char)) + 1;
  char *array = calloc(len, sizeof(char));
  array[len - 1] = '\0';

  size_t index = len - 2;

  while(input > 0) {
    if(input % 2 == 0)
      array[index] = '0';
    else
      array[index] = '1';

    input >>= 1;
    --index;
  }

  return array;
}

size_t getEncodedSize(char *input)
{
  size_t char_count = getSize(input);
  char *count_bits = convertToByte(char_count);

  printf("size_t to bits : %li -> [%s]\n", char_count, count_bits);
  free(count_bits);
  return 0;
}
