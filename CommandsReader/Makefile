all: CommandsReader.out

CommandsReader.out: main.cpp ../common/ip_communication.o
	make -C ../common all
	g++ -g -O0 -Wall -std=c++17 ../common/ip_communication.o -o CommandsReader.out main.cpp

clean:
	rm -f *.o *.out
