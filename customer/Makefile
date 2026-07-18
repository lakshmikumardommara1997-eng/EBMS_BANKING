CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -fPIC -Iinclude  -I../common/include 

SRC = src/Customer.cpp
OBJ = build/Customer.o

TARGET = ../lib/libCustomer.so

all: create_dirs $(TARGET)

create_dirs:
	mkdir -p build
	mkdir -p ../lib

$(TARGET): $(OBJ)
	$(CXX) -shared -o $@ $(OBJ)

build/Customer.o: src/Customer.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf build
	rm -f $(TARGET)
