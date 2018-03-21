/*
 **  Demasking QrMatrices
 **  file : demask.c
 **  description : demask qr code matrices, use file with proc_code.c
 */

# include <stdio.h>
# include <stdlib.h>

# include "../headers/mask.h"
# include "../headers/draw.h"

# include "../headers/gen_img.h"

#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)

const char finder_pattern[7][7] = {
    {'1', '1', '1', '1', '1', '1', '1'},
    {'1', '0', '0', '0', '0', '0', '1'},
    {'1', '0', '1', '1', '1', '0', '1'},
    {'1', '0', '1', '1', '1', '0', '1'},
    {'1', '0', '1', '1', '1', '0', '1'},
    {'1', '0', '0', '0', '0', '0', '1'},
    {'1', '1', '1', '1', '1', '1', '1'}
};

const char align_pattern[5][5] = {
    {'1', '1', '1', '1', '1'},
    {'1', '0', '0', '0', '1'},
    {'1', '0', '1', '0', '1'},
    {'1', '0', '0', '0', '1'},
    {'1', '1', '1', '1', '1'},
};


const char prot_finder_pattern[7][7] = {
    {'f', 'f', 'f', 'f', 'f', 'f', 'f'},
    {'f', 'f', 'f', 'f', 'f', 'f', 'f'},
    {'f', 'f', 'f', 'f', 'f', 'f', 'f'},
    {'f', 'f', 'f', 'f', 'f', 'f', 'f'},
    {'f', 'f', 'f', 'f', 'f', 'f', 'f'},
    {'f', 'f', 'f', 'f', 'f', 'f', 'f'},
    {'f', 'f', 'f', 'f', 'f', 'f', 'f'}
};

const char prot_align_pattern[5][5] = {
    {'a', 'a', 'a', 'a', 'a'},
    {'a', 'a', 'a', 'a', 'a'},
    {'a', 'a', 'a', 'a', 'a'},
    {'a', 'a', 'a', 'a', 'a'},
    {'a', 'a', 'a', 'a', 'a'},
};

const size_t Ap_coord[40][8] =
{
    {0, 0, 0, 0, 0, 0, 0, 0},      //1
    {1, 6, 18, 0, 0, 0, 0, 0},     //2
    {1, 6, 22, 0, 0, 0, 0, 0},     //3
    {1, 6, 26, 0, 0, 0, 0, 0},     //4
    {1, 6, 30, 0, 0, 0, 0, 0},     //5
    {1, 6, 34, 0, 0, 0, 0, 0},     //6
    {6, 6, 22, 38, 0, 0, 0, 0},     //7
    {6, 6, 24, 42, 0, 0, 0, 0},     //8
    {6, 6, 26, 46, 0, 0, 0, 0},     //9
    {6, 6, 28, 50, 0, 0, 0, 0},     //10
    {6, 6, 30, 54, 0, 0, 0, 0},     //11
    {6, 6, 32, 58, 0, 0, 0, 0},     //12
    {6, 6, 34, 62, 0, 0, 0, 0},     //13
    {13, 6, 26, 46, 66, 0, 0, 0},    //14
    {13, 6, 26, 48, 70, 0, 0, 0},    //15
    {13, 6, 26, 50, 74, 0, 0, 0},    //16
    {13, 6, 30, 54, 78, 0, 0, 0},    //17
    {13, 6, 30, 56, 82, 0, 0, 0},    //18
    {13, 6, 30, 58, 86, 0, 0, 0},    //19
    {13, 6, 34, 62, 90, 0, 0, 0},    //20
    {22, 6, 28, 50, 72, 94, 0, 0},    //21
    {22, 6, 26, 50, 74, 98, 0, 0},    //22
    {22, 6, 30, 54, 78, 102, 0, 0},    //23
    {22, 6, 28, 54, 80, 106, 0, 0},    //24
    {22, 6, 32, 58, 84, 110, 0, 0},    //25
    {22, 6, 30, 58, 86, 114, 0, 0},    //26
    {22, 6, 34, 62, 90, 118, 0, 0},    //27
    {33, 6, 26, 50, 74, 98, 122, 0},    //28
    {33, 6, 30, 54, 78, 102, 126, 0},    //29
    {33, 6, 26, 52, 78, 104, 130, 0},    //30
    {33, 6, 30, 56, 82, 108, 134, 0},    //31
    {33, 6, 34, 60, 86, 112, 138, 0},    //32
    {33, 6, 30, 58, 86, 114, 142, 0},    //33
    {33, 6, 34, 63, 90, 118, 146, 0},    //34
    {46, 6, 30, 54, 78, 102, 126, 150},    //35
    {46, 6, 24, 50, 76, 102, 128, 154},    //36
    {46, 6, 28, 54, 80, 106, 132, 158},    //37
    {46, 6, 32, 58, 84, 110, 136, 162},    //38
    {46, 6, 26, 54, 82, 110, 138, 166},    //39
    {46, 6, 30, 58, 86, 114, 142, 170}    //40
};


