CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Iinclude -fPIC

SRC = src/Common.cpp
OBJ = build/Common.o

TARGET = ../lib/libCommon.so

all: create_dirs $(TARGET)

create_dirs:
	mkdir -p build
	mkdir -p ../lib

$(TARGET): $(OBJ)
	$(CXX) -shared -o $@ $(OBJ)

build/Common.o: src/Common.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf build
	rm -f $(TARGET)
