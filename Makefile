PROG_NAME=main

CC=gcc
CFLAGS=-O3 -Wall
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


$(PROG_NAME):main.o
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)


