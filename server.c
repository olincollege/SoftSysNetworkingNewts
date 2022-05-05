#include "server.h"

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

Writes the user's output to the socket 

Args: 
    my_client: A void pointer to an integer storing the client number

*/
void *chat_write(void *my_client)
{   
    /* Max message size is 100 characters */
    char message[100];

    /* Cast client to int pointer */
    int client;
    client = (intptr_t) my_client;

    /* Wait for user input, and write it to client. */
    while(1){
        fgets(message, sizeof(message), stdin);  
        if (write(client, message, sizeof(message)) < 0) {
            perror("Uh oh");
        }

        /* Close socket if interrupted */
        catch_signal(SIGINT, close_socket);
    }
   pthread_exit(NULL);
}

/* 

Reads the from the socket 

Args: 
    my_client: A void pointer to an integer storing the client number

*/
void *chat_read(void *my_client)
{   
    /* Maximum message buffer size of 100 */
    char buf[100] = { 0 };

    /* Cast client to int pointer */
    int client;
    client = (intptr_t) my_client;

    /* Repeatedly read from socket */
    int bytes_read;
    while(1){
        /* Close socket if interrupted */
        catch_signal(SIGINT, close_socket);

        bytes_read = read(client, buf, sizeof(buf));
        /* If data is detected, print output */
        if( bytes_read > 0 ) {
            printf("Charlie: ");
            printf("%s", buf);
        }
   }
   /* Exit thread */
   pthread_exit(NULL);
}

/* Main function to establish connection with client */
int main(int argc, char **argv)
{   
    /* Initialize threads */
    pthread_t thread_a;
    pthread_t thread_b;
    int rc;
    char i;

    /* Initialize socket */
    struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
    char buf[100] = { 0 };
    int bytes_read;
    socklen_t opt = sizeof(rem_addr);

    /* Set message size to 100 */
    char message[100];

    /* Allocate socket */
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    /* Bind socket to Port 1 of the first available local bluetooth adapter */
    loc_addr.rc_family = AF_BLUETOOTH;
    loc_addr.rc_bdaddr = *BDADDR_ANY;
    loc_addr.rc_channel = (uint8_t) 1;
    bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));

    /* Put socket into listening mode. */
    listen(s, 1);

    /* Accept one connection */
    client = accept(s, (struct sockaddr *)&rem_addr, &opt);
    ba2str( &rem_addr.rc_bdaddr, buf );

    fprintf(stderr, "Connection from %s\n", buf);
    memset(buf, 0, sizeof(buf));

    /* Ask user if they want to connect */
    printf("Do you want to connect?(y/n) ");
    scanf("%c", &i);

    if (i=='y'){
        /* Create threads for reading from and writing to other user */
        rc = pthread_create(&thread_a, NULL, chat_write, (void *) (intptr_t) client);
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
        rc = pthread_create(&thread_b, NULL, chat_read, (void *) (intptr_t) client);
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
        pthread_exit(NULL);
    }
    else {
        /* Inform user that they will not be connected to */
        char message[11]="No connect";
        if (write(client, message, sizeof(message)) < 0) {
            perror("Uh oh");
        }
        printf("Closing\n");
        /* Close connection */
        close(client);
        close(s);
        return 0;
    }
    return 0;
}