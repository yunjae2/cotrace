all: program

program: program.c libcotrace.so
	gcc -no-pie -finstrument-functions -Wl,-rpath,. -o program program.c -L. -lcotrace

libcotrace.so: ctxtrace.o objtrace.o
	gcc -shared -fPIC -o libcotrace.so ctxtrace.o objtrace.o -ldl

ctxtrace.o: ctxtrace.c ctxtrace.h
	gcc -c -fPIC -o ctxtrace.o ctxtrace.c

objtrace.o: objtrace.c objtrace.h
	gcc -c -fPIC -o objtrace.o objtrace.c

run: program
	./program

clean:
	rm -f *.o *.so program
