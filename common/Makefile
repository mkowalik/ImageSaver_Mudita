all: ip_communication.o 

ip_communication.o: ip_communication.cpp ip_communication.h
	g++ -ggdb -O0 -Wall -std=c++17 -c -o ip_communication.o ip_communication.cpp

clean:
	rm -f *.o *.out
