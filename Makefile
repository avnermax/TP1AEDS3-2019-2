CC = gcc
CFLAGS = -g -Wall
CLIBS = -lm
OBJS = arg.o funcoes.o
TARGET = tp
all: $(TARGET)

tp: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o tp tp.c $(CLIBS)
arg.o: arg.c
	$(CC) $(CFLAGS) -c arg.c $(CLIBS)
funcoes.o: funcoes.c funcoes.h
	$(CC) $(CFLAGS) -c funcoes.c $(CLIBS)

clean:
	rm -rf $(OBJS) tp
