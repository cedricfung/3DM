all:
	cc -std=c99 -g -march=native -Wall -Wno-psabi -lm -Iinclude -o test src/*.c tests/*.c
	./test
