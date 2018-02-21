# include <stdio.h>
# include <stdlib.h>

# include "../headers/draw.h"
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
            if(matrix[x][y] == '1')
                matrix[x][y] = '3';
            if(matrix[x][y] == '0')
                matrix[x][y] = '2';
        }
    }
}

void unprotectMatrix(struct QrCode_Enc* data) {
    // 2 -> 0
    // 3 -> 1
    // 9 will stay 9
    // 7 will stay 7
    char** matrix = data->mat;
    size_t size = data->size;
    for(size_t x = 0; x < size; ++x) {
        for(size_t y = 0; y < size; ++y) {
            if(matrix[x][y] == 0)
                continue;
            if(matrix[x][y] == '3')
                matrix[x][y] = '1';
            if(matrix[x][y] == '2')
                matrix[x][y] = '0';
        }
    }
}

void fillMatrix(struct QrCode_Enc* data, char** input) {
    size_t size = data->size;
    
    size_t startx = size - 1;
    size_t starty = size - 1;
}
