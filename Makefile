# Makefile

CPPFLAGS = -MMD
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
LDFLAGS =
LDLIBS =

SRC = main.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}


main: ${OBJ}

-include ${DEP}

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} main

# END
