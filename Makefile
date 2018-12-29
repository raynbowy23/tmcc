CFLAGS=-Wall -std=c11 -g
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
$(warning MAKE = $(MAKE))
$(warning CC = $(CC))
$(warning CFLAGS = $(CFLAGS))
OLD_SHELL := $(SHELL)
SHELL = $(warning [Making: $@]   [Dependencies: $^]   [Changed: $?])$(OLD_SHELL)

all: tmcc
tmcc: $(OBJS)
		cc -o $@ $(OBJS) $(CFLAGS)

$(OBJS): tmcc.h util.h

test: tmcc
			./tmcc -test
			./test.sh

clean:
			rm -f tmcc *.o *~ tmp*
			
