# Cypher Makefile

CC= gcc -fsanitize=address
CPPFLAGS= `pkg-config --cflags sdl2` -MMD
CFLAGS= -Wall -Wextra -std=c99 -O2 -g
LDFLAGS=
LDLIBS=`pkg-config --libs sdl2` -lSDL_image -lm

SRC= main.c analysis/analysis.c msg_encoding/encode.c msg_encoding/encode_message.c draw/mask.c msg_encoding/weaver.c draw/draw.c draw/fill_mat.c gen_img/gen_img.c ../sdl/mysdl.c error_cor/RS/encode_RS.c error_cor/RS/array.c error_cor/RS/op.c msg_decoding/decode.c msg_decoding/unweaver.c msg_decoding/decode_message.c freect.c
OBJ=${SRC:.c=.o}
DEP=${SRC:.c=.d}

all : test 

test: ${OBJ}
	${CC} ${CFLAGS} ${CPPFLAGS} ${OBJ} ${LDLIBS} -o main

.PHONY: clean

clean:
	${RM} ${OBJ} ${DEP} *~
	${RM} main
	${RM} *.d

-include ${DEP}

# End
