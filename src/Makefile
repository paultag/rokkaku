BUILDCHAIN=../meta/buildchain/

LIB=../lib/
INC=../include/
BIN=../bin/

TARGETS=$(shell ls *.cc | sed s/.cc/.o/g)
CXXFLAGS=-Wall -c -I$(INC)
LDFLAGS=-lncurses -lpanel -lutil -lansiescape -lshibuya -lmenu
OUTPUT=$(BIN)rokkaku

all: $(TARGETS) link
clean:
	rm -f *.o

include $(BUILDCHAIN)build.mk
