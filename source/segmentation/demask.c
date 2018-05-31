/*
**  Demasking QrMatrices
**  file : demask.c
**  description : demask qr code matrices, use file with proc_code.c
*/

# include "demask.h"

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

void demask(char **mat, size_t size, int mask)
{
    if (mask == 0)
        demask0(mat , size);
    else if (mask == 1)
        demask1(mat , size);
    else if (mask == 2)
        demask2(mat , size);
    else if (mask == 3)
        demask3(mat , size);
    else if (mask == 4)
        demask4(mat , size);
    else if (mask == 5)
        demask5(mat , size);
    else if (mask == 6)
        demask6(mat , size);
    else if (mask == 7)
        demask7(mat , size);
    return;
}
