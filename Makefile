all: program trace

program: program.c libctxtrace.so
	gcc -no-pie -finstrument-functions -Wl,-rpath,. -o program program.c -L. -lctxtrace

libctxtrace.so: ctxtrace.c ctxtrace.h
	gcc -shared -fPIC ctxtrace.c -o libctxtrace.so

run: program
	./program

clean:
	rm -f *.o program
