CC      = /usr/bin/gcc
CFLAGS  = -Wall -g -D_REENTRANT 
LDFLAGS = -Wl,-t 
NAME = main

OBJ = allen_web.o allen.o logger.o main.o 

wbs_aufgabe1: $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(OBJ) $(NAME)
