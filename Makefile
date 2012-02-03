all: genbintrees

genbintrees: genbintrees.cpp
	g++ -Wall -o genbintrees genbintrees.cpp
