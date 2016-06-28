#
#  Makefile for ARM/Linux user application
#  make all - build dragonboard_test
#  make dragonboard_test - build dragonboard_test
#  make clean - clean

CC = aarch64-linux-gnu-gcc

all: dragonboard_test

dragonboard_test: dragonboard_test.c
	${CC} -o $@ $<

clean:
	rm -f *.o
	rm -f dragonboard_test
