CC=gcc
CFLAGS=-c -Wall -DPC
CFILEDIR = ../Motor-driver/

all: particle_filter

particle_filter: particle_filter.o cons_map.o 
	$(CC) -shared -o particle_filter.dll particle_filter.o const_map.o 

particle_filter.o: $(CFILEDIR)particle_filter.c
	$(CC) $(CFLAGS) $(CFILEDIR)particle_filter.c

const_map: cons_map.o
	$(CC) -shared -o const_map.dll const_map.o

cons_map.o: $(CFILEDIR)const_map.c
	$(CC) $(CFLAGS) $(CFILEDIR)const_map.c
	
clean:
	del /s *.o *.dll