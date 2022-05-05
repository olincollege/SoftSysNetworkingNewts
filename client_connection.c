#include "client_connection.h"

void *Read(void *s){
    //Read from Socket
    int sock;
    sock = (int)s;
    char buf[1024];
    while(1){
        //Read input from the socket
        if(read(sock, buf, sizeof(buf)) > 0 ){
            //If the string sent is "No connect" close socket connection
            if (strcmp(buf, "No connect") == 0){
                printf("Connection denied\n");
                pthread_exit(NULL);
                exit(1);
            }
            char *b = &buf[2]; //Remove first two characters
            printf("%s",b); //Print read output to standard output terminal
            }
    }
    pthread_exit(NULL);
    
}
void *Write(void *s){
    int sock;
    sock = (int)s;
    char message[100];
    while(1){
        //Get message from standard input from user
        fgets(message, sizeof(message), stdin);
        //Write message to socket output
        if(write(sock, message, sizeof(message)) < 0 ){ 
            perror("uh 1oh");}
        }
        pthread_exit(NULL);
}


int client_connection(char* MAC){
    struct sockaddr_rc addr = { 0 };
    int s, status, bytes_read, client;
    char message[100];

    // allocate a socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // set the connection parameters (who to connect to)
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba( MAC, &addr.rc_bdaddr );

    printf("Trying to Connect\n");
    // connect to server
    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));
    
    // send a message
    if( status == 0 ) {
        printf("Connected\n");
        pthread_t threads[2];
        int rc;
        int rc1;
        //Create thread for writing data to socket
        rc = pthread_create(&threads[1], NULL, Write, (void*)s);
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
        //Create thread for reading data from socket
        rc1 = pthread_create(&threads[0], NULL, Read, (void*)s );
        if (rc1){
            printf("ERROR; return code from pthread_create() is %d\n", rc1);
            exit(-1);
        }
        pthread_exit(NULL);
    }
    if( status < 0 )
        perror("uh oh");

    close(s);
    return 0;
}
