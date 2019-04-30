program: program.c
	gcc -no-pie -finstrument-functions -o program program.c

run: program
	./program

clean:
	rm -f *.o program