// SUB_FUNCTIONS

    static inline
void demask0(char **mat, size_t size)
{
    for(size_t i = 0; i < size; i++)
    {
        for(size_t j = 0; j < size; j++)
        {
            if((i + j) % 2 == 0)
            {
                if(mat[i][j] == '1')
                    mat[i][j] = '0';
                else if(mat[i][j] == '0')
                    mat[i][j] = '1';
                else
                    continue;
            }
        }
    }
}

    static inline
void demask1(char **mat, size_t size)
{
    for(size_t i = 0; i < size; i++)
    {
        for(size_t j = 0; j < size; j++)
        {
            if(i % 2 == 0)
            {
                if(mat[i][j] == '1')
                    mat[i][j] = '0';
                else if(mat[i][j] == '0')
                    mat[i][j] = '1';
            }
        }
    }
}

    static inline
void demask2(char **mat, size_t size)
{
    for(size_t i = 0; i < size; i++)
    {
        for(size_t j = 0; j < size; j++)
        {
            if(j % 3 == 0)
            {
                if(mat[i][j] == '1')
                    mat[i][j] = '0';
                else if(mat[i][j] == '0')
                    mat[i][j] = '1';
            }
        }
    }
}

    static inline
void demask3(char **mat, size_t size)
{
    for(size_t i = 0; i < size; i++)
    {
        for(size_t j = 0; j < size; j++)
        {
            if((i + j) % 3 == 0)
            {
                if(mat[i][j] == '1')
                    mat[i][j] = '0';
                else if(mat[i][j] == '0')
                    mat[i][j] = '1';
            }
        }
    }
}

    static inline
void demask4(char **mat, size_t size)
{
    for(size_t i = 0; i < size; i++)
    {
        for(size_t j = 0; j < size; j++)
        {
            if(((size_t)floor(i / 2) + (size_t)floor(j / 3)) % 2 == 0)
            {
                if(mat[i][j] == '1')
                    mat[i][j] = '0';
                else if(mat[i][j] == '0')
                    mat[i][j] = '1';
            }
        }
    }
}

    static inline
void demask5(char **mat, size_t size)
{
    for(size_t i = 0; i < size; i++)
    {
        for(size_t j = 0; j < size; j++)
        {
            if((((i * j) % 2) + ((i * j) % 3)) == 0)
            {
                if(mat[i][j] == '1')
                    mat[i][j] = '0';
                else if(mat[i][j] == '0')
                    mat[i][j] = '1';
            }
        }
    }
}

    static inline
void demask6(char **mat, size_t size)
{
    for(size_t i = 0; i < size; i++)
    {
        for(size_t j = 0; j < size; j++)
        {
            if((((i * j) % 2) + ((i * j) % 3)) % 2 == 0)
            {
                if(mat[i][j] == '1')
                    mat[i][j] = '0';
                else if(mat[i][j] == '0')
                    mat[i][j] = '1';
            }
        }
    }
}

    static inline
void demask7(char **mat, size_t size)
{
    for(size_t i = 0; i < size; i++)
    {
        for(size_t j = 0; j < size; j++)
        {
            if((((i + j) % 2) + ((i * j) % 3)) % 2 == 0)
            {
                if(mat[i][j] == '1')
                    mat[i][j] = '0';
                else if(mat[i][j] == '0')
                    mat[i][j] = '1';
            }
        }
    }
}

