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
    char** mat; // matrix
    struct EncData* data; // whole data
    size_t size; // size of the edge of the qrcode
    char* enc;  // what's encoded
    size_t size_enc;
};

extern const char finder_pattern[7][7];
extern const char align_pattern[5][5];
extern const size_t Ap_coord[40][8];

void applyMask(char **mat, size_t size, int mask);

int sequential_Eval(char** mat, size_t size);
int block_Eval(char** mat, size_t size);
int pattern_Eval(char** mat, size_t size);
int** evaluate(struct QrCode_Enc* data, int version, int cor);

struct QrCode_Enc* initQrCode(struct EncData* data);
void initMatrix(struct QrCode_Enc* data);

# endif
