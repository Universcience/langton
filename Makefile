CFLAGS=-Wall -Wextra -g
LDFLAGS=-lncurses

BIN=langton

turing: main.c langton.c

.PHONY: all clean

all: ${BIN}

clean:
	-rm -f ${BIN}
