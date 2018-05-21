/*
**  Header file for QrCode Image Generator
**  file : gen_img.h
**  decription : header file
*/

# ifndef _GEN_IMG_H_
# define _GEN_IMG_H_

# include "../../sdl/mysdl.h"

void Generate_QrCode(char **mat, int version, const char *name, int mod_size,
        char* outPath, int display);

# endif
