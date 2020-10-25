CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror

all: main.o keyboardinput.o screenoutput.o UDPinput.o UDPoutput.o list.o
	gcc $(CFLAGS) main.o keyboardinput.o screenoutput.o UDPinput.o UDPoutput.o list.o -lpthread -o s-talk

main.o: main.c
	gcc $(CFLAGS) -c main.c

keyboardinput.o: keyboardinput.c
	gcc $(CFLAGS) -c keyboardinput.c

screenoutput.o: screenoutput.c
	gcc $(CFLAGS) -c screenoutput.c

UDPinput.o: UDPinput.c
	gcc $(CFLAGS) -c UDPinput.c

UDPoutput.o: UDPoutput.c
	gcc $(CFLAGS) -c UDPoutput.c

list.o: list.c
	gcc $(CFLAGS) -c list.c

clean:
	rm s-talk *.o