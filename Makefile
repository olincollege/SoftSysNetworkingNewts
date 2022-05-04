scan_server.o: scan_server.c scan_server.h
	gcc scan_server.c -c -lbluetooth

client_connection.o: client_connection.c client_connection.h
	gcc client_connection.c -c -lbluetooth

server: server.c server.h
	gcc `pkg-config --cflags gtk+-3.0` -o server server.c `pkg-config --libs gtk+-3.0` -lbluetooth -pthread

main: client_connection.o client_connection.h main.c scan_server.c scan_server.h
	gcc main.c client_connection.o scan_server.o -o main -lbluetooth -pthread