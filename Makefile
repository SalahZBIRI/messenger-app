CC = gcc
CFLAGS = -Wall -g
LIBS = -pthread `pkg-config --cflags --libs gtk+-3.0`

all: interface cl serv

interface: interface.c
	$(CC) $(CFLAGS) -o interface interface.c $(LIBS)

cl: cl.c
	$(CC) $(CFLAGS) -o cl cl.c $(LIBS)

serv: serv.c
	$(CC) $(CFLAGS) -o serv serv.c $(LIBS)

clean:
	rm -f interface cl serv

