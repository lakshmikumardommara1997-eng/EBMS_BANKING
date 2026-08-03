CXX = g++

CXXFLAGS = -std=c++20 -Wall -Wextra -g -fPIC

ODPI_HOME = /home/lakshmi/c_plus_plus_projects/odpi
ORACLE_HOME = /home/lakshmi/oracle/instantclient_23_26

INCLUDES = \
-Iinclude \
-I../common/include \
-I../Logger/include \
-I../utils/include \
-I$(ODPI_HOME)/include

LDFLAGS = \
-L$(ODPI_HOME)/lib \
-L$(ORACLE_HOME) \
-Wl,-rpath,$(ODPI_HOME)/lib \
-Wl,-rpath,$(ORACLE_HOME)

LIBS = \
-lodpic \
-lclntsh

SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst src/%.cpp,build/%.o,$(SRC))

TARGET = ../lib/libdb.so

all: create_dirs $(TARGET)

create_dirs:
	mkdir -p build
	mkdir -p ../lib

$(TARGET): $(OBJ)
	$(CXX) -shared -o $@ $(OBJ) $(LDFLAGS) $(LIBS)

build/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf build
	rm -f $(TARGET)