// MAIN_FUNCTION

void applyMask(char **mat, size_t size, int mask)
{
    switch(mask) {
        case 1 : {
                     demask1(mat, size);
                     break;
                 }
        case 2 : {
                     demask2(mat, size);
                     break;
                 }
        case 3 :
                 {
                     demask3(mat, size);
                     break;
                 }
        case 4 : {
                     demask4(mat, size);
                     break;
                 }
        case 5 : {
                     demask5(mat, size);
                     break;
                 }
        case 6 : {
                     demask6(mat, size);
                     break;
                 }
        case 7 : {
                     demask7(mat, size);
                     break;
                 }
        default : {
                      demask0(mat, size);
                      break;
                  }
    }
}

static inline int check_cpt(int cpt)
{
    if (cpt >= 5)
    {
        if (cpt == 5)
            return 3;
        return 1;
    }
    return 0;
}

static inline void check_status(int status)
{
    if(status == 1)
        printf("\033[0;31m");
    else
        printf("\033[0;34m");
}

// condition #1 similar color consecutive
int sequential_Eval(char** mat, size_t size) {
    // declare the two penality var (horizontal and vertical)
    int h_penality = 0, v_penality = 0;
    // declare a counter to keep the number of consecutive bits (0/1)
    int cpt = 0;
    // declare the last visited bit var to check
    char lbit = mat[0][0];


    // tmp var to keep track of status (changed yes/no)
    //int status = 0;

    // the horizontal block
    for(size_t x = 0; x < size; ++x)
    {
        int line_val = 0;
        cpt = 0;
        for(size_t y = 0; y < size; ++y)
        {
            if(mat[x][y] == lbit)
                cpt += 1;
            else
            {
                //status = status == 1 ? 0 : 1;
                cpt = 1;
                lbit = mat[x][y];
            }
            //check_status(status);
            //printf(" %c ", mat[x][y]);
            // we add the possible penality in function of cpt
            line_val += check_cpt(cpt);
        }
        //printf("\033[0m");
        //printf(" = %3i\n", line_val);
        h_penality += line_val;
    }
    /*printf("\n\033[0m");
      printf("Total line = %3i\n", h_penality);*/

    // we reset the counter and last bit
    cpt = 0, lbit = mat[0][0];

    // the vertical block
    for(size_t y = 0; y < size; ++y)
    {
        int col_val = 0;
        cpt = 0;
        for(size_t x = 0; x < size; ++x)
        {
            if(mat[x][y] == lbit)
                cpt += 1;
            else
            {
                cpt = 1;
                lbit = mat[x][y];
            }
            col_val += check_cpt(cpt);
        }
        v_penality += col_val;
    }
    //printf("Total column = %3i\n", v_penality);
    // return the total penality of this condition
    return h_penality + v_penality;
}

// Condition #2 looking for 2*2 blocks (3 points per block)
int block_Eval(char** mat, size_t size) {
    int penality = 0;

    for(size_t x = 0; x + 1 < size; ++x) 
    {
        for(size_t y = 0; y + 1 < size; ++y)
        {
            if(mat[x][y] == mat[x + 1][y + 1])
                if(mat[x][y] == mat[x][y + 1])
                    if(mat[x][y + 1] == mat[x + 1][y])
                        penality += 1;
        }
    }
    return penality * 3;
}

// Condition #3 we looking for 2 patterns
int pattern_Eval(char** mat, size_t size) {
    int penality = 0;
    // 1 : black and 0 : white
    char* patterns[2] = { "10111010000", "00001011101" };

    int test = 1;
    for(size_t x = 0; x < size; ++x) 
    {
        for(size_t y = 0; y < size; ++y)
        {
            for(int p = 0; p < 2; ++p)
            {
                test = 1;
                if( y + 10 < size )
                {
                    for(size_t cur = 0, i = 0; cur < 11 && test == 1; ++cur, ++i)
                    {
                        if(mat[x][y + i] != patterns[p][cur])
                            test = 0;
                    }
                    if(test == 1)
                        penality += 40;
                }
                if( x + 10 < size )
                {
                    test = 1;
                    for(size_t cur = 0, i = 0; cur < 11 && test == 1; ++cur, ++i)
                    {
                        if(mat[x + i][y] != patterns[p][cur])
                            test = 0;
                    }
                    if(test == 1)
                        penality += 40;
                }
            }
        }
    }
    return penality;
}

