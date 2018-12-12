tmcc: tmcc.c

test: tmcc
	./test.sh
clean:
	rm -f tmcc *.o *~ tmp*
