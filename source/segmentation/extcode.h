/*
**  Header file for QrCode Extraction 
**  file : extcode.h
**  description : indexes function...
*/

# ifndef _EXTCODE_H_
# define _EXTCODE_H_

# include <math.h>
# include "apsamp.h"
//# include "geotrans.h"
# include <stdlib.h>
# include <err.h>

/*
struct QrCode
{
    int  version;
    char **mat;
};*/

struct QrCode *extract_QrCode(struct GeoImg *qrimg);
struct QrCode *extract_EpCode(struct GeoImg *qrimg, struct GeoImg *coimg);

# endif
