LIB_NAME=libwiringNPi.so
DEMO_NAME=demo
PERFIX=/usr

CC=gcc
CFLAGS=-O0 -Wall
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
debug:rebuild demo
	gdb ./$(DEMO_NAME)

install:$(LIB_NAME)
	install --mode=0644 wiringNPi.h  $(PERFIX)/include/
	install --mode=0644 wiringNPiSPI.h  $(PERFIX)/include/
	install --mode=0644 libwiringNPi.so $(PERFIX)/lib/

uninstall:
	-rm  $(PERFIX)/include/wiringNPi.h
	-rm  $(PERFIX)/include/wiringNPiSPI.h
	-rm  $(PERFIX)/lib/libwiringNPi.so

$(LIB_NAME):wiringNPi.o wiringNPiSPI.o elinux.o
	$(CC) $^ -o $@ $(LIB_CFLAGS)

$(DEMO_NAME):demo.o wiringNPi.o wiringNPiSPI.o
	$(CC) $^ -o $@  $(DEMO_FLAGS)
	

wiringNPi.o: wiringNPi.c wiringNPi.h elinux.h
	$(CC) -c $< $(LIB_CFLAGS)

wiringNPiSPI.o: wiringNPiSPI.c wiringNPiSPI.h elinux.h
	$(CC) -c $< $(LIB_CFLAGS)

elinux.o: elinux.c elinux.h
	$(CC) -c $< $(LIB_CFLAGS)

demo.o: demo.c wiringNPi.h

