include ../config.in

SRCS=$(wildcard *.c)
PROGS=$(SRCS:.c=)

all:$(PROGS)
$(PROGS):%:%.c
	$(CC) $^ $(CFLAGS) -o $@ $(LIBS)
	ctags -R
	cscope -Rbq

clean:
	rm -f $(PROGS) cscope.* tags
