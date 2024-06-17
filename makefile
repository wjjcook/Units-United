CXX := g++
CXXFLAGS := -I src/include
LDFLAGS := -L src/lib
LIBS := -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf

# Source files and object files
SRCS := main.cpp game.cpp text.cpp button.cpp player.cpp
OBJS := $(SRCS:.cpp=.o)

# Target executable
TARGET := main

# Build target
all: $(TARGET)

# Linking
$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

# Compilation
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean

# Example makefile
CXX = g++
CXXFLAGS = -Wall -g
OBJ = main.o player.o
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJ)
    $(CXX) $(CXXFLAGS) -o $@ $^

main.o: main.cpp player.h
    $(CXX) $(CXXFLAGS) -c main.cpp

player.o: player.cpp player.h
    $(CXX) $(CXXFLAGS) -c player.cpp

clean:
    rm -f $(OBJ) $(TARGET)