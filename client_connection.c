

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

int client_connection(char* MAC)
{
    struct sockaddr_rc addr = { 0 };
    int s, status, bytes_read, client;
    char buf[1024] = { 0 };
    char message[100];
    //char dest[18] = MAC; //CHANGE THIS TO YOUR BLUETOOTH MAC ADDRESS
    // allocate a socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // set the connection parameters (who to connect to)
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba( MAC, &addr.rc_bdaddr );
    printf("Trying to Connect\n");
    // connect to server
    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));
    printf("Connected\n");
    // send a message
    if( status == 0 ) {
        while(1){
            fgets(message, sizeof(message), stdin);
           
            status = write(s, message, sizeof(message));
                printf("Sent\n");
            bytes_read = read(s, buf, sizeof(buf));
            if( bytes_read > 0 ){
            printf("Vedaant %s\n", buf);
        }
    }
    }
    if( status < 0 ){ 
    perror("uh oh");
    }
    close(s);

    return 0;
}
