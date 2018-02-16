/*
**  QrCode Code Processor
**  file: proc_code.c
**  description : takes in the QrCode struct and returns the PCode struct, which
**  contains the version, the mask code, the error correction, the 10bits of
**  errors/format correction
*/

# include "proc_code.h"

// STATIC FUNCTION

static inline
char *strbinXOR(char* a, char *b)
{
    if(strlen(a) != strlen(b))
    {
        warn("XOR Length error");
    }
    
    size_t strend = strlen(a);
    char *ret = malloc(sizeof(char) * strend);
    for(size_t i = 0; i < strend; i++)
    {
        if(a[i] != b[i])
            ret[i] = '1';
        else
            ret[i] = '0';
    }
    return ret;
}

static inline 
int bin_int(char* a)
{
    size_t len = strlen(a);
    //warn("string size : %lu", len);
    int ret = 0;
    
    for(size_t i = 0; i < len; i++)
    {
        if(a[i] == '1')
            ret += pow(2, len-i-1);
    }
    return ret;
}

static inline
char get_correction_lvl(char* a)
{
    int val = bin_int(a);
    
    if(val == 0)
        return 'M';
    else if(val == 1)
        return 'L';
    else if(val == 2)
        return 'H';
    else if(val == 3)
        return 'Q';

    return EXIT_FAILURE;
}

static inline
char *get_format1(char **mat)
{
    char *fmt = malloc(sizeof(char) * 15);
    fmt[0] = mat[8][0];
    fmt[1] = mat[8][1];
    fmt[2] = mat[8][2];
    fmt[3] = mat[8][3];
    fmt[4] = mat[8][4];
    fmt[5] = mat[8][5];
    fmt[6] = mat[8][7];
    fmt[7] = mat[8][8];
    fmt[8] = mat[7][8];
    fmt[9] = mat[5][8];
    fmt[10] = mat[4][8];
    fmt[11] = mat[3][8];
    fmt[12] = mat[2][8];
    fmt[13] = mat[1][8];
    fmt[14] = mat[0][8];
    mat[8][0] ='c';
    mat[8][1] ='c';
    mat[8][2] ='c';
    mat[8][3] ='c';
    mat[8][4] ='c';
    mat[8][5] ='c';
    mat[8][7] ='c';
    mat[8][8] ='c';
    mat[7][8] ='c';
    mat[5][8] ='c';
    mat[4][8] ='c';
    mat[3][8] ='c';
    mat[2][8] ='c';
    mat[1][8] ='c';
    mat[0][8] ='c';
    return fmt;
}

static inline
char *get_format2(char **mat, int version)
{
    int size = 4 * version + 17;
 
    char *fmt = malloc(sizeof(char) * 15);
    fmt[0] = mat[size-1][8];
    fmt[1] = mat[size-2][8];
    fmt[2] = mat[size-3][8];
    fmt[3] = mat[size-4][8];
    fmt[4] = mat[size-5][8];
    fmt[5] = mat[size-6][8];
    fmt[6] = mat[size-7][8];
    fmt[7] = mat[8][size-8];
    fmt[8] = mat[8][size-7];
    fmt[9] = mat[8][size-6];
    fmt[10] = mat[8][size-5];
    fmt[11] = mat[8][size-4];
    fmt[12] = mat[8][size-3];
    fmt[13] = mat[8][size-2];
    fmt[14] = mat[8][size-1];
    mat[size-1][8] = 'c';
    mat[size-2][8] = 'c';
    mat[size-3][8] = 'c';
    mat[size-4][8] = 'c';
    mat[size-5][8] = 'c';
    mat[size-6][8] = 'c';
    mat[size-7][8] = 'c';
    mat[8][size-8] = 'c';
    mat[8][size-7] = 'c';
    mat[8][size-6] = 'c';
    mat[8][size-5] = 'c';
    mat[8][size-4] = 'c';
    mat[8][size-3] = 'c';
    mat[8][size-2] = 'c';
    mat[8][size-1] = 'c';
    return fmt;   
}

static inline
void color_timing_pat(char **mat, int size)
{
    for(int i = 8; i < size - 8; i++)
    {
        mat[i][6] = 't';
        mat[6][i] = 't';
    }
}

