tmcc: tmcc.c

test: tmcc
	bash ./test.sh
clean:
	rm -f tmcc *.o *~ tmp*
