# GNU Makefile

CC = gcc
CCFLAGS = -Wall
LDFLAGS = -O3 -pthread 
TARGET = main

%.o: %.c
	$(CC) $(CCFLAGS) -c $<

%: %.o
	$(CC) $(LDFLAGS) $^ -o $@

all: $(TARGET)

# Dependencias

main: matrix.o main.c
matrix.o: matrix.c matrix.h

clean:
	rm -f *.o *~ $(TARGET)
