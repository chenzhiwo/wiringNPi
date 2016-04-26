PROG_NAME=demo

CC=gcc
CFLAGS=-O0 -Wall
LDFLAGS=-lm

.PHONY:clean rebuild exec debug

all:$(PROG_NAME)

clean:
	@echo "Cleaning workspace.........."
	-rm ./*.o ./$(PROG_NAME)

rebuild:clean all

exec:all
	./$(PROG_NAME)

debug:CFLAGS += -g
debug:rebuild
	gdb ./$(PROG_NAME)

$(PROG_NAME):demo.o wiringNPi.o
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)

demo.o: demo.c wiringNPi.h
wiringNPi.o: wiringNPi.c wiringNPi.h
