CFLAGS=-Wall -std=c11
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

all: tmcc
tmcc: $(OBJS)

$(OBJS): tmcc.h util.h

test: tmcc
			./tmcc -test
			./test.sh

clean:
			rm -f tmcc *.o *~ tmp*
			
