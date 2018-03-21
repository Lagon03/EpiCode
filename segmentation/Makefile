# Simple Makefile

CC=gcc
CPPFLAGS= `pkg-config --cflags sdl2` -MMD
CFLAGS= -Wall -Wextra -std=c99 -O2
LDFLAGS=
LDLIBS= `pkg-config --libs sdl2` -lSDL2_image -lm

SRC= ../sdl/mysdl.c ../preproc/preproc.c fpfind.c extcode.c surfdraw.c demask.c proc_code.c bit_ext.c FPValid.c geotrans.c main.c
OBJ=${SRC:.c=.o}
DEP=${SRC:.c=.d}

all : main 

main: ${OBJ}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} main

-include ${DEP}

# End
