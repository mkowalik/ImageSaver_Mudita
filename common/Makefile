# Michal Kowalik <kowalik.michal1@gmail.com>
all: ip_communication.o ip_communication_fifo.o 

ip_communication_fifo.o: ip_communication.o ip_communication_fifo.cpp ip_communication_fifo.h
	g++ -ggdb -Wall -std=c++17 -c -o ip_communication_fifo.o ip_communication_fifo.cpp

ip_communication.o: ip_communication.cpp ip_communication.h 
	g++ -ggdb -Wall -std=c++17 -c -o ip_communication.o ip_communication.cpp

clean:
	rm -f *.o *.out
