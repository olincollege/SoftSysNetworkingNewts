#include "client_connection.h"

int s, client;
/* Set up a signal handler.

   sig: signal number
   handler: signal handler function
*/
int catch_signal(int sig, void (*handler) (int)) {
    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    return sigaction(sig, &action, NULL);
}

/* Signal handler: Close the socket.
 */
void close_socket(int sig)
{
    close(client);
    close(s);
    printf("\nClosing connection!\n");
    exit(EXIT_SUCCESS);
}

/*
Read data from socket in a thread

args: 
    s: A void pointer to an integer storing the socket number.
 */
void *Read(void *s)
{
    /* Cast socket to int pointer */
    int sock;
    sock = (intptr_t) s;

    /* Maximum buffer length is 100 characters */
    char buf[100];
    while(1){
        /* Close socket if interrupted */
        catch_signal(SIGINT, close_socket);
        
        /* Read input from the socket */
        if(read(sock, buf, sizeof(buf)) > 0 ){

            /* If the string sent is "No connect" close socket connection */
            if (strcmp(buf, "No connect") == 0){
                printf("Connection denied\n");
                pthread_exit(NULL);
                exit(1);
            }
            /* Print received message. */
            printf("%s", buf);
            }
    }
    pthread_exit(NULL);
}


/*
Write data to socket in a thread

args: 
    s: A void pointer to an integer storing the socket number.
 */
void *Write(void *s)
{
    /* Cast socket to int pointer */
    int sock;
    sock = (intptr_t) s;

    /* Max message size is 100 characters */
    char message[100];

    /* Wait for user input, and write it to client. */
    while(1){
        fgets(message, sizeof(message), stdin);
        if(write(sock, message, sizeof(message)) < 0 ){ 
            perror("uh oh");}

        /* Close socket if interrupted */
        catch_signal(SIGINT, close_socket);
        }
    pthread_exit(NULL);
}

/* Communicates to the server using a socket. */
int client_connection(char* MAC)
{   
    
    /* Initialize threads */
    pthread_t thread_a;
    pthread_t thread_b;
    int rc;

    /* Initialize socket */
    struct sockaddr_rc addr = { 0 };
    int status, bytes_read;
    printf("Trying to Connect\n");

    /* Set message size to 100 */
    char message[100];

    /* Allocate socket */
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    /* set the connection parameters (who to connect to) */
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba( MAC, &addr.rc_bdaddr );

    /* Connect to server */
    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));
    
    /* On successful connection begin communication (pending server approval) */
    if( status == 0 ) {
        printf("Pending server approval\n");
        
        /* Create threads for reading from and writing to other user */
        rc = pthread_create(&thread_a, NULL, Write, (void *) (intptr_t) s);
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
        rc = pthread_create(&thread_b, NULL, Read, (void *) (intptr_t) s );
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
        pthread_exit(NULL);
    }
    if( status < 0 )
        perror("uh oh");
    
    /* Close connection */
    close(s);
    return 0;
}
