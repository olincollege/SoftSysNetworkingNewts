scan_server.o: scan_server.c scan_server.h
	gcc scan_server.c -c -lbluetooth

main: scan_server.o main.c scan_server.h
	gcc main.c scan_server.o -o main -lbluetooth