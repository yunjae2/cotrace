all: program trace

program: program.c libtrace.so
	gcc -no-pie -finstrument-functions -o program program.c -L. -ltrace

libtrace.so: trace.c trace.h
	gcc -shared -fPIC trace.c -o libtrace.so

run: program
	LD_LIBRARY_PATH=. ./program

clean:
	rm -f *.o program
