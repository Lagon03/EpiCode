/*
** Signs matrix with the patterns
*/

# include "color_mat.h"

static inline
void color_format1(char **mat)
{
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
}

static inline
void color_format2(char **mat, int size)
{
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
            if((mat[ap[i]][ap[j]] != '0' && mat[ap[i]][ap[j]] != '1' 
                && mat[ap[i]][ap[j]] != 't') || 
                ap[i] == 0 || ap[j] == 0)
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
void color_version1(char **mat, int size)
{
    mat[size - 9][0] = 'v';
    mat[size - 9][1] = 'v';
    mat[size - 9][2] = 'v';
    mat[size - 9][3] = 'v';
    mat[size - 9][4] = 'v';
    mat[size - 9][5] = 'v';
    mat[size - 10][0] = 'v';
    mat[size - 10][1] = 'v';
    mat[size - 10][2] = 'v';
    mat[size - 10][3] = 'v';
    mat[size - 10][4] = 'v';
    mat[size - 10][5] = 'v';
    mat[size - 11][0] = 'v';
    mat[size - 11][1] = 'v';
    mat[size - 11][2] = 'v';
    mat[size - 11][3] = 'v';
    mat[size - 11][4] = 'v';
    mat[size - 11][5] = 'v';
}

static inline
void color_version2(char **mat, int size)
{
    mat[0][size - 9] = 'v';
    mat[1][size - 9] = 'v';
    mat[2][size - 9] = 'v';
    mat[3][size - 9] = 'v';
    mat[4][size - 9] = 'v';
    mat[5][size - 9] = 'v';
    mat[0][size - 10] = 'v';
    mat[1][size - 10] = 'v';
    mat[2][size - 10] = 'v';
    mat[3][size - 10] = 'v';
    mat[4][size - 10] = 'v';
    mat[5][size - 10] = 'v';
    mat[0][size - 11] = 'v';
    mat[1][size - 11] = 'v';
    mat[2][size - 11] = 'v';
    mat[3][size - 11] = 'v';
    mat[4][size - 11] = 'v';
    mat[5][size - 11] = 'v';
}

void color_spec_pat(char **mat, int version)
{
    int size = 4 * version + 17;
    mat[4 * version + 9][8] = 'd';
    color_format1(mat);
    color_format2(mat, size);
    if(version >= 7)
    {
        color_version1(mat, size);
        color_version2(mat, size);
    }
    color_finders(mat, size);
    color_separators(mat, size);
    color_timing_pat(mat, size);
    color_alignment(mat, version);
}
