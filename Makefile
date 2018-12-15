CFLAGS=-Wall -std=c11
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

tmcc: $(OBJS)

$(OBJS): tmcc.h

test: tmcc
			./tmcc -test
			./test.sh

clean:
			rm -f tmcc *.o *~ tmp*
			
