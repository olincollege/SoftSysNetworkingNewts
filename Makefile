scan_server.o: scan_server.c scan_server.h
	gcc scan_server.c -c -lbluetooth

client_connection.o: client_connection.c client_connection.h
	gcc client_connection.c -c -lbluetooth

server1.o: server1.c server1.h
	gcc server1.c -c -lbluetooth

main: server1.o scan_server.o client_connection.o client_connection.h main.c scan_server.h
	gcc main.c server1.o scan_server.o client_connection.o -o main -lbluetooth