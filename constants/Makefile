CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Iinclude -fPIC

SRC = src/Constants.cpp
OBJ = build/Constants.o

TARGET = ../lib/libConstants.so

all: create_dirs $(TARGET)

create_dirs:
	mkdir -p build
	mkdir -p ../lib

$(TARGET): $(OBJ)
	$(CXX) -shared -o $@ $(OBJ)

build/Constants.o: src/Constants.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf build
	rm -f $(TARGET)
