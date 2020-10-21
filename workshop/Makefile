CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror

all: socket thread

socket: socket.c
	gcc $(CFLAGS) socket.c -lpthread -o socket

thread: thread.c sleep.c
	gcc $(CFLAGS) thread.c sleep.c -lpthread -o thread
	
clean:
	rm -f socket thread