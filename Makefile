all: program

program: program.c libcotrace.so
	gcc -no-pie -finstrument-functions -Wl,-rpath,. -o program program.c -L. -lcotrace

libcotrace.so: ctxtrace.o objtrace.o common.o
	gcc -shared -fPIC -o libcotrace.so ctxtrace.o objtrace.o common.o -ldl

ctxtrace.o: ctxtrace.c ctxtrace.h common.h
	gcc -c -fPIC -o ctxtrace.o ctxtrace.c

objtrace.o: objtrace.c objtrace.h common.h
	gcc -c -fPIC -o objtrace.o objtrace.c

common.o: common.c common.h
	gcc -c -fPIC -o common.o common.c

run: program
	./program

clean:
	rm -f *.o *.so program
