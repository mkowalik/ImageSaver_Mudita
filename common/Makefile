all: icCommunication.o test_client.out test_server.out

test_client.out: icCommunication.o test_client.cpp
	g++ -g -std=c++17 -Wall -o test_client.out icCommunication.o test_client.cpp 

test_server.out: icCommunication.o test_server.cpp
	g++ -g -std=c++17 -Wall -o test_server.out icCommunication.o test_server.cpp

icCommunication.o: icCommunication.cpp icCommunication.h
	g++ -g -std=c++17 -Wall -c -o icCommunication.o icCommunication.cpp

clean:
	rm *.o *.out
