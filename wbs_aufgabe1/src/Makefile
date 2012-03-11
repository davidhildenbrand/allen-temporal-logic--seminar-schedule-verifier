CC      = /usr/bin/gcc
CFLAGS  = -Wall -g -D_REENTRANT 
LDFLAGS = -Wl,-t 
NAME = wbs_aufgabe1

OBJ = allan_web.o allan.o logger.o wbs_aufgabe1.o 

wbs_aufgabe1: $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(OBJ) $(NAME)
