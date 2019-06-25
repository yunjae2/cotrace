# cotrace manual

## Introduction
`cotrace` is a tracer that traces context and object informations of a program. Using the tracer, an user can extract such informations from a program, and examine the results using provided scripts. The cotrace trace objects dynamically allocated only.


## Tutorial
```
$ make run_trace
$ ./scripts/ctxtree.py program ctx.data 
$ ./scripts/objlist.py program obj.data
```

## Building cotrace
`libcotrace.so` is needed for tracing. Run `make` to build the library in the cotrace directory.


## Buildling a program to trace
Add `-no-pie -finstrument-functions` flags to gcc when building a program.
Below is an example for building a program.
```
$ gcc -no-pie -finstrument-functions -o program program.c
```


## Tracing a program
Preload the cotrace library when running a program like below.
```
$ LD_PRELOAD=<cotrace directory>/libcotrace.so <executable>
```
The trace results will be saved to `ctx.data` and `obj.data` in binary format in the current directory.


## Generating human-friendly results
To see the results in a human eye, two scripts are provided in `scripts/` directory.
The trace results and an executable of a program are used as inputs.

#### Generating context tree
```
$ ./scripts/ctxtree.py <executable> <ctx data file>
```
For further use, run the script with `-h` option.

#### Generating object list
```
$ ./scripts/objlist.py <executable> <obj data file>
```
The objects are sorted by size in descending order.


## Author
Yunjae Lee \<lyj7694@gmail.com\>
