all:
	cc -std=c99 -march=native -Wall -Wno-psabi -lm -o test -Iinclude src/3dm.c tests/test.c
	./test
