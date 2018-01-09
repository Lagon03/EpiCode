# Cypher Makefile

CC= gcc -fsanitize=address

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -std=c99 -O2
LDFLAGS=
LDLIBS=`pkg-config --libs sdl` -lSDL_image -lm

OBJ= test.o
DEP=${SRC:.o=.d}

all : test 

test: ${OBJ}
	${CC} ${CFLAGS} ${CPPFLAGS} ${OBJ} ${LDLIBS} -o test

.PHONY: clean

clean:
	${RM} ${OBJ} ${DEP} *~
	${RM} test
	${RM} *.d

-include ${DEP}

# End
