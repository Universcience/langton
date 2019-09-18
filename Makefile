CFLAGS+=-Wall -Wextra
LDFLAGS=-lncurses

BIN=langton

langton: main.c langton.h langton.c

.PHONY: all clean

all: ${BIN}

clean:
	-rm -f ${BIN}
