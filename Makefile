all:server client poll

client:client.c
	gcc client.c -o client

server:server.c
	gcc server.c -o server

poll:poll.c
	gcc poll.c -o poll

clean:
	rm -f server
	rm -f client
	rm -f poll
