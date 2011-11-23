BUILDCHAIN=../meta/buildchain/
include $(BUILDCHAIN)/library-info.mk

LIB=../lib/
INC=../include/
BIN=../bin/

TARGETS=$(shell ls *.cc | sed s/.cc/.o/g)
CXXFLAGS=-Wall -c -I$(INC)
LDFLAGS=-lncurses -lpanel -lutil -lansiescape -lshibuya
OUTPUT=$(BIN)rokkaku

all: $(TARGETS) link

include $(BUILDCHAIN)build.mk
