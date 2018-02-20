/*
**  Extracts bit from data matrix into a single string
**  file : bit_ext.c
**  discription : Inorder to go through the Reed-Solomon decoding, a binary
**  string containing the cyphered message will be needed. 
*/

# include "bit_ext.h"

// SUB_FUNCTION

void upwards(char **mat, char *msg, size_t size, int *ip, int *jp, int *kp)
{
    int k = *kp;
    int i = *ip;
    int j = *jp;
    while(j < size && i < size && i >= 0 && j  >= 0 && (mat[i][j] == '1' || mat[i][j] == '0'))
    {
        warn("1 going up %d %d", i, j);
        msg[k] = mat[i][j];
        j -= 1;
        k += 1;
        warn("2 going up %d %d", i, j); 
        msg[k] = mat[i][j];
        j++;
        i--;
        k++;
        warn("3 going up %d %d", i, j); 
    }

    *kp = k;
    *ip = i;
    *jp = j;
}

void downwards(char **mat, char *msg, size_t size, int *ip, int *jp, int *kp)
{
    size_t k = *kp;
    int i = *ip;
    int j = *jp;
    
    while(j < size && i < size && j >= 0 && i >= 0 && (mat[i][j] == '1' || mat[i][j] == '0'))
    {
        warn("1 going down %d %d", i, j);
        msg[k] = mat[i][j];
        j--;
        k++;
        warn("2 going down %d %d", i, j); 
        msg[k] = mat[i][j];
        j++;
        i++;
        k++;
        warn("3 going down %d %d", i, j);
    }
    
    *kp = k;
    *ip = i;
    *jp = j;    
}
// MAIN_FUNCTION

char *ext_cyphmsg(char **mat, size_t size, int version)
{
    size_t version_m = 0;
    if(version >= 7)
        version_m = 36;
    int nb_ap = Ap_coord[version - 1][0];
    int totnb_bit = size * size - ( 3*8*8 + nb_ap*5*5 + 2*(size - 8*2)) - 
                       31 - version_m;
    char *msg = calloc(totnb_bit, sizeof(char));
    int i = size - 1;
    int j = size - 1;
    int k = 0;
    warn("1");
    while(1)
    {
        warn("iter");
        upwards(mat, msg, size, &i, &j, &k);    
        warn("2");
        //verticalism
    
        if(mat[i][j] == 't')
        {
            i--;
            if(mat[i][j] == 'v')
            {
                //do version module
            }
            upwards(mat, msg, size, &i, &j, &k);
            i += 1;
            j++; 
        }   
    
        warn("3");
        if(mat[i][j] == 'a')
        {   
            if(mat[i][j - 1] == 'a')
            {
                i -= 5;
                upwards(mat, msg, size, &i, &j, &k);
            }
            else
            {
                j--;
                msg[k] = mat[i][j];
                k++;
                i--;
                while(mat[i][j + 1] == 'a')
                {
                    msg[k] = mat[i][j];
                    i--;
                    k++;
                }
                j++;
                upwards(mat, msg, size, &i, &j, &k);
            }
        }
    
        warn("3");
        //before going down
        i++;
        
        if(0 > j - 2)
            break;
        else
            j -= 2;
        
        if(mat[i][j] == 't')
            j--;
    
        warn("4");
        downwards(mat, msg, size, &i, &j, &k);
        
        if(i < size & j < size && mat[i][j] == 't')
        {
            i++;
            downwards(mat, msg, size, &i, &j, &k);
        }
        i--;
        j--;
            
    }
    warn("end"); 
    warn("output : %s", msg);
    return msg;
}
