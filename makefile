all:
	g++ -I src/include -L src/lib -o main main.cpp game.cpp game.hpp text.cpp text.hpp button.cpp button.hpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