static inline
void color_finders(char **mat, int size)
{
    //A
    for(int i = 0; i < 7; i++)
    {
        for(int j = 0; j < 7; j++)
        {
            mat[i][j] = 'f';
        }
    }

    //B
    for(int i = size - 7; i < size; i++)
    {
        for(int j = 0; j < 7; j++)
        {
            mat[i][j] = 'f';
        }
    }
    
    //C
    for(int i = 0; i < 7; i++)
    {
        for(int j = size - 7; j < size; j++)
        {
            mat[i][j] = 'f';
        }
    }
}

static inline
void color_separators(char **mat, int size)
{
    for(int i = 0; i < 8; i++)
    {   
        mat[7][i] = 's';
    }
    
    for(int i = 0; i < 8; i++)
    {   
        mat[i][7] = 's';
    }

    for(int i = size - 8; i < size; i++)
    {   
        mat[7][i] = 's';
    }

    for(int i = size - 8; i < size; i++)
    {   
        mat[i][7] = 's';
    }

    for(int i = 0; i < 8; i++)
    {   
        mat[i][size - 8] = 's';
    }

    for(int i = 0; i < 8; i++)
    {   
        mat[size - 8][i] = 's';
    }       
}

static inline
void sub_color_alignment(char **mat, int i, int j)
{
    //warn("sub function");
    mat[i][j] = 'a';
    mat[i - 1][j] = 'a';
    mat[i - 1][j - 1] = 'a';
    mat[i - 1][j - 2] = 'a';
    mat[i - 1][j + 1] = 'a';
    mat[i - 1][j + 2] = 'a';
    mat[i - 2][j - 1] = 'a';
    mat[i - 2][j - 2] = 'a';
    mat[i - 2][j + 1] = 'a';
    mat[i - 2][j + 2] = 'a';
    mat[i - 2][j] = 'a';
    mat[i + 1][j] = 'a';
    mat[i + 1][j - 1] = 'a';
    mat[i + 1][j - 2] = 'a';
    mat[i + 1][j + 1] = 'a';
    mat[i + 1][j + 2] = 'a';
    mat[i + 2][j] = 'a';
    mat[i + 2][j - 1] = 'a';
    mat[i + 2][j - 2] = 'a';
    mat[i + 2][j + 1] = 'a';
    mat[i + 2][j + 2] = 'a';
    mat[i][j - 1] = 'a';
    mat[i][j - 2] = 'a';
    mat[i][j + 1] = 'a';
    mat[i][j + 2] = 'a';
}

static inline
void color_alignment(char **mat, int version)
{
    const int *ap = Ap_coord[version-1];
    
    for(int i = 1; i < 8; i++)
    {
        for(int j = 1; j < 8; j++)
        { 
            //warn("in loop");
            if(mat[ap[i]][ap[j]] != '0' && mat[ap[i]][ap[j]] != '1')
            {       
                    //warn("bad coord %d %d", ap[i], ap[j]);
                    continue;
            }
            
            //warn("good coord %d %d", ap[i], ap[j]);
            sub_color_alignment(mat, ap[i], ap[j]);
        }
    }  
}

static inline
void color_spec_pat(char **mat, int version)
{
    int size = 4 * version + 17;
    mat[4 * version + 9][8] = 'd';
    color_finders(mat, size);
    color_separators(mat, size);
    color_timing_pat(mat, size);
    color_alignment(mat, version);
}   

// MAIN FUNCTION

struct PCode *get_code(struct QrCode *qr)
{
    struct PCode *code = malloc(sizeof(struct PCode));
    
    color_spec_pat(qr->mat, qr->version);
    
    char *fmt1 = get_format1(qr->mat);
    char *fmt2 = get_format2(qr->mat, qr->version);
    //warn("fmt1 %s, fmt2 %s", fmt1, fmt2);
    
    if (strcmp(fmt1, fmt2) != 0) // for now we will just use fmt1
        warn("fmt1 != fmt2");
    
    char *key = "101010000010010";
    char *xored = strbinXOR(fmt1, key); 
    //warn("xored %s", xored);
    
    char *err_cor_lvl = calloc(2, sizeof(char));
    char *mask = calloc(3, sizeof(char));
    err_cor_lvl[0] = xored[0];
    err_cor_lvl[1] = xored[1];
    mask[0] = xored[2];
    mask[1] = xored[3];
    mask[2] = xored[4];
    
    int maskv = bin_int(mask);
    char cor_lvl = get_correction_lvl(err_cor_lvl);
    
    warn("error level correction %s %c, mask %s %d", err_cor_lvl, cor_lvl,  mask, maskv);
   
    size_t size = qr->version * 4 + 17;
    
    demask(qr->mat, size, maskv);  
 
    return code;
}

