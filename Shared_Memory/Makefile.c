CC = cc
CFLAGS = -std=c99 -pedantic -Wall
OBJECTSS = filename.o

all: test_makefile

filename.o: filename.c
	$(CC) $(CFLAGS) -c filename.c

appname: $(OBJECTS)
	$(CC) $(OBJECTS) -O test_makefile

clean:
	rm -f *.o test_makefile