# I am a comment, and I want to say that the variable CC will be


# Hey!, I am comment number 2. I want to say that CFLAGS will be the
# options I'll pass to the compiler.
CFLAGS= -c -Wall -std=c11 -Wextra -pedantic -Woverlength-strings -g -O0 -D_GNU_SOURCE 
# LIBS muss im lezten Schritt angehängt werden
LIBS = -L/usr/local/lib -lm -ljansson  -lpq -llog4c
INCLUDES = -I/usr/include/postgresql -I/usr/include/pgsql92
DEPS = decode_json.h struct.h pgs_interface.h debug.h 
OBJ = main.o decode_json.o pgs_interface.o debug.o
# the compiler to use.
CC=gcc $(INCLUDES) 
all: pgs_feeder

pgs_feeder: $(OBJ)
	$(CC) $(OBJ) $(LIBS) -o $@ 


%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<




