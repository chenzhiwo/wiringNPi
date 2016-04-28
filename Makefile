LIB_NAME=libwiringNPi.so
DEMO_NAME=demo
PERFIX=/usr

CC=gcc
CFLAGS=-O3 -Wall
LIB_CFLAGS=$(CFLAGS) -shared -fPIC
LDFLAGS=
DEMO_FLAGS=-lwiringNPi

.PHONY:clean rebuild exec debug install

all:$(LIB_NAME)

clean:
	@echo "Cleaning workspace.........."
	-rm ./*.o ./$(LIB_NAME) ./$(DEMO_NAME)

rebuild:clean all

exec:all install $(DEMO_NAME)
	./$(DEMO_NAME)

debug:CFLAGS+=-g
debug:rebuil
	gdb ./$(DEMO_NAME)

install:$(LIB_NAME)
	install --mode=0644 wiringNPi.h  $(PERFIX)/include/
	install --mode=0644 libwiringNPi.so $(PERFIX)/lib/

uninstall:
	-rm  $(PERFIX)/include/wiringNPi.h
	-rm  $(PERFIX)/lib/libwiringNPi.so

$(LIB_NAME):wiringNPi.o
	$(CC) $^ -o $@ $(LIB_CFLAGS)

$(DEMO_NAME):demo.o
	$(CC) $^ -o $@  $(DEMO_FLAGS)
	
demo.o: demo.c wiringNPi.h
wiringNPi.o: wiringNPi.c wiringNPi.h
	$(CC) -c $< $(LIB_CFLAGS)
