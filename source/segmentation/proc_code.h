/*
**  Header file fot QrCode code processing
**  file : proc_code.h
**  description : header file
*/

# ifndef _PROC_C_H_
# define _PROC_C_H_

# include <math.h>
# include "demask.h"
# include "extcode.h"
# include "bit_ext.h"
# include "color_mat.h"
# include <string.h>

struct PCode
{
    int Version;    //Version
    int Mask;       //Mask
    char ECL;       //Error Correction Level
    char *BStream;  //Message in bits
};  

struct PCode *get_code(struct QrCode *qr);

# endif
