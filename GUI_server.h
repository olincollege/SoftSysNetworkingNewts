#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <stdio.h>


void *chat_read(GtkWidget *showChat);
void signup_button_clicked(GtkWidget *wid,gpointer data);
static void activate (GtkApplication* app, gpointer user_data);
