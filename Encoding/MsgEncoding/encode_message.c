# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <err.h>

# include "../headers/analysis.h"
# include "../headers/encode.h"

const char *alpha_ = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ $%*+-./:";

//-----------------------------------------------------------------------------
//                              Tools functions
//-----------------------------------------------------------------------------


char* adjustString(char* data, char* input, size_t limit, size_t size
    , size_t in_len)
{
  if(limit <= size + in_len) {
    limit = size + in_len + 1;
    data = realloc(data, size + in_len + 1);
  }
  data = strcat(data, input);
  return data;
}

size_t getDec(char _char)
{
  size_t i = 0;
  while(_char != alpha_[i])
    ++i;
  return i;
}

//-----------------------------------------------------------------------------
//                              End of tools functions
//-----------------------------------------------------------------------------

char* num_encoding(char* data, size_t len)
{
  // We need to create (len / 3) pair (if len %3 == 0) else add 1 more
  // then convert it to binary
  size_t nbpair;
  if(len % 3 == 0)
    nbpair = len / 3;
  else
    nbpair = (len / 3) + 1;

  char* enc_message = calloc(nbpair * 10, sizeof(char));
  enc_message[0] = '\0';
  size_t size = 0;
  size_t i = 0;
  while(i < len)
  {
    char* pair = calloc(4, sizeof(char));
    pair[3] = '\0';

    for(int y = 0; y < 3 && i < len; ++y, ++i)
    {
      pair[y] = data[i];
    }
    size_t numC = strtol(pair, NULL, 10);
    char *bits = convertToByte(numC);
    bits = adjustSize(bits, 10);
    enc_message = adjustString(enc_message, bits, nbpair * 10, size, 10);
    size += 10;

    free(pair);
    free(bits);
  }
  return enc_message;
}

char* alpha_encoding(char* data, size_t len)
{
  // We need to create (len / 3) pair (if len %3 == 0) else add 1 more
  // then convert it to binary
  size_t nbpair;
  if(len % 2 == 0)
    nbpair = len / 2;
  else
    nbpair = (len / 2) + 1;

  char* enc_message = calloc(nbpair * 10, sizeof(char));
  enc_message[0] = '\0'; // put a terminating symbole at the start
  size_t size = 0; // actual used space of the enc_message pointer
  size_t i = 0; // position in the input
  while(i < len)
  {
    int* pair = calloc(2, sizeof(int));

    for(int y = 0; y < 2 && i < len; ++y, ++i)
    {
      pair[y] = getDec(data[i]);
      if(y + 1 < 2 && i + 1 >= len)
        pair[y + 1] = -1;
    }
    size_t bin_num = 0; // variable for stocking the decimal value of the pair 
    char *bits; // variable for stocking the value converted into binary
    if(pair[1] == -1) { // if for one character
      bin_num = pair[0];
      bits = convertToByte(bin_num);
      bits = adjustSize(bits, 6);
    }
    else { // else for more than 1 character 
      bin_num = (pair[0] * 45) + pair[1];
      bits = convertToByte(bin_num);
      bits = adjustSize(bits, 11);
    }
    // then we concatenate the binary and the whole encoded message
    enc_message = adjustString(enc_message, bits, nbpair * 10, size, 11);
    // we increment the used size of the encoded message var
    if(pair[1] == -1)
      size += 6;
    else
      size += 11;
  
    // we free for avoiding leaking
    free(pair);
    free(bits);
  }

  return enc_message;
}

char* byte_encoding(char* data, size_t len)
{  
  char* enc_message = calloc(len * 8, sizeof(char));
  enc_message[0] = '\0';
  size_t size = 0;
  size_t i = 0;
  while(i < len)
  {
    size_t numC = (size_t)data[i];
    char *bits = convertToByte(numC);
    bits = adjustSize(bits, 8);
    enc_message = adjustString(enc_message, bits, len * 8, size, 8);
    size += 8;

    free(bits);
    ++i;
  }
  return enc_message;

}
