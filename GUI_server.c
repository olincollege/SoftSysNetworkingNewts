#include "GUI_server.h"

int client;
pid_t pid; 

/* Create widgets for chat entry box, send button, and the layout grid. */
GtkWidget *chatEntry, *sendBtn, *grid;

/* Create a text buffer to store chat history */
gchar buffer[10000];

/*
Read from socket and display on GTK application

args:
    my_client: The client as a void pointer to an integer
    showChat: A GTK Widget corresponding to the chat text space
*/
void *chat_read(GtkWidget *showChat)
{   
    /* Creat buffer of 100 bytes */
    char buf[100] = { 0 };
    int bytes_read;

    /* Read socket for messages from client */
    while(1){
        bytes_read = read(client, buf, sizeof(buf));
        if( bytes_read > 0 ) {
            /* Print received message to standard output */
            printf("Charlie: ");
            printf("%s", buf);

            /* Add message to text history buffer */
            strcat(buf, "\n");
            strcat(buffer, buf);

            /* Display text in the text space */
            gtk_label_set_text(GTK_LABEL(showChat), (const gchar *) buffer);
        }
   }
}

/* 
Write to socket and display on GTK application

args:
    wid: pointer to GTKWidget
    data: A general gpointer to the widget which displays texts

*/
void signup_button_clicked(GtkWidget *wid,gpointer data)
 {  
    /* Get the text from the chat box entry */
    const gchar* chatData = gtk_entry_get_text(GTK_ENTRY(chatEntry)); 

    /* Append previous data to current data */
    gchar buf[100];
    strcat(buf, chatData);
    strcat(buf, "\n");
    strcat(buffer, chatData);
    strcat(buffer, "\n");

    /* Write to socket to send to client */
    if (write(client, buf, sizeof(buf)) < 0) {
            perror("Uh oh");
    }
    /* Display text sent to the widget in the chat application */
    gtk_label_set_text(GTK_LABEL(data), (const gchar *) buffer); 
    

    /* Clear chat entry box */
    gtk_entry_set_text(GTK_ENTRY(chatEntry),""); 
 } 

/*
Create a GTK window and carry out app functions

args:
    app: Pointer to the GTK application that signalled this function
    user_data: General gpointer
*/
static void activate (GtkApplication* app, gpointer user_data)
 {  
     /* Create a new window */
    GtkWidget *window;
    window = gtk_application_window_new (app);

    /* Set title and display size */
    gtk_window_set_title (GTK_WINDOW (window), "An (incomplete) bluetooth chat app!");
    gtk_window_set_default_size (GTK_WINDOW (window), 500, 400);


    /* Create space to enter text. */
    chatEntry = gtk_entry_new(); 

    /* Create button for sending text */
    sendBtn = gtk_button_new_with_label("Send");

    /* Create a widget and starting label for chat messages */
    GtkWidget *showChat;
    showChat = gtk_label_new("Welcome to your chat!\n");

    /* Set text alignment to left */
    gtk_label_set_xalign (GTK_LABEL(showChat), 0.0);


    /* Fork process, one to send texts and the other to receive texts */
      pid = fork();
        if (pid == 0){
            /* Send texts in child process */
            g_signal_connect(sendBtn,"clicked",G_CALLBACK(signup_button_clicked),showChat);
        }
        else {
            /* Receive texts */
            chat_read(showChat);
        }

    /* Create a new GTK box */
    GtkWidget *box; box = gtk_box_new(GTK_ORIENTATION_VERTICAL,2);

    /* Arrange all the widgets in the box */
    gtk_box_pack_start(GTK_BOX(box),showChat,FALSE,FALSE,0); 
    gtk_box_pack_end(GTK_BOX(box),sendBtn,FALSE,FALSE,0); 
    gtk_box_pack_end(GTK_BOX(box),chatEntry,FALSE,FALSE,0);


    /* Add box to the window */
    gtk_container_add(GTK_CONTAINER(window),box); 

    /* Display window */
    gtk_widget_show_all (window);
 } 




int main(int argc, char **argv)
{   
    /* Initialize socket */
    struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
    char buf[100] = { 0 };
    int s, bytes_read;
    socklen_t opt = sizeof(rem_addr);

    /* Set message length to 100 */
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
    char i;
    scanf("%c", &i);

    /* If yes, run application */
    if (i=='y'){
        GtkApplication *app;
        int status;
        app = gtk_application_new ("com.hackthedeveloper", G_APPLICATION_FLAGS_NONE);
        g_signal_connect (app, "activate", G_CALLBACK(activate), NULL);
        status = g_application_run(G_APPLICATION(app), argc, argv);
        g_object_unref (app);
    }


    /* If not, send message back to client denying connection */
    else{
        char message[11]="No connect";
        if (write(client, message, sizeof(message)) < 0) {
            perror("Uh oh");
        }
        printf("Closing\n");
    }
    /* Close socket */
    close(client);
    close(s);
    return 0;
}

