.PHONY: all
.PHONY: clean
ARGS= -std=c++11 -Wall -O3

all: fourconnect
fourconnect: main.o GameState.o Globals.o MoveCache.o
	g++ -o $@ $(ARGS) $^

test: main.o GameState.o Globals.o MoveCache.o
	g++ -o $@ $(ARGS) $^
	
main.o: Main.cpp
	g++ -c -o $@ $(ARGS) $<

GameState.o: GameState.cpp
	g++ -c -o $@ $(ARGS) $<

Globals.o: Globals.cpp
	g++ -c -o $@ $(ARGS) $<
	
MoveCache.o: MoveCache.cpp
	g++ -c -o $@ $(ARGS) $<
	
Test.o: Test.cpp
	g++ -c -o $@ $(ARGS) $<
	
clean:
	rm -f GameState.o main.o Globals.o MoveCache.o Test.o fourconnect test
