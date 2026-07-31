CXX = g++

CXXFLAGS = -std=c++20 -Wall -Wextra -fPIC

INCLUDES = \
-Iinclude \
-I../common/include \
-I../customer/include \
-I../utils/include

SRC = $(wildcard src/*.cpp)

OBJ = $(patsubst src/%.cpp,build/%.o,$(SRC))

TARGET = ../lib/libLogger.so

all: create_dirs $(TARGET)

create_dirs:
	mkdir -p build
	mkdir -p ../lib

$(TARGET): $(OBJ)
	$(CXX) -shared -o $@ $(OBJ)

build/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf build
	rm -f $(TARGET)
print:
	@echo $(INCLUDES)
	@pwd