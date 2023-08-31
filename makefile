ncsnake: main.o snake.o
	g++ main.o snake.o -o ncsnake -lncurses

run: ncsnake
	./ncsnake

snake.o: src/snake.cpp src/snake.h
	g++ -c src/snake.cpp

main.o: src/main.cpp src/snake.h
	g++ -c src/main.cpp

clean:
	rm *.o
