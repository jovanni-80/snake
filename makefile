run: build
	./ncsnake

build: main.o snake.o
	g++ main.o snake.o -o ncsnake -lncurses

main.o: src/main.cpp src/snake.h
	g++ -c src/main.cpp

snake.o: src/snake.cpp src/snake.h
	g++ -c src/snake.cpp

clean:
	rm *.o
