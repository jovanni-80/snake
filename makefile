ncsnake: main.o
	g++ main.o -o ncsnake -lncurses

main.o: main.cpp
	g++ -c main.cpp

clean:
	rm *.o
