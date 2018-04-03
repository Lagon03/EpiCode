# Cypher Makefile

CC= gcc
CPPFLAGS= `pkg-config --cflags sdl2` -MMD
CFLAGS= -Wall -Wextra -std=c99 -O2
LDFLAGS=
LDLIBS=`pkg-config --libs sdl2` -lSDL_image -lm

SRC= decode.c main.c
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
