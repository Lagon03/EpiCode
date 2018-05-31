# ifndef _FREECT_H_
# define _FREECT_H_

# include <stdlib.h>
# include <stdio.h>
# include <err.h>

void Completefree(struct EncData* data, struct options* args, 
        struct QrCode_Enc* QrCode, struct Weave* weave);
void freeCodewords(struct Codewords* codewords);
void freeED(struct EncData* data);
void freeQrCode(struct QrCode_Enc* QrCode);
void freeWeave(struct Weave* weave);
# endif
