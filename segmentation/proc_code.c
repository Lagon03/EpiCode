/*
**  file: proc_code.c
**  description : takes in the QrCode struct and returns the PCode struct, which
**  contains the version, the mask code, the error correction, the 10bits of
**  errors/format correction
*/

# include "proc_code.h"
# include "constfile.h"

// STATIC FUNCTION

static const char* F_bits[32] =
{ 
        "111011111000100", // Low
        "111001011110011",
        "111110110101010",
        "111100010011101",
        "110011000101111",
        "110001100011000",
        "110110001000001",
        "110100101110110",  
        "101010000010010", // Medium
        "101000100100101",
        "101111001111100",
        "101101101001011",
        "100010111111001",
        "100000011001110",
        "100111110010111",
        "100101010100000", 
        "011010101011111", // Quartile
        "011000001101000",
        "011111100110001",
        "011101000000110",
        "010010010110100",
        "010000110000011",
        "010111011011010",
        "010101111101101",
        "001011010001001", // High
        "001001110111110",
        "001110011100111",
        "001100111010000",
        "000011101100010",
        "000001001010101",
        "000110100001100",
        "000100000111011" 
};

static inline
void print_mat(char **mat, int size)
{
    printf("\n");
    for(int i = 0; i < size; i++)
    {
        printf("[ ");
        for(int j = 0; j < size; j++)
        {
            printf("%c ", mat[i][j]);
        }
        printf("]\n");
    }
    printf("\n");
}

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
    fmt[14] = mat[0][8];
    fmt[13] = mat[1][8];
    fmt[12] = mat[2][8];
    fmt[11] = mat[3][8];
    fmt[10] = mat[4][8];
    fmt[9] = mat[5][8];
    fmt[8] = mat[7][8];
    fmt[7] = mat[8][8];
    fmt[6] = mat[8][7];
    fmt[5] = mat[8][5];
    fmt[4] = mat[8][4];
    fmt[3] = mat[8][3];
    fmt[2] = mat[8][2];
    fmt[1] = mat[8][1];
    fmt[0] = mat[8][0];
    //printf("%s \n", fmt);
    return fmt;
}

static inline
char *get_format2(char **mat, int version)
{
    int size = 4 * version + 17;
 
    char *fmt = malloc(sizeof(char) * 15);
    fmt[14] = mat[8][size-1];
    fmt[13] = mat[8][size-2];
    fmt[12] = mat[8][size-3];
    fmt[11] = mat[8][size-4];
    fmt[10] = mat[8][size-5];
    fmt[9] = mat[8][size-6];
    fmt[8] = mat[8][size-7];
    fmt[7] = mat[8][size-8];
    fmt[6] = mat[size-7][8];
    fmt[5] = mat[size-6][8];
    fmt[4] = mat[size-5][8];
    fmt[3] = mat[size-4][8];
    fmt[2] = mat[size-3][8];
    fmt[1] = mat[size-2][8];
    fmt[0] = mat[size-1][8];
    return fmt;   
}

static inline
int CorrectFormat(char *fmt)
{
    int besti = 0;
    int bestdiff = 4;
    int diff = 0;
    
    for(int i = 0; i < 32; i++)
    {
        diff = 0;
        for(int j = 0; j < 15; j++)
        {
            if(fmt[j] != F_bits[i][j])
                diff++; 
        }
       
        //printf("%s %s %d\n", fmt, F_bits[i], diff);
         
        if(diff < bestdiff)
        {
            if(diff == 0)
                return i;
            bestdiff = diff;    
            besti = i;
            
        }
            
    }
    
    if(bestdiff >= 3)
        return -1;
    
    return besti;    
}

static inline
int CorrectFormatInv(char *fmt)
{
    int besti = 0;
    int bestdiff = 4;
    int diff = 0;
    
    for(int i = 0; i < 32; i++)
    {
        diff = 0;
        for(int j = 0; j < 15; j++)
        {
            if(fmt[14 - j] != F_bits[i][j])
                diff++; 
        }
        
        //printf("%s %s %d\n", fmt, F_bits[i], diff);
        
        if(diff < bestdiff)
        {
            if(diff == 0)
                return i;
            bestdiff = diff;    
            besti = i;
            
        }
            
    }
    
    if(bestdiff >= 3)
        return -1;
    
    return besti;    
}

static inline
char GetErrorCorrectionlvl(int i)
{ 
    char ErrorCorrectionlvl = 'H';
    if(i <= 23)
        ErrorCorrectionlvl = 'Q';
    if(i <= 15)
        ErrorCorrectionlvl = 'M';
    if(i <= 7)
        ErrorCorrectionlvl = 'L';
    return ErrorCorrectionlvl;
}

static inline
void TransposeMat(char **mat, int size)
{
    char tmp;
    for(int i = 0; i < size; i++)
    {
        for(int j = i; j < size; j++)
        {
            tmp = mat[i][j];
            mat[i][j] = mat[j][i];
            mat[j][i] = tmp;
        }
    }
}

// MAIN FUNCTION

struct PCode *get_code(struct QrCode *qr)
{
    struct PCode *code = malloc(sizeof(struct PCode));
    
    //TransposeMat(qr->mat, qr->version * 4 + 17);
    //print_mat(qr->mat, qr->version * 4 + 17);
    //warn("Starting Mat->BStream");
    char *fmt1 = get_format1(qr->mat);
    int fmtIndex = CorrectFormat(fmt1);
    //warn("fmt1 = %s, Correction : %d", fmt1, fmtIndex);
    if(fmtIndex == -1)
    {
        fmtIndex = CorrectFormatInv(fmt1);
        //warn("INV fmt1 = %s, Correction : %d", fmt1, fmtIndex);
        if(fmtIndex == -1)
        {
            char *fmt2 = get_format2(qr->mat, qr->version);
            fmtIndex = CorrectFormat(fmt2);
            //warn("fmt2 = %s, Correction : %d", fmt2, fmtIndex);
            if(fmtIndex == -1)
            {
                fmtIndex = CorrectFormatInv(fmt2);
                //warn("INV fmt2 = %s, Correction : %d", fmt2, fmtIndex);
                if(fmtIndex == -1)
                    err(EXIT_FAILURE, "Segmentation error : Corrupted Format");
                else
                    TransposeMat(qr->mat, qr->version * 4 + 17);
            }
            free(fmt2);
        }
        else
        {
            TransposeMat(qr->mat, qr->version * 4 + 17);
        } 
    }
    free(fmt1);
    //warn("End Format correction");
    //print_mat(qr->mat, qr->version * 4 + 17);
    char ECL = GetErrorCorrectionlvl(fmtIndex);
    int Mask = fmtIndex % 8;
    size_t size = qr->version * 4 + 17;
    //print_mat(qr->mat, size);
    color_spec_pat(qr->mat, qr->version);
    //print_mat(qr->mat, size);
    demask(qr->mat, size, Mask);
    //print_mat(qr->mat, size); 
    char* msg = ext_cyphmsg(qr->mat, size, qr->version);
    
    code->BStream = msg; 
    code->Mask = Mask;
    code->Version = qr->version;
    code->ECL = ECL;
    
    return code;
}

