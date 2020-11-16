all: commands_reader.out log_saver.o

commands_reader.out: main.cpp log_saver.o commands_reader.o ../common/ip_communication.o ../common/ip_communication_fifo.o
	make -C ../common all
	g++ -ggdb -Wall -std=c++17 ../common/ip_communication.o ../common/ip_communication_fifo.o log_saver.o commands_reader.o -o commands_reader.out main.cpp

log_saver.o: log_saver.cpp log_saver.h
	g++ -ggdb -Wall -std=c++17 -c -o log_saver.o log_saver.cpp

commands_reader.o: commands_reader.cpp commands_reader.h
	g++ -ggdb -Wall -std=c++17 -c -o commands_reader.o commands_reader.cpp

clean:
	rm -f *.o *.out