int ratio_Eval(char** mat, size_t size) {
    int penality = 0;
    size_t dark_cells = 0;
    for(size_t x = 0; x < size; ++x) {
        for(size_t y = 0; y < size; ++y) {
            if(mat[x][y] == '1')
                dark_cells += 1;
        }
    }
    float ratio = ((float)dark_cells / (float)(size * size)) * 100;
    float prev_mult = ratio + 5;
    float next_mult = 1;
    while(prev_mult > ratio)
        prev_mult -= 5;
    while(next_mult < ratio)
        next_mult += 5;
    prev_mult = abs(prev_mult - 50);
    next_mult = abs(next_mult - 50);
    prev_mult = prev_mult / 5;
    next_mult = next_mult / 5;

    penality = min(prev_mult, next_mult) * 10;
    return penality;
}

int** evaluate(struct QrCode_Enc* data, int version, int cor) {
    int** penalty = malloc(8 * sizeof(int*));
    char** mat = data->mat;
    for(int mask = 0; mask < 8; ++mask) {
        setFormatString(data, S_bits[cor][mask]);
        if(version > 6)
            setVersionString(data, V_bits[version]);

        // Preprocessing (applying the mask)
        applyMask(mat, data->size, mask);

        setFormatString(data, S_bits[cor][mask]);
        if(version > 6)
            setVersionString(data, V_bits[version]);

        unprotectMatrix(data);

        penalty[mask] = malloc(5 * sizeof(int));
        for(int condition = 0; condition < 4; ++condition) {
            /*// Preprocessing (applying the mask)
              applyMask(mat, data->size, mask);

              setFormatString(data, S_bits[cor][mask]);
              if(version > 6)
              setVersionString(data, V_bits[version]);

              unprotectMatrix(data);

            //Generate_QrCode(mat, version, "t.bmp", 8);*/

            // evaluation bloc
            switch(condition) {
                case 1 : // block contition
                    penalty[mask][1] = block_Eval(mat, data->size);
                    break;
                case 2 : // similar pattern condition
                    penalty[mask][2] = pattern_Eval(mat, data->size);
                    break;
                case 3 : // ratio of light/dark
                    penalty[mask][3] = ratio_Eval(mat, data->size);
                    break;
                default : // consecutive color condition
                    penalty[mask][0] = sequential_Eval(mat, data->size);
                    break;
            }

            /*protectMatrix(data);
            // PostProcessing (reverting the mask)
            applyMask(mat, data->size, mask);*/

        }
        protectMatrix(data);
        // PostProcessing (reverting the mask)
        applyMask(mat, data->size, mask);
        penalty[mask][4] = penalty[mask][0] + penalty[mask][1]
            + penalty[mask][2] + penalty[mask][3];
    }

    return penalty;
}

void setTiming(struct QrCode_Enc* data) {
    int k = 1;
    for(size_t x = 6; x < data->size - 7; ++x, ++k) {
        data->mat[x][6] = 't';//'0' + (k % 2);
        data->mat[6][x] = 't';//'0' + (k % 2);
    }
}
    static inline
void sub_color_alignment(char **mat, size_t i, size_t j)
{
    for(int x = 0,  k = -2; x < 5; ++x, ++k)
        for(int y = 0, l = -2; y < 5; ++y, ++l) {
            mat[i + k][j + l] = 'a';//align_pattern[x][y];
        }
}

