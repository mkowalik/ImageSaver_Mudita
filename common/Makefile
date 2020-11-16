all: test_client.exe test_server.exe

test_client.exe: icCommunication.o test_client.cpp
	g++ -g -std=c++17 -o test_client.exe icCommunication.o test_client.cpp

test_server.exe: icCommunication.o test_server.cpp
	g++ -g -std=c++17 -o test_server.exe icCommunication.o test_server.cpp

icCommunication.o: icCommunication.cpp
	g++ -g -std=c++17 -c -o icCommunication.o icCommunication.cpp

clean:
	rm *.o *.exe