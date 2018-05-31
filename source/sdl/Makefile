# Simple Makefile

CC=gcc
CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -std=c99 -O2
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image -lm

SRC= mysdl.c test.c
OBJ=${SRC:.c=.o}
DEP=${SRC:.c=.d}

all : test 

test: ${OBJ}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} test

-include ${DEP}

# End
