# change application name here (executable output name)
TARGET=epidec
 
# compiler
CC=gcc #-fsanitize=address
CPPFLAGS= `pkg-config --cflags sdl2` `pkg-config --cflags --libs gtk+-3.0` -MMD -pthread -export-dynamic
CFLAGS= -Wall -Wextra -std=c99 -O2 -g
LDFLAGS= -pthread -export-dynamic
LDLIBS=`pkg-config --libs sdl2` -lSDL2_image -lm

GTKLIB=`pkg-config --cflags --libs gtk+-3.0`

ISRC= decode.c

MSRC= ../Encoding/enc_main.c ../Encoding/freect.c
ASRC= ../Encoding/analysis/analysis.c
MESRC= ../Encoding/msg_encoding/encode.c ../Encoding/msg_encoding/encode_message.c ../Encoding/msg_encoding/weaver.c
DSRC= ../Encoding/draw/draw.c ../Encoding/draw/fill_mat.c ../Encoding/draw/mask.c
GSRC= ../Encoding/gen_img/gen_img.c
RSSRC= ../Encoding/error_cor/RS/array.c ../Encoding/error_cor/RS/op.c ../Encoding/error_cor/RS/encode_RS.c
SSRC= ../sdl/mysdl.c
MDSRC= ../Encoding/msg_decoding/decode.c ../Encoding/msg_decoding/unweaver.c ../Encoding/msg_decoding/decode_message.c

#SEGMENTATION
IPSRC= ../preproc/preproc.c
SGSRC= ../segmentation/fpfind.c ../segmentation/FPValid.c ../segmentation/geotrans.c ../segmentation/apsamp.c ../segmentation/extcode.c ../segmentation/surfdraw.c
BSSRC= ../segmentation/proc_code.c ../segmentation/color_mat.c ../segmentation/demask.c ../segmentation/bit_ext.c
MSSRC= ../segmentation/segmentation.c


OBJ=${ISRC:.c=.o} ${MSRC:.c=.o} ${ASRC:.c=.o} ${MESRC:.c=.o} ${DSRC:.c=.o} ${GSRC:.c=.o} ${RSSRC:.c=.o} ${SSRC:.c=.o} ${MDSRC:.c=.o} ${IPSRC:.c=.o} ${SGSRC:.c=.o} ${BSSRC:.c=.o} ${MSSRC:.c=.o}
DEP=${ISRC:.c=.d} ${MSRC:.c=.d} ${ASRC:.c=.d} ${MESRC:.c=.d} ${DSRC:.c=.d} ${GSRC:.c=.d} ${RSSRC:.c=.d} ${SSRC:.c=.d} ${MDSRC:.c=.d} ${IPSRC:.c=.d} ${SGSRC:.c=.d} ${BSSRC:.c=.d} ${MSSRC:.c=.d}

IOBJ= ${ISRC:.c=.o}
all : epi 

epi: ${OBJ}
	${CC} ${CFLAGS} ${CPPFLAGS} ${OBJ} ${LDLIBS} ${GTKLIB} -o ${TARGET}

interface: main.c
	${CC} -c ${CCFLAGS} main.c ${GTKLIB} -o main.o

.PHONY: clean

clean:
	${RM} ${OBJ} ${DEP} *-
	${RM} epicode
	${RM} *.d

-include ${DEP}
