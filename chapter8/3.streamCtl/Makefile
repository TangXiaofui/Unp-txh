CC=gcc
CFLAGS= -I../../include -g -Wall
LIBS = ../../lib/libunp.a -lpthread

SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

all:ser cli
	ctags -R
	cscope -Rbq

ser:ser.o
	$(CC) $^ $(CFLAGS) -o $@ $(LIBS)

cli:cli.o
	$(CC) $^ $(CFLAGS) -o $@ $(LIBS)
	

$(OBJS):%.o:%.c
	$(CC) -c $< $(CFLAGS) -o $@ 

clean:
	rm -f *.o cscope.* ser cli tags
