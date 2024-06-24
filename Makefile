# Simple makefile

CC := g++ -std=c++20 -O2 -fopenmp

#CCW := -Wall -Werror -Wextra

.DEFAULT_GOAL := main.cpp

main.cpp:
	$(CC) $(CCW) -o ./bin/bpt ./src/main.cpp

all: clean test

clean:
	rm -rf ./bin/bpt

run: main.cpp
	./bin/bpt 5

test: main.cpp
	./bin/bpt 1
