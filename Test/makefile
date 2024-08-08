CC = g++
SRC = src/
CFLAGS = -Wall -Wextra -std=c++11
LDFLAGS = -lglfw -lGL

test: test.cpp $(SRC)/glad.c
	$(CC) $(CFLAGS) -o test test.cpp $(SRC)/glad.c $(LDFLAGS)

.PHONY: clean
clean:
	rm -f test