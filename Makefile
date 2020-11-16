all: 
	(cd common; make all)
	(cd ImageWriter; make all)
	(cd CommandsReader; make all)

clean:
	(make -C ./common/ clean)
	(make -C ./CommandsReader/ clean)
	(make -C ./ImageWriter/ clean)


