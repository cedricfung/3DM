all:
	cc -std=c99 -march=native -Wall -lm -o test 3dm.c test.c
	./test
