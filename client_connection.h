#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

int client_connection();
void *Read(void *s);
void *Write(void *s);