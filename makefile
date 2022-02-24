ncsnake: main.o
	g++ main.o -o ncsnake -lncurses

main.o: main.cpp snake.h
	g++ -c main.cpp

clean:
	rm *.o
