program: program.c trace.h
	gcc -no-pie -finstrument-functions -o program program.c

run: program
	./program

clean:
	rm -f *.o program
