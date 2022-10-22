CC=g++
CFLAGS=-I.

SRCS := client.cpp requests.cpp helpers.cpp buffer.cpp commands.cpp utils.cpp

client: $(SRCS)
	$(CC) -o client $(SRCS) -Wall -Wextra

run: client
	./client

clean:
	rm -f *.o client
