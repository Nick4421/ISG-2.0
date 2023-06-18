CXX=g++
CXXFLAGS=-Wall -Wextra -Wpedantic -Wshadow

oldmain: oldmain.o ArrayList.o
	${CXX} ${CXXFLAGS} oldmain.o ArrayList.o -o oldmain

oldmain.o: ArrayList.o oldmain.cpp
	${CXX} ${CXXFLAGS} -c oldmain.cpp

ArrayList.o: ArrayList.cpp ArrayList.h
	${CXX} ${CXXFLAGS} -c ArrayList.cpp

clean:
	rm -f *.o oldmain