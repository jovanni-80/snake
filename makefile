snakeGame: main.o
	g++ main.o -o snakeGame -lncurses

main.o: main.cpp
	g++ -c main.cpp

clean:
	rm *.o
