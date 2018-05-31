# include <stdio.h>
# include <stdlib.h>

# include "../headers/draw.h"
# include "../headers/mask.h"
# include "../headers/encode.h"


static void setTiming(struct QrCode_Enc* data, int protected) {
    int k = 1;
    if(protected == 0) {
        for(size_t x = 6; x < data->size - 7; ++x, ++k) {
            data->mat[x][6] = '0' + (k % 2);
            data->mat[6][x] = '0' + (k % 2);
        }
    }
    else {
        for(size_t x = 6; x < data->size - 7; ++x, ++k) {
            data->mat[x][6] = 't';
            data->mat[6][x] = 't';
        }
    }
}
    static inline
void sub_color_alignment(char **mat, size_t i, size_t j, int protected)
{
    if(protected == 0) {
        for(int x = 0,  k = -2; x < 5; ++x, ++k)
            for(int y = 0, l = -2; y < 5; ++y, ++l) {
                mat[i + k][j + l] = align_pattern[x][y];
            }
    }
    else {
        for(int x = 0,  k = -2; x < 5; ++x, ++k)
            for(int y = 0, l = -2; y < 5; ++y, ++l) {
                mat[i + k][j + l] = 'a';
            }
    }

}

static void setAlignment(struct QrCode_Enc* data, int protected) {
    char** mat = data->mat;
    size_t size = data->size;
    const size_t *ap = Ap_coord[data->data->version - 1];

    for(size_t i = 1; i < 8; i++)
    {
        for(size_t j = 1; j < 8; j++)
        {
            if(ap[i] == 0 || ap[j] == 0)
                continue;
            if(protected == 0) {
                if((ap[i] == 6 && ap[j] == 6) ||
                        (ap[i] == 6 && ap[j] == size - 7) || (ap[i] == size - 7 
                            && ap[j] == 6))
                    continue;            
                sub_color_alignment(mat, ap[i], ap[j], protected);
            }
            else {
                if((ap[i] == 6 && ap[j] == 6) ||
                        (ap[i] == 6 && ap[j] == size - 7) || (ap[i] == size - 7 
                            && ap[j] == 6))
                    continue;            
                sub_color_alignment(mat, ap[i], ap[j], protected);
            }
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
    // Set the dark module
    mat[size - 8][8] = '1';

    // Set timing pattern
    setTiming(data, 0);
    // Set alignment pattern at fixed position
    setAlignment(data, 0);
}

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
            size_t save_y = y;
            size_t save_x = x;
            if(x == 0 && y == 0) {
                for(int i = 0; i < 7; ++i, ++x) {
                    for(int j = 0; j < 7; ++j, ++y) {
                        matrix[x][y] = 'f';
                    }
                    y = save_y;
                }
                x = save_x;
            }
            if(x == 0 && y == size - 7)  {
                for(int i = 0; i < 7; ++i, ++x) {
                    for(int j = 0; j < 7; ++j, ++y) {
                        matrix[x][y] = 'f';
                    }
                    y = save_y;
                }
                x = save_x;
            }
            if(x == size - 7 && y == 0) {
                for(int i = 0; i < 7; ++i, ++x) {
                    for(int j = 0; j < 7; ++j, ++y) {
                        matrix[x][y] = 'f';
                    }
                    y = save_y;
                }
                x = save_x;
            }
        }
    }

    // protect the separator
    for(size_t x = 0; x < 8; ++x) {
        matrix[x][7] = 's';
        matrix[x][size - 8] = 's';

        matrix[7][x] = 's';
        matrix[7][size - 8 + x] = 's';

        matrix[size - 8 + x][7] = 's';
        matrix[size - 8][x] = 's';
    }

    // protect the string information
    for(size_t x = 0; x < 8; ++x) {
        if(x >= 6) {
            matrix[x + 1][8] += 2;
            matrix[8][x + 1] += 2;
        }
        else {
            matrix[x][8] += 2;
            matrix[8][x] += 2;
        }
        if (x != 0)
            matrix[size - 8 + x][8] += 2;
        matrix[8][size - 8 + x] += 2;
    }

    // protect the dark module
    matrix[size - 8][8] = '9';

    // protect the timing
    setTiming(data, 1);
    // protect the aligment pattern
    setAlignment(data, 1);
}

void unprotectMatrix_B(struct QrCode_Enc* data) {
    // we will use the following notation to protect reserved area :
    // 0 -> 2
    // 1 -> 3
    // 9 will stay 9
    // 7 will stay 7
    char** matrix = data->mat;
    size_t size = data->size;
    for(size_t x = 0; x < size; ++x) {
        for(size_t y = 0; y < size; ++y) {
            size_t save_y = y;
            size_t save_x = x;
            if(x == 0 && y == 0) {
                for(int i = 0; i < 7; ++i, ++x) {
                    for(int j = 0; j < 7; ++j, ++y) {
                        matrix[x][y] = finder_pattern[i][j];
                    }
                    y = save_y;
                }
                x = save_x;
            }
            if(x == 0 && y == size - 7)  {
                for(int i = 0; i < 7; ++i, ++x) {
                    for(int j = 0; j < 7; ++j, ++y) {
                        matrix[x][y] = finder_pattern[i][j];
                    }
                    y = save_y;
                }
                x = save_x;
            }
            if(x == size - 7 && y == 0) {
                for(int i = 0; i < 7; ++i, ++x) {
                    for(int j = 0; j < 7; ++j, ++y) {
                        matrix[x][y] = finder_pattern[i][j];
                    }
                    y = save_y;
                }
                x = save_x;
            }
        }
    }
    // Set the separator
    for(size_t x = 0; x < 8; ++x) {
        matrix[x][7] = '0';
        matrix[x][size - 8] = '0';

        matrix[7][x] = '0';
        matrix[7][size - 8 + x] = '0';

        matrix[size - 8 + x][7] = '0';
        matrix[size - 8][x] = '0';
    }

    // set the string information
    for(size_t x = 0; x < 8; ++x) {
        if(x >= 6) {
            matrix[x + 1][8] -= 2;
            matrix[8][x + 1] -= 2;
        }
        else {
            matrix[x][8] -= 2;
            matrix[8][x] -= 2;
        }
        if (x != 0)
            matrix[size - 8 + x][8] -= 2;
        matrix[8][size - 8 + x] -= 2;
    }

    // Set the dark module
    matrix[size - 8][8] = '1';

    // Set timing pattern
    setTiming(data, 0);
    // Set alignment pattern at fixed position
    setAlignment(data, 0);
}

void setFormatString(struct QrCode_Enc* data, const char format[15]) {
    size_t size = data->size;
    char** mat = data->mat;

    for(size_t x = 0; x < 8; ++x) {
        if(x >= 6) {
            mat[x + 1][8] = format[14 - x];
            mat[8][x + 1] = format[x];
        }
        else {
            mat[x][8] = format[14 - x];
            mat[8][x] = format[x];
        }
        if (x != 0)
            mat[size - 8 + x][8] = format[7 - x];
        mat[8][size - 8 + x] = format[7 + x];
    }
}

void setVersionString(struct QrCode_Enc* data, const char version[18]) {
    size_t size = data->size;
    char** mat = data->mat;

    for(size_t x = 0; x < 6; ++x) {
        mat[x][size - 11] = version[3 * x];
        mat[x][size - 10] = version[3 * x + 1];
        mat[x][size - 9] = version[3 * x + 2];

        mat[size - 11][x] = version[3 * x];
        mat[size - 10][x] = version[3 * x + 1];
        mat[size - 9][x] = version[3 * x + 2];
    }
}
