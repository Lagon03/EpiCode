# include <stdio.h>
# include <stdlib.h>

# include "analysis.h"

int main (void)
{
  //char *input;
  int n = 1;

  printf("Please enter input to detect mode\n");
  //n = scanf("\t%m[0-9-a-z]", &input);
  char *input = "Bonjour ceci est un input hard coded.";
  if(n == 1)
  {
    int* mod;
    Mode mode;

    mod = selectMode(input);
    printf("Input: %s\n", input);
    mode = checkmod(mod);
    
    printf("\n--> ");
    switch(mode) 
    {  
      case Numeric:
        {
          printf("Numerical mode\n");  
          break;
        }
      case Alphanumeric:  
        {
          printf("Alphanumerical mode\n");  
          break;
        }
      default :  
        {
          printf("Byte mode\n");
          break;
        }
    }  

    free(mod);
  }
  else
    fprintf(stderr, "No matching characters\n Please use characters in [a-z]\n");
  //free(input);
  return 1;
}
