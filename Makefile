GUI_server: GUI_server.c GUI_server.h
	gcc `pkg-config --cflags gtk+-3.0` -o GUI_server GUI_server.c `pkg-config --libs gtk+-3.0` -lbluetooth -pthread

server: server.c server.h
	gcc -o server server.c -lbluetooth -pthread

client: client_connection.o client_connection.h client.c scan_server.c scan_server.h
	gcc client_connection.c -c -lbluetooth
	gcc scan_server.c -c -lbluetooth
	gcc client.c client_connection.o scan_server.o -o client -lbluetooth -pthread