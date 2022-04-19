#include "scan_server.h"
#include "client_connection.h"
#include "server1.h"

int main() {

    int i;
    char** my_scan = scan_server();
    printf("Select Device: ");
    scanf("%i/n",&i);
    printf("Selected: %s\n", my_scan[i]);
    // child process because return value zero
    if (fork() == 0)
        server1();
  
    // parent process because return value non-zero.
    else
        client_connection(my_scan[i]);

    return 0;
}