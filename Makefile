scan_server.o: scan_server.c scan_server.h
	gcc scan_server.c -c -lbluetooth

client_connection.o: client_connection.c client_connection.h
	gcc client_connection.c -c -lbluetooth

server.o: server.c server.h
	gcc server.c -c -lbluetooth

main: client_connection.o client_connection.h main.c scan_server.c scan_server.h
	gcc main.c client_connection.o scan_server.o -o main -lbluetooth