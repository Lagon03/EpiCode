# ifndef CYPHER_ANALYSIS_H
# define CYPHER_ANALYSIS_H

# include <stdio.h>
# include <stdlib.h>


struct options
{
  char* message;
  int correction;
  int mode;
  int version;
  int epi;
  char* output;
  char* name;
};

enum data
{
  Numeric = 0, 
  Alphanumeric = 1, 
  Byte = 2,
  Kanji = 3
};
typedef enum data Mode;

size_t getSize(char *input);
int* selectMode(char *input);
Mode checkmod(int *mod);

# endif
