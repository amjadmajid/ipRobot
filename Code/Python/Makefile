CC=gcc
CFLAGS=-c -Wall
CFILEDIR = ../Motor-driver/

all: const_map	

const_map: cons_map.o
	$(CC) -shared -o const_map.dll const_map.o

cons_map.o: $(CFILEDIR)const_map.c
	$(CC) $(CFLAGS) $(CFILEDIR)const_map.c