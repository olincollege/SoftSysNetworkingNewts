#include "scan_server.h"
#include "client_connection.h"

int main() {

    int i;
    char** my_scan = scan_server();
    printf("Select Device: ");
    scanf("%i", &i);
    printf("Selected: %s\n", my_scan[i]);
    client_connection(my_scan[i]);
    
    return 0;
}