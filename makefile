CXX := g++
CXXFLAGS := -g -Wall -I src/include
LDFLAGS := -L src/lib
LIBS := -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_NET

# Source files and object files
SRCS := $(wildcard *.cpp units/*.cpp messages/*.cpp)
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
	-del /f /q *.o
	-del /f /q units\*.o
	-del /f /q messages\*.o
	-del /f /q $(TARGET).exe

# Phony targets
.PHONY: all clean