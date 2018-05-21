# Simple Makefile

CC=gcc
CPPFLAGS= `pkg-config --cflags sdl2` -MMD
CFLAGS= -Wall -Wextra -std=c99 -O2
LDFLAGS=
LDLIBS= `pkg-config --libs sdl2` -lSDL2_image -lm

#SEGMENTATION
IPSRC= ../sdl/mysdl.c ../preproc/preproc.c
SGSRC= fpfind.c FPValid.c geotrans.c aploc.c apsamp.c extcode.c surfdraw.c
BSSRC= proc_code.c color_mat.c demask.c bit_ext.c
MSSRC= segmentation.c

#DECRYPTION

OBJ=${IPSRC:.c=.o} ${SGSRC:.c=.o} ${BSSRC:.c=.o} ${MSSRC:.c=.o}
DEP=${IPSRC:.c=.d} ${SGSRC:.c=.d} ${BSSRC:.c=.d} ${MSSRC:.c=.d}

all : segmentation 

segmentation: ${OBJ}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} segmentation

-include ${DEP}

# End
