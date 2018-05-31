# executable output name
TARGET1=epidec
TARGET2=epicod

# compiler
CC=gcc #-fsanitize=address
CPPFLAGS= `pkg-config --cflags sdl2` `pkg-config --cflags --libs gtk+-3.0` -MMD -pthread -export-dynamic
CFLAGS= -Wall -Wextra -std=c99 -O2 -g
LDFLAGS= -pthread -export-dynamic
LDLIBS=`pkg-config --libs sdl2` -lSDL2_image -lm

GTKLIB=`pkg-config --cflags --libs gtk+-3.0`

DISRC= source/Interface/decode.c
:x
MISRC= source/Interface/multi.c

MSRC= source/Encoding/enc_main.c source/Encoding/freect.c
ASRC= source/Encoding/analysis/analysis.c
MESRC= source/Encoding/msg_encoding/encode.c source/Encoding/msg_encoding/encode_message.c source/Encoding/msg_encoding/weaver.c
DSRC= source/Encoding/draw/draw.c source/Encoding/draw/fill_mat.c source/Encoding/draw/mask.c
GSRC= source/Encoding/gen_img/gen_img.c
RSSRC= source/Encoding/error_cor/RS/array.c source/Encoding/error_cor/RS/op.c source/Encoding/error_cor/RS/encode_RS.c source/Encoding/error_cor/RS/decode_RS.c
SSRC= source/sdl/mysdl.c
MDSRC= source/Encoding/msg_decoding/decode.c source/Encoding/msg_decoding/unweaver.c source/Encoding/msg_decoding/decode_message.c

#SEGMENTATION
IPSRC= source/preproc/preproc.c
SGSRC= source/segmentation/fpfind.c source/segmentation/FPValid.c source/segmentation/geotrans.c source/segmentation/aploc.c source/segmentation/apsamp.c source/segmentation/extcode.c source/segmentation/surfdraw.c
BSSRC= source/segmentation/proc_code.c source/segmentation/color_mat.c source/segmentation/demask.c source/segmentation/bit_ext.c
MSSRC= source/segmentation/segmentation.c


DOBJ=${DISRC:.c=.o} ${MSRC:.c=.o} ${ASRC:.c=.o} ${MESRC:.c=.o} ${DSRC:.c=.o} ${GSRC:.c=.o} ${RSSRC:.c=.o} ${SSRC:.c=.o} ${MDSRC:.c=.o} ${IPSRC:.c=.o} ${SGSRC:.c=.o} ${BSSRC:.c=.o} ${MSSRC:.c=.o}
DDEP=${DISRC:.c=.d} ${MSRC:.c=.d} ${ASRC:.c=.d} ${MESRC:.c=.d} ${DSRC:.c=.d} ${GSRC:.c=.d} ${RSSRC:.c=.d} ${SSRC:.c=.d} ${MDSRC:.c=.d} ${IPSRC:.c=.d} ${SGSRC:.c=.d} ${BSSRC:.c=.d} ${MSSRC:.c=.d}

EOBJ=${EISRC:.c=.o} ${MSRC:.c=.o} ${ASRC:.c=.o} ${MESRC:.c=.o} ${DSRC:.c=.o} ${GSRC:.c=.o} ${RSSRC:.c=.o} ${SSRC:.c=.o} ${MDSRC:.c=.o}
EDEP=${EISRC:.c=.d} ${MSRC:.c=.d} ${ASRC:.c=.d} ${MESRC:.c=.d} ${DSRC:.c=.d} ${GSRC:.c=.d} ${RSSRC:.c=.d} ${SSRC:.c=.d} ${MDSRC:.c=.d}

MEOBJ= ${MISRC:.c=.o} ${MSRC:.c=.o} ${ASRC:.c=.o} ${MESRC:.c=.o} ${DSRC:.c=.o} ${GSRC:.c=.o} ${RSSRC:.c=.o} ${SSRC:.c=.o} ${MDSRC:.c=.o} ${IPSRC:.c=.o} ${SGSRC:.c=.o} ${BSSRC:.c=.o} ${MSSRC:.c=.o}

all : Epicode 

epicod: ${EOBJ}
	${CC} ${CFLAGS} ${CPPFLAGS} ${EOBJ} ${LDLIBS} ${GTKLIB} -o ${TARGET2}
epidec: ${DOBJ}
	${CC} ${CFLAGS} ${CPPFLAGS} ${DOBJ} ${LDLIBS} ${GTKLIB} -o ${TARGET1}
Epicode: ${MEOBJ}
	${CC} ${CFLAGS} ${CPPFLAGS} ${MEOBJ} ${LDLIBS} ${GTKLIB} -o epicode


.PHONY: clean

clean:
	${RM} ${DOBJ} ${DDEP} *-
	${RM} ${EOBJ} ${EDEP} *-
	${RM} ${TARGET1} ${TARGET2} epicode
	${RM} *.d epicode.o

-include ${DDEP} $ ${EDEP}
