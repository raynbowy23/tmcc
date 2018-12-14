tmcc: tmcc.c

test: tmcc
        ./test.sh

clean:
        rm -f 9cc *.o *~ tmp*
