all: test_client.exe test_server.exe

test_client.exe: icCommunication.o test_client.cpp
	g++ -g -std=c++17 -Wall -o test_client.exe icCommunication.o test_client.cpp 

test_server.exe: icCommunication.o test_server.cpp
	g++ -g -std=c++17 -Wall -o test_server.exe icCommunication.o test_server.cpp

icCommunication.o: icCommunication.cpp
	g++ -g -std=c++17 -Wall -c -o icCommunication.o icCommunication.cpp

clean:
	rm *.o *.exe
