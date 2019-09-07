CFLAGS=-Wall -Wextra -g
LDFLAGS=-lncurses

BIN=langton

langton: main.c langton.c

.PHONY: all clean

all: ${BIN}

clean:
	-rm -f ${BIN}
