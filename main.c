#include "scan_server.h"
#include "client_connection.h"

int main() {
    int i;
    char** my_scan = scan_server();
    printf("scan: %s\n",my_scan[0]);
    printf("Select Device: ");
    scanf("%i",&i);
    printf("Selected: %s", my_scan[i]);
    client_connection(my_scan[i]);
    printf("Connecting\n");

    return 0;
}