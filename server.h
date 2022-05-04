#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <signal.h>

void *chat_write(void *my_client);
void *chat_read(void *my_client);

