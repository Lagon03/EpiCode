/*
** Header file for apsamp.c
*/

# ifndef _AP_SAMP_H_
# define _AP_SAMP_H_

# include <stdlib.h>
# include "geotrans.h"
# include "surfdraw.h"
# include "constfile.h"

struct QrCode
{
    int version;
    char **mat;
};

void SampleCodeV2_6(struct GeoImg *qrimg, struct QrCode *qr, int WA, int WB, int WC, int HA, int HB, int HC);
void SampleCodeV7_40(struct GeoImg *qrimg, struct QrCode *qr, int WA, int WB, int WC, int HA, int HB, int HC);
void SampleCodeV2_6E(struct GeoImg *qrimg, struct QrCode *qr, int WA, int WB,
int WC, int HA, int HB, int HC, SDL_Surface *co);
void SampleCodeV7_40E(struct GeoImg *qrimg, struct QrCode *qr, int WA, int WB,
int WC, int HA, int HB, int HC, SDL_Surface *co);
# endif
