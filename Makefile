# I am a comment, and I want to say that the variable CC will be
# the compiler to use.
CC=gcc
# Hey!, I am comment number 2. I want to say that CFLAGS will be the
# options I'll pass to the compiler.
CFLAGS= -c -Wall -std=c11 -Wextra -pedantic -Woverlength-strings -g -O0  -D_POSIX_C_SOURCE=200112L  -D_GNU_SOURCE 
# LIBS muss im lezten Schritt angehängt werden
LIBS = -L/usr/local/lib -lm -ljansson 

DEPS = decode_json.h struct.h 
OBJ = main.o decode_json.o 

all: pgs_feeder

pgs_feeder: $(OBJ)
	$(CC) $(OBJ) $(LIBS) -o $@ 


%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<




