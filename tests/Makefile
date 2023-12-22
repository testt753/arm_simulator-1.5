CC=clang -Wall -g
LDFLAGS=-fsanitize=address
PROGSTEST= testX 

all: $(PROGSTEST)

testX.o: testX.c ../memory.h ../util.h
	$(CC) -c testX.c

testX: testX.o ../memory.o ../util.o
	$(CC) -o testX testX.o ../memory.o ../util.o $(LDFLAGS)