void setAlignment(struct QrCode_Enc* data) {
    char** mat = data->mat;
    size_t size = data->size;
    const size_t *ap = Ap_coord[data->data->version-1];

    for(size_t i = 1; i < 8; i++)
    {
        for(size_t j = 1; j < 8; j++)
        {
            //if(ap[i] != 0 && ap[j] != 0)
            //    printf("%c\n", mat[ap[i]][ap[j]]);
            if((mat[ap[i]][ap[j]] != 0 && (mat[ap[i]][ap[j]] != '1' && mat[ap[i]][ap[j]] != 't')) || 
                    ap[i] == 0 || ap[j] == 0 || (ap[i] == 6 && ap[j] == 6) ||
                    (ap[i] == 6 && ap[j] == size - 7) || (ap[i] == size - 7 
                        && ap[j] == 6))
                continue;            
            sub_color_alignment(mat, ap[i], ap[j]);
        }
    }  
}

void initMatrix(struct QrCode_Enc* data) {
    size_t size = data->size;
    char** mat = data->mat;
    for(size_t x = 0; x < size; ++x) {
        for(size_t y = 0; y < size; ++y) {
            size_t save_y = y;
            size_t save_x = x;
            if(x == 0 && y == 0) {
                for(int i = 0; i < 7; ++i, ++x) {
                    for(int j = 0; j < 7; ++j, ++y) {
                        mat[x][y] = prot_finder_pattern[i][j];
                    }
                    y = save_y;
                }
                x = save_x;
            }
            if(x == 0 && y == size - 7)  {
                for(int i = 0; i < 7; ++i, ++x) {
                    for(int j = 0; j < 7; ++j, ++y) {
                        mat[x][y] = prot_finder_pattern[i][j];
                    }
                    y = save_y;
                }
                x = save_x;
            }
            if(x == size - 7 && y == 0) {
                for(int i = 0; i < 7; ++i, ++x) {
                    for(int j = 0; j < 7; ++j, ++y) {
                        mat[x][y] = prot_finder_pattern[i][j];
                    }
                    y = save_y;
                }
                x = save_x;
            }
        }
    }
    // Set the separator
    for(size_t x = 0; x < 8; ++x) {
        mat[x][7] = 's';//'0';
        mat[x][size - 8] = 's';//'0';

        mat[7][x] = 's';//'0';
        mat[7][size - 8 + x] = 's';//'0';

        mat[size - 8 + x][7] = 's';//'0';
        mat[size - 8][x] = 's';//'0';
    }
    // Set timing pattern
    setTiming(data);
    // Set alignment pattern at fixed position
    setAlignment(data);
    // Set Dark Module
    mat[(4 * data->data->version) + 9][8] = '1';//'9';

    // Reserve the format information area
    for(size_t x = 0; x < 8; ++x) { 
        mat[size - 8 + x][8] = mat[size - 8 + x][8] == 0 ? 'c' : mat[size - 8 + x][8];
        mat[x][8] = mat[x][8] == 0 ? 'c' : mat[x][8];
        mat[8][x] = mat[8][x] == 0 ? 'c' : mat[8][x];
        mat[8][size - 8 + x] = mat[8][size - 8 + x] == 0 ? 'c' : mat[8][size - 8 + x];
        mat[8][8] = 'c';
    }
    if(data->data->version >= 7)
    {
        for(size_t x = 0; x < 6; ++x) {
            mat[x][size - 11] = 'v';//'7';
            mat[x][size - 10] = 'v';//'7';
            mat[x][size - 9] = 'v';//'7';

            mat[size - 11][x] = 'v';//'7';
            mat[size - 10][x] = 'v';//'7';
            mat[size - 9][x] = 'v';//'7';
        }
    }
}

struct QrCode_Enc* initQrCode(struct EncData* data) {
    struct QrCode_Enc* QrCode = malloc(sizeof(struct QrCode_Enc));

    size_t size = (4 * (data->version)) + 17;

    char** matrix = calloc(size, sizeof(char*));
    for(size_t i = 0; i < size; ++i)
        matrix[i] = calloc(size, sizeof(char));

    QrCode->size = size;
    QrCode->mat = matrix;
    QrCode->data = data;

    // generate matrix
    initMatrix(QrCode);

    // we protect the matrix to avoid corruption of vital area
    //protectMatrix(QrCode);

    // translate content into matrix
    //fill_mat(QrCode->mat, size, data->version, data->enc, QrCode->size_enc);

    // unprotect the matrix to have correct value
    //unprotectMatrix(QrCode);

    // return QrCode struct
    return QrCode;
}
