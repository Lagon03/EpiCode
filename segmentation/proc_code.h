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
# include <string.h>

struct PCode
{
    char *err_cor_lvl;
    char *mask;
    char *err_cor;
    char *code;
};  

struct PCode *get_code(struct QrCode *qr);

# endif
