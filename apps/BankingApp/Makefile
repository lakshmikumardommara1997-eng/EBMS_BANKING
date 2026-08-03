CXX = g++

CXXFLAGS = -std=c++20 -Wall -Wextra -g

INCLUDES = -I include -I../../common/include -I../../customer/include -I../../Logger/include -I../../utils/include -I../../database/include -I/home/lakshmi/c_plus_plus_projects/odpi/include

LIBS = -L../../lib -lCommon -lCustomer -lLogger -lUtils -ldb

TARGET = ../../bin/BankingSystem

SRC = $(wildcard *.cpp) $(wildcard src/*.cpp)

all:
	mkdir -p ../../bin
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRC) $(LIBS) -o $(TARGET)

clean:
	rm -f $(TARGET)
