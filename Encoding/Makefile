# Cypher Makefile

CC= gcc -fsanitize=address
CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -std=c99 -O2
LDFLAGS=
LDLIBS=`pkg-config --libs sdl` -lSDL_image -lm

SRC= main.c Analysis/analysis.c MsgEncoding/encode.c MsgEncoding/encode_message.c Draw/mask.c
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
