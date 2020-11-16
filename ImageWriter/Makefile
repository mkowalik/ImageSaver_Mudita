all: ImageWriter.out

ImageWriter.out: commands_factory.o command.o bmpimage.o ../common/ip_communication.o main.cpp 
	make -C ../common all
	g++ -ggdb -O0 -Wall -std=c++17 -o ImageWriter.out ../common/ip_communication.o commands_factory.o command.o bmpimage.o main.cpp

commands_factory.o: commands_factory.cpp commands_factory.h command.o
	g++ -ggdb -O0 -Wall -std=c++17 -c -o commands_factory.o commands_factory.cpp

command.o: command.cpp command.h
	g++ -ggdb -O0 -Wall -std=c++17 -c -o command.o command.cpp

bmpimage.o: bmpimage.cpp bmpimage.h image.h
	g++ -ggdb -O0 -Wall -std=c++17 -c -o bmpimage.o bmpimage.cpp

clean:
	rm -f *.o *.out

