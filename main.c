# include <stdio.h>
# include <stdlib.h>

# include "analysis.h"

int main (void)
{
  char *input;
  int n = 1;

  printf("Please enter input to detect mode\n");
  n = scanf("\t%m[0-9-a-z]", &input);
  //char *input = "45/89*423-2+1\0";
  if(n == 1)
    {
      int* mod;

      mod = selectMode(input);
      printf("Input: %s\n", input);
      checkmod(mod);

      free(mod);
    }
  else
    fprintf(stderr, "No matching characters\n Please use characters in [a-z]\n");
  free(input);
	return 1;
}
