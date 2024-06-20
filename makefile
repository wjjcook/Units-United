CXX := g++
CXXFLAGS := -I src/include
LDFLAGS := -L src/lib
LIBS := -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf

# Source files and object files
SRCS := main.cpp game.cpp text.cpp button.cpp player.cpp unit.cpp caveman.cpp
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
	-del -rf $(OBJS) $(TARGET).exe

# Phony targets
.PHONY: all clean