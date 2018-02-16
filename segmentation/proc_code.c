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
    return fmt;   
}


// MAIN FUNCTION

struct PCode *get_code(struct QrCode *qr)
{
    struct PCode *code = malloc(sizeof(struct PCode));
    char *fmt1 = get_format1(qr->mat);
    char *fmt2 = get_format2(qr->mat, qr->version);
    warn("fmt1 %s, fmt2 %s", fmt1, fmt2);
    
    if (strcmp(fmt1, fmt2) != 0) // for now we will just use fmt1
        warn("fmt1 != fmt2");
    
    char *key = "101010000010010";
    char *xored = strbinXOR(fmt1, key); 
    warn("xored %s", xored);
    
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

