#include "scan_server.h"

/* Scan the surroundings for bluetooth devices with active bluetooth connections */
char** scan_server()
{
    /* Initialize variables */
    inquiry_info *ii = NULL;
    int max_rsp, num_rsp;
    int dev_id, sock, len, flags;
    int i;
    char addr[19]= {0};
    char name[248] = { 0 };

    /* Choose local Bluetooth adapter */
    dev_id = hci_get_route(NULL);

    /* Open socket to local Bluetooth microcontroller */
    sock = hci_open_dev( dev_id );
    if (dev_id < 0 || sock < 0) {
        perror("opening socket");
        exit(1);
    }

    printf("Scanning for Devices...\n");
    len  = 8;
    max_rsp = 255;

    /* Flush cache of previously detected devices */
    flags = IREQ_CACHE_FLUSH;

    /* Create inquiry info struct to store bluetooth addresses */
    ii = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info));

    /* Perform Bluetooth device discovery */
    num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii, flags);
    if( num_rsp < 0 ) perror("hci_inquiry");
    
    /* Store and display detected bluetooth addresses as a list of user-friendly names */
    char **device_list = (char**)malloc(num_rsp *sizeof(addr));
    for (i = 0; i < num_rsp; i++) {
        ba2str(&(ii+i)->bdaddr, addr);
        memset(name, 0, sizeof(name));
        
        /* Convert bluetooth address name into a user friendly name, [unknown] if no name found. */
        if (hci_read_remote_name(sock, &(ii+i)->bdaddr, sizeof(name), name, 0) < 0)
        strcpy(name, "[unknown]");

        /* Add to device list and print */
        device_list[i]= strdup(addr);
        printf("%d  %s  %s\n",i, device_list[i], name);
    }

    free( ii );
    close( sock );
    return device_list;
}