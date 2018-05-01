CC=g++
CXXFLAGS=-g -std=c++11

_all := test
test:test.o haoml.o
all:$(_all)
clean:
	@rm -rf $(_all) *.o
