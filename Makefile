#CTXFLAGS:= -DCTX_MIN_RUNTIME=1000	# In nanoseconds


all: program lib

lib: libcotrace.so libmhint.so

program_hint: program.c libmhint.so
	gcc -o program program.c

program: program.c libcotrace.so
	gcc -no-pie -finstrument-functions -o program program.c

libmhint.so: mhint.o
	gcc -shared -fPIC -o libmhint.so mhint.o -ldl

libcotrace.so: ctxtrace.o objtrace.o common.o
	gcc -shared -fPIC -o libcotrace.so ctxtrace.o objtrace.o common.o -ldl

mhint.o: mhint.c mhint.h
	gcc -c -fPIC -o mhint.o mhint.c

ctxtrace.o: ctxtrace.c ctxtrace.h common.h
	gcc $(CTXFLAGS) -c -fPIC -o ctxtrace.o ctxtrace.c

objtrace.o: objtrace.c objtrace.h common.h
	gcc -c -fPIC -o objtrace.o objtrace.c

common.o: common.c common.h
	gcc -c -fPIC -o common.o common.c

run: program
	./program

run_trace: program libcotrace.so
	LD_PRELOAD=./libcotrace.so ./program

run_hint: program libmhint.so
	LD_PRELOAD=./libmhint.so ./program

clean:
	rm -f *.o *.so *.data program
