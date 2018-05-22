CC=g++
CXXFLAGS=-g -std=c++11
VPATH=haoml

_all := test
test:test.o haoml.o
all:$(_all)
doc:
	rm -rf docs/*
	doxygen
clean:
	@rm -rf $(_all) *.o
.PHONY:doc clean
