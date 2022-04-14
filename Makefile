client: client.c
	gcc - client.c -o client -lbluetooth

server: server.c
	gcc - server.c -o server -lbluetooth