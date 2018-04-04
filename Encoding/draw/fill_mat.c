/*
**  Fill Qr Data Matrix 
**  file : fill_mat.c
**  discription : Writing down the data into the matrix 
*/

# include "err.h"

# include "../headers/fill_mat.h"
# include "../headers/mask.h"

// SUB_FUNCTION

static inline
void upwards(char **mat, char *msg, size_t size, int *ip, int *jp, int *kp)
{
    int k = *kp;
    int i = *ip;
    int j = *jp;
    while(((size_t)j < size && (size_t)i < size) && 
            (i >= 0 && j  >= 0) && (mat[i][j] == 0))
    {
        //warn("1 going up %d %d", i, j);
        mat[i][j] = msg[k];
        //mat[i][j] = 'x';
        j -= 1;
        k += 1;
        //warn("2 going up %d %d", i, j); 
        mat[i][j] = msg[k];
        //mat[i][j] = 'x';
        j++;
        i--;
        k++;
        //warn("3 going up %d %d", i, j); 
    }

    *kp = k;
    *ip = i;
    *jp = j;
}

static inline
void downwards(char **mat, char *msg, size_t size, int *ip, int *jp, int *kp)
{
    int k = *kp;
    int i = *ip;
    int j = *jp;
    
    while(((size_t)j < size && (size_t)i < size) && 
            (j >= 0 && i >= 0) && (mat[i][j] == 0))
    {
        //warn("1 going down %d %d", i, j);
        mat[i][j] = msg[k];
        //mat[i][j] = 'o';
        j--;
        k++;
        //warn("2 going down %d %d", i, j); 
        mat[i][j] = msg[k];
        //mat[i][j] = 'o';
        j++;
        i++;
        k++;
        //warn("3 going down %d %d", i, j);
    }
    
    *kp = k;
    *ip = i;
    *jp = j;    
}

static inline
int check_stop_up(char **mat, int *ip, int *jp, int *kp)
{
    int k = *kp;
    int i = *ip;
    int j = *jp;
    int ret = 0;
    
    //out of upper bounds, go down
    if(i < 0) //stop 1
    {
        j -= 2;
        i = 0;
        ret = 1;
    }
    else if(mat[i][j] == 'c') //if we are in c, go down, stop 2
    {
        j -= 2;
        i++;
        if (mat[i][j] == 't')
        {
            j--;
        }
        ret = 1;
    }
    else if(mat[i][j] == 't') //if we are in t
    {
        i--;
        if(mat[i][j] == 'v')//deal with version later
        {
            j -= 3;
            ret = 4;
        }
        else
        {
            //no V, then continue up
            ret = 3;
        }
    }
    else if(mat[i][j] == 'a')
    {
        ret = 6;   
        if(mat[i][j - 1] == 'a')
        {
            i -= 5;
            ret = 5;
        }
    } 
    
    *kp = k;
    *ip = i;
    *jp = j;
    return ret; // Unknown stop
}

static inline
int check_stop_down(char **mat, size_t size, int *ip, int *jp, int *kp)
{
    int k = *kp;
    int i = *ip;
    int j = *jp;
    int ret = 0;

    //out of bottom bounds, go up
    //warn("checking down");
    //warn("i/size : %d/%lu", i, size);
    //warn("%d %d", i , j);
    if ((size_t)i >= size)
    {
        j -= 2;
        i = size - 1;
        if (mat[i][j] == 'c')
        {
            i = size - 9;
        }
        ret = 1;
    }
    else if(mat[i][j] == 't')
    {
        i++;
        ret = 2;
    }
    else if(mat[i][j] == 'a')
    {
        ret = 5;
        if(mat[i][j - 1] == 'a')
        { 
            i += 5;
            ret = 3;
        }
    }
    else if(mat[i][j] == 'v' || mat[i][j] == 's')
    {
        j -= 2;
        i -= 1;
        ret = 4;
        if(j < 0)
            ret = 10;
    }
    
    *kp = k;
    *ip = i;
    *jp = j;
    return ret;
}

// MAIN_FUNCTION

