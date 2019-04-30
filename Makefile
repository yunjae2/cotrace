all: program trace

program: program.c libtrace.so
	gcc -no-pie -finstrument-functions -Wl,-rpath,. -o program program.c -L. -ltrace

libtrace.so: trace.c trace.h
	gcc -shared -fPIC trace.c -o libtrace.so

run: program
	./program

clean:
	rm -f *.o program
