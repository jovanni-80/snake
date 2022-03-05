ncsnake: main.o
	g++ main.o -o bin/ncsnake -lncurses

main.o: src/main.cpp src/snake.h
	g++ -c src/main.cpp

clean:
	rm *.o
