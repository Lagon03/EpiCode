/*
**  Header Dmask
**  file : dmask.h
**  description : header file dmask
*/

# ifndef _MASK_H_
# define _MASK_H_

# include <stddef.h>
# include <math.h>

# include "analysis.h"
# include "encode.h"

struct QrCode_Enc
{
    char** mat;
    struct EncData* data;
    size_t size;
};

void applyMask(char **mat, size_t size, int mask);

int sequential_Eval(char** mat, size_t size);
int block_Eval(char** mat, size_t size);
int pattern_Eval(char** mat, size_t size);
int** evaluate(struct QrCode_Enc* data);

struct QrCode_Enc* initQrCode(struct EncData* data);
void initMatrix(struct QrCode_Enc* data);

# endif
