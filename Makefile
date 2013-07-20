clang:
	clang -std=c99 -g -march=native -Wall -lm -Iinclude -o test src/*.c tests/*.c
	./test

gcc:
	gcc -std=c99 -g -march=native -Wall -Wno-psabi -lm -Iinclude -o test src/*.c tests/*.c
	./test

test: clang gcc
