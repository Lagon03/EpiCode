# Cypher Makefile

CC= gcc #-fsanitize=address
CPPFLAGS= `pkg-config --cflags sdl2` -MMD
CFLAGS= -Wall -Wextra -std=c99 -O2 -g
LDFLAGS=
LDLIBS=`pkg-config --libs sdl2` -lSDL2_image -lm

EMSRC= e_main.c freect.c
DMSRC= d_main.c freect.c

# Encoding and Decoding code
ASRC= analysis/analysis.c
MESRC= msg_encoding/encode.c msg_encoding/encode_message.c msg_encoding/weaver.c
DSRC= draw/draw.c draw/fill_mat.c draw/mask.c
GSRC= gen_img/gen_img.c
RSSRC= error_cor/RS/array.c error_cor/RS/op.c error_cor/RS/encode_RS.c error_cor/RS/decode_RS.c
SSRC= ../sdl/mysdl.c
MDSRC= msg_decoding/decode.c msg_decoding/unweaver.c msg_decoding/decode_message.c

# Segmentation code
IPSRC= ../preproc/preproc.c
SGSRC= ../segmentation/fpfind.c ../segmentation/FPValid.c ../segmentation/geotrans.c ../segmentation/aploc.c ../segmentation/apsamp.c ../segmentation/extcode.c ../segmentation/surfdraw.c
BSSRC= ../segmentation/proc_code.c ../segmentation/color_mat.c ../segmentation/demask.c ../segmentation/bit_ext.c
MSSRC= ../segmentation/segmentation.c

# Encoding objects
OBJ=${EMSRC:.c=.o} ${ASRC:.c=.o} ${MESRC:.c=.o} ${DSRC:.c=.o} ${GSRC:.c=.o} ${RSSRC:.c=.o} ${SSRC:.c=.o} ${MDSRC:.c=.o}
DEP=${EMSRC:.c=.d} ${ASRC:.c=.d} ${MESRC:.c=.d} ${DSRC:.c=.d} ${GSRC:.c=.d} ${RSSRC:.c=.d} ${SSRC:.c=.d} ${MDSRC:.c=.d}

# Decoding objects
DOBJ=${DMSRC:.c=.o} ${MSRC:.c=.o} ${ASRC:.c=.o} ${MESRC:.c=.o} ${DSRC:.c=.o} ${GSRC:.c=.o} ${RSSRC:.c=.o} ${SSRC:.c=.o} ${MDSRC:.c=.o} ${IPSRC:.c=.o} ${SGSRC:.c=.o} ${BSSRC:.c=.o} ${MSSRC:.c=.o}
DDEP=${DMSRC:.c=.d} ${MSRC:.c=.d} ${ASRC:.c=.d} ${MESRC:.c=.d} ${DSRC:.c=.d} ${GSRC:.c=.d} ${RSSRC:.c=.d} ${SSRC:.c=.d} ${MDSRC:.c=.d} ${IPSRC:.c=.d} ${SGSRC:.c=.d} ${BSSRC:.c=.d} ${MSSRC:.c=.d}


all : encode decode

encode: ${OBJ}
	${CC} ${CFLAGS} ${CPPFLAGS} ${OBJ} ${LDLIBS} -o encode

decode: ${DOBJ}
	${CC} ${CFLAGS} ${CPPFLAGS} ${DOBJ} ${LDLIBS} -o decode


.PHONY: clean

clean:
	${RM} ${OBJ} ${DEP} *~	
	${RM} ${DOBJ} ${DDEP} *-
	${RM} encode decode
	${RM} *.d

-include ${DEP} ${DDEP}

# End
