CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Iinclude -fPIC -g

SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst src/%.cpp,build/%.o,$(SRC))

TARGET = ../lib/libConstants.so

all: create_dirs $(TARGET)

create_dirs:
	mkdir -p build
	mkdir -p ../lib

$(TARGET): $(OBJ)
	$(CXX) -shared -o $@ $(OBJ)

build/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS)  -c $< -o $@

clean:
	rm -rf build
	rm -f $(TARGET)
