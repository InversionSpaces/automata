CC=g++
CFLAGS=--std=c++17
SRC=main.cpp
HDR=automata.hpp

all:
	$(CC) $(CFLAGS) $(HDR) $(SRC) -o automata

clear:
	rm automata
