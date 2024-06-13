all:
	g++ -I src/include -L src/lib -o main main.cpp game.cpp game.hpp -lmingw32 -lSDL2main -lSDL2 -lSDL2ttf
