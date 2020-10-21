CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror

all: socket thread thread2

socket: socket.c
	gcc $(CFLAGS) socket.c -lpthread -o socket

thread: thread.c sleep.c
	gcc $(CFLAGS) thread.c sleep.c -lpthread -o thread

thread2: thread2.c sleep.c
	gcc $(CFLAGS) thread2.c sleep.c -lpthread -o thread2
	
clean:
	rm -f socket thread thread2