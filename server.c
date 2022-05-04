#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <stdlib.h>

#include <pthread.h>
#include <stdio.h>

void *chat_read(void *my_client)
{   
    char message[100];
    int client;
    client = (int)my_client;
    while(1){
        fgets(message, sizeof(message), stdin);  
        if (write(client, message, sizeof(message)) < 0) {
            perror("Uh oh");
        }

    }
   pthread_exit(NULL);
}

void *chat_write(void *my_client)
{
    char buf[1024] = { 0 };
    int client;
    client = (int)my_client;
    int bytes_read;
    while(1){
        bytes_read = read(client, buf, sizeof(buf));
        if( bytes_read > 0 ) {
            printf("Charlie: ");
            printf("%s", buf);
        }
   }
   pthread_exit(NULL);
}


int main(int argc, char **argv)
{
    pthread_t thread_a;
    pthread_t thread_b;
    int rc;
    struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
    char buf[1024] = { 0 };
    int s, client, bytes_read;
    socklen_t opt = sizeof(rem_addr);
    char message[100];
   int status=0;
    // allocate socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // bind socket to port 1 of the first available 
    // local bluetooth adapter
    loc_addr.rc_family = AF_BLUETOOTH;
    loc_addr.rc_bdaddr = *BDADDR_ANY;
    loc_addr.rc_channel = (uint8_t) 1;
    bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));

    // put socket into listening mode
    listen(s, 1);

    // accept one connection
    client = accept(s, (struct sockaddr *)&rem_addr, &opt);

    ba2str( &rem_addr.rc_bdaddr, buf );
    fprintf(stderr, "accepted connection from %s\n", buf);
    memset(buf, 0, sizeof(buf));

      rc = pthread_create(&thread_a, NULL, chat_read, (void *)client);
      if (rc){
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
      }
      rc = pthread_create(&thread_b, NULL, chat_write, (void *)client);
      if (rc){
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
      }
   pthread_exit(NULL);

    // close connection
    close(client);
    close(s);
    return 0;
}