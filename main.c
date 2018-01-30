# include <stdio.h>
# include <stdlib.h>
# include <err.h>
# include <string.h>

# include "analysis.h"
# include "encode.h"

struct options* checkArg(int argc, char* argv[])
{
  struct options *arg = malloc(sizeof(struct options));
  arg->message = NULL;
  arg->correction = -1;
  arg->mode = -1;
  for(int i = 1; i < argc; ++i)
  {
    if(strcmp(argv[i], "-d") == 0) { // arg for the data
      ++i;
      if (i >= argc)
        err(1, "!!!--------> Error missing arguments.\n");
      arg->message = argv[i];
    }
    if(strcmp(argv[i], "-c") == 0){
      ++i;
      if (i >= argc)
        err(1, "!!!--------> Error missing arguments.\n");
      switch((int)argv[i][0]) {
        case 'M': // Medium
          {
            arg->correction = 1;
            break;
          }
        case 'Q': // Quartile
          {
            arg->correction = 2;
            break;
          }
        case 'H': // High
          {
            arg->correction = 3;
            break;
          }
        default: // Low
          {
            arg->correction = 0;
            break;
          }
      }
    }
  }
  return arg;
}

int main (int argc, char* argv[])
{
  struct options *arg = checkArg(argc, argv);
  if(arg->message == NULL)
    arg->message = "HELLO WORLD";
  int* mod;
  Mode mode;

  mod = selectMode(arg->message);
  arg->mode = *mod;
  printf("Input: %s\n", arg->message);
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
  getEncodedSize(arg);

  free(mod);
  free(arg);

  return 1;
}
