all:main

main:main.o read_matrix.o functions.o
	g++ -std=c++11 -O3 -Wall -o main main.o read_matrix.o functions.o
main.o:main.cpp read_matrix.h functions.o
	g++ -std=c++11 -O3 -Wall -c main.cpp
read_matrix.o:read_matrix.cpp
	g++ -std=c++11 -O3 -Wall -c read_matrix.cpp
functions.o:functions.cpp
	g++ -std=c++11 -O3 -Wall -c functions.cpp

clean:
	rm *.o main
