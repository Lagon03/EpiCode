#include <stdio.h>
#include <stdlib.h>

#include "../headers/analysis.h"

size_t getSize(char *input)
{
  size_t i = 0;
  for(; input[i] != '\0'; ++i);
      /*printf("%c",input[i]);
  printf("\n");*/
  return i;
}

int* selectMode(char *input)
{
  int *mod = calloc(3, sizeof(int));
  size_t i = 0;
  while(*(input + i) != '\0')
  {
    int char_ = ((int)*(input + i));
    //printf("Ascii value : %i\n", char_);
    if (((char_ > 47 && char_ < 58)))
      mod[0] = 1;
    else if ((((char_ > 64 && char_ < 90)) |
          (char_ > 35 && char_ < 59)) | (char_ == 32))
      mod[1] = 1;
    else if (char_ != 44 && (((char_ < 38) && (char_ > 35)) | ((char_ < 48) 
            && (char_ > 44)) | (char_ == 58)))
      mod[1] = 1;
    else
    {
      mod[2] = 1;
      break;
    }
    ++i;
  }
  return mod;
}

Mode checkmod(int *mod) // just for debug purpose
{
  for(int i = 2; i > -1; i--)
  {
    if(mod[i] == 1)
    {
      //printf("\n--mode : %i\n", i);
      return (Mode) i;
    }
  }
  return (Mode) 0;
}
