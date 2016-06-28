#
#  Makefile for ARM/Linux user application
#  make all - build dragonboard_led
#  make dragonboard_led - build dragonboard_led
#  make clean - clean

CC = aarch64-linux-gnu-gcc

CFLAGS = -Wall 

all: dragonboard_led

dragonboard_led: led.c
	${CC} -o $@ $< $(CFLAGS)

dragonboard_led_sys: led_sys.c
	${CC} -o $@ $< $(CFLAGS)

clean:
	rm -f *.o
	rm -f dragonboard_led
