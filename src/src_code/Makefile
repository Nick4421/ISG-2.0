# Makefile for encode.c and decode.c

CXX=gcc
CXXFLAGS=-Wall -Wextra -Wpedantic -Wshadow

all: encode decode

decode: decode.o
	${CXX} ${CXXFLAGS} decode.o -o decode

encode: encode.o
	${CXX} ${CXXFLAGS} encode.o -o encode

decode.o: decode.c
	${CXX} ${CXXFLAGS} -c decode.c

encode.o: encode.c
	${CXX} ${CXXFLAGS} -c encode.c

clean:
	rm -f *.o encode decode