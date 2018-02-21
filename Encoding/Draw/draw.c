# include <stdio.h>
# include <stdlib.h>

# include "../headers/draw.h"
# include "../headers/mask.h"
# include "../headers/encode.h"

void protectMatrix(struct QrCode_Enc* data) {
    // we will use the following notation to protect reserved area :
    // 0 -> 2
    // 1 -> 3
    // 9 will stay 9
    // 7 will stay 7
    char** matrix = data->mat;
    size_t size = data->size;
    for(size_t x = 0; x < size; ++x) {
        for(size_t y = 0; y < size; ++y) {
            if(matrix[x][y] == 0)
                continue;
            if((x == 0 && y == 0) | (x == size - 8 && y == 0) | 
                    (x == 0 && y == size - 8)) {
                for(int i = x; i < 7; ++i)
                    for(int j = x; j < 7; ++j)
                        matrix[i][j] = 'c';
            }
            if(matrix[x][y] == '1')
                matrix[x][y] = '3';
            if(matrix[x][y] == '0')
                matrix[x][y] = '2';
        }
    }
}

static void setTiming(struct QrCode_Enc* data) {
    int k = 1;
    for(size_t x = 6; x < data->size - 7; ++x, ++k) {
        data->mat[x][6] = '0' + (k % 2);
        data->mat[6][x] = '0' + (k % 2);
    }
}
    static inline
void sub_color_alignment(char **mat, size_t i, size_t j)
{
    for(int x = 0,  k = -2; x < 5; ++x, ++k)
        for(int y = 0, l = -2; y < 5; ++y, ++l) {
            mat[i + k][j + l] = align_pattern[x][y];
        }
}

static void setAlignment(struct QrCode_Enc* data) {
    char** mat = data->mat;
    size_t size = data->size;
    const size_t *ap = Ap_coord[data->data->version-1];

    for(size_t i = 1; i < 8; i++)
    {
        for(size_t j = 1; j < 8; j++)
        { 
            if((mat[ap[i]][ap[j]] != 0 && mat[ap[i]][ap[j]] != '1') || 
                    ap[i] == 0 || ap[j] == 0 || (ap[i] == 6 && ap[j] == 6) ||
                    (ap[i] == 6 && ap[j] == size - 7) || (ap[i] == size - 7 
                        && ap[j] == 6))
                continue;            
            sub_color_alignment(mat, ap[i], ap[j]);
        }
    }  
}


void unprotectMatrix(struct QrCode_Enc* data) {
    size_t size = data->size;
    char** mat = data->mat;
    for(size_t x = 0; x < size; ++x) {
        for(size_t y = 0; y < size; ++y) {
            size_t save_y = y;
            size_t save_x = x;
            if(x == 0 && y == 0) {
                for(int i = 0; i < 7; ++i, ++x) {
                    for(int j = 0; j < 7; ++j, ++y) {
                        mat[x][y] = finder_pattern[i][j];
                    }
                    y = save_y;
                }
                x = save_x;
            }
            if(x == 0 && y == size - 7)  {
                for(int i = 0; i < 7; ++i, ++x) {
                    for(int j = 0; j < 7; ++j, ++y) {
                        mat[x][y] = finder_pattern[i][j];
                    }
                    y = save_y;
                }
                x = save_x;
            }
            if(x == size - 7 && y == 0) {
                for(int i = 0; i < 7; ++i, ++x) {
                    for(int j = 0; j < 7; ++j, ++y) {
                        mat[x][y] = finder_pattern[i][j];
                    }
                    y = save_y;
                }
                x = save_x;
            }
        }
    }
    // Set the separator
    for(size_t x = 0; x < 8; ++x) {
        mat[x][7] = '0';
        mat[x][size - 8] = '0';

        mat[7][x] = '0';
        mat[7][size - 8 + x] = '0';

        mat[size - 8 + x][7] = '0';
        mat[size - 8][x] = '0';
    }
    // Set timing pattern
    setTiming(data);
    // Set alignment pattern at fixed position
    setAlignment(data);
}

