scan_server.o: scan_server.c scan_server.h
	gcc scan_server.c -c -lbluetooth

client_connection.o: client_connection.c client_connection.h
	gcc client_connection.c -c -lbluetooth

main: scan_server.o client_connection.o client_connection.h main.c scan_server.h
	gcc main.c scan_server.o client_connection.o -o main -lbluetooth