void fill_mat(char **mat, size_t size, int version, char *msg, size_t msg_length)
{
    size_t version_m = 0;
    int nv = 0;
    if(version >= 7)
    {
        nv = 2;
        version_m = 36;
        if(version >= 14)
            nv = 4;
        if(version >= 21)
            nv = 6;
        if(version >= 28)
            nv = 8;
        if(version >= 35)
            nv = 10; 
    }
    int nb_ap = Ap_coord[version - 1][0];
    /*printf("Size = %li\n", size);
    printf("Version = %i\n", version);
    printf("Version_m = %li\n", version_m);
    printf("Number of AP = %i\n", nb_ap);*/
    int totnb_bit = size * size - ( 3*8*8 + nb_ap*5*5 + 2*(size - 8*2)) - 31 -
                    version_m + nv * 5;
    //verify the msg length and the totnb_bit possible
    if((size_t)totnb_bit != msg_length)    
        warn("size error in fill mat | Size to match : %i; Current : %li\n", totnb_bit, msg_length);

    int i = size - 1;
    int j = size - 1;
    int k = 0;
    //warn("1");
    while(1)
    {
        //warn("iter");

        //reading up
        upwards(mat, msg, size, &i, &j, &k);
        
        //Check why up stopped
        int stop = check_stop_up(mat, &i, &j, &k);
        
        if (stop == 1) // out of bound
        {   
            //warn("up 1");
            //warn("%d %d", i, j);
            downwards(mat, msg, size, &i, &j, &k);
            //warn("%d %d", i, j);
        }
        else if (stop == 2) // in a c (maybe c then t)
        {
            //warn("up 2");
            downwards(mat, msg, size, &i, &j, &k);
        }
        else if (stop == 3) // in a t
        {
            //warn("up 3");
            continue;
        }
        else if (stop == 4)//in a t->v
        {
            //warn("up 4");
            for(int h = 0; h < 6; h++)
            {
                mat[i][j] = msg[k];
                //mat[i][j] = 'd';
                i--;
                k++;
            }
            i = 7;
            j++;
            //warn("%d %d", i,j);
            downwards(mat, msg, size, &i, &j, &k);
        }
        else if (stop == 5)//in a aaaaa
        {
            //warn("up 5");
            continue;
        }
        else if (stop == 6)//in 1a or 0a
        {
            //warn("up 6");
            j--;
            for(int h = 0; h < 5; h++)
            {
                if(mat[i][j] == 't')
                {
                    i--;
                    continue;
                }
                mat[i][j] = msg[k];
                //mat[i][j] = 'x';
                i--;
                k++;
            }
            j++;
            continue;
        }
        else
        {
            printf("%i | %i | stop : %i\n", i, j, stop);
            err(EXIT_FAILURE, "error in fill_mat");
        }   
         
        //done reading down
        //warn("2nd half");
    
        stop = check_stop_down(mat, size, &i, &j, &k);
        
        while( stop != 1)
        {
            //stop = check_stop_down(mat, size, &i, &j, &k);
            
            if (stop == 1) // out of bound + ?c 
            {
                //warn("down 1");
                break;
            }
            else if (stop == 2) // in a t
            {
                //warn("down 2");
                downwards(mat, msg, size, &i, &j, &k);
            }
            else if (stop == 3) // in a a
            {   
                //warn("down 3");
                downwards(mat, msg, size, &i, &j, &k);
            }   
            else if (stop == 4) // in a v or s
            {   
                //warn("down 4");
                break;
            }
            else if (stop == 5) //in ba
            {
                //warn("down 5");
                j--;
                for(int h = 0; h < 5; h++)
                {
                    if(mat[i][j] == 't')
                    {
                        i++;
                        continue;
                    }
                    mat[i][j] = msg[k];
                    //warn("%c 1", mat[i][j]);
                    //mat[i][j] = 'x';
                    i++;
                    k++;
                }
                j++;
                //warn("%c 2", mat[i][j]);
                downwards(mat,msg, size, &i, &j, &k);
            }
            else if (stop == 10)
            {
                //warn("end");
                break;
            }
            else
            {
                err(EXIT_FAILURE, "error in fill_mat");
            }
            
            stop = check_stop_down(mat, size, &i, &j, &k);
        }
        //warn("%d, %d end loop", i, j);
        if(stop == 10)
            break;
    }
    //warn("end"); 
    //warn("output length %d / %d : %s", totnb_bit, k, msg);
}
