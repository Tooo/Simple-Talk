CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror

all: main.o keyboard.o sender.o receiver.o screen.o list.o listmanager.o
	gcc $(CFLAGS) main.o keyboard.o sender.o receiver.o screen.o list.o listmanager.o -lpthread -o s-talk

main.o: main.c
	gcc $(CFLAGS) -c main.c

keyboard.o: keyboard.c
	gcc $(CFLAGS) -c keyboard.c

sender.o: sender.c
	gcc $(CFLAGS) -c sender.c

receiver.o: receiver.c
	gcc $(CFLAGS) -c receiver.c

screen.o: screen.c
	gcc $(CFLAGS) -c screen.c

list.o: list.c
	gcc $(CFLAGS) -c list.c

listmanager.o: listmanager.c
	gcc $(CFLAGS) -c listmanager.c

clean:
	rm s-talk *.o