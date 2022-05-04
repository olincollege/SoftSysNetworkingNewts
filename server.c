#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include <pthread.h>
#include <stdio.h>


int client;
pid_t pid; 

// Create widget for email entry.


GtkWidget *emailLabel, *emailEntry, *signupBtn, *grid;

gchar buffer[10000];
void *chat_write(void *my_client, GtkWidget *showEmail )
{   
    char buf[1024] = { 0 };
    int bytes_read;
    while(1){
        bytes_read = read(client, buf, sizeof(buf));
        if( bytes_read > 0 ) {
            printf("Charlie: ");
            printf("%s", buf);

            // Append previous data to current data
            // strcpy(buffer, "hello");
            strcat(buf, "\n");
            strcat(buffer, buf);

            // Print out the text sent to data to the label emailData
            printf("here1");
            gtk_label_set_text(GTK_LABEL(showEmail), (const gchar *) buffer);
            printf(buffer);
            printf("here2");
        }
   }
}
void signup_button_clicked(GtkWidget *wid,gpointer data)
 {  
    // Get the text from the email entry 
    gchar* emailData = gtk_entry_get_text(GTK_ENTRY(emailEntry)); 

    // Append previous data to current data
    // strcpy(buffer, "hello");
    gchar buffer2[100];
    strcat(buffer2, emailData);
    strcat(buffer2, "\n");
    strcat(buffer, emailData);
    strcat(buffer, "\n");
    g_printf("%s\n", buffer);

    if (write(client, buffer2, sizeof(buffer2)) < 0) {
            perror("Uh oh");
    }
    // Print out the text sent to data to the label emailData
    gtk_label_set_text(GTK_LABEL(data), (const gchar *) buffer); 
    

    // Set the email entry box to blank
    gtk_entry_set_text(GTK_ENTRY(emailEntry),""); 
 } 

static void activate (GtkApplication* app, gpointer user_data)
 {  
     // Create a new window
    GtkWidget *window;
    GtkWidget *showEmail;
    window = gtk_application_window_new (app);

    // Set title and size
    gtk_window_set_title (GTK_WINDOW (window), "User Input");
    gtk_window_set_default_size (GTK_WINDOW (window), 500, 400);


    // Create an entry for email.
    emailEntry = gtk_entry_new(); 

    // Create button called Send
    signupBtn = gtk_button_new_with_label("Send");

    // Create a starting label
    showEmail = gtk_label_new("Welcome to your chat!\n");

    // Set alignment to left (1.0 for right)
    gtk_label_set_xalign (showEmail, 0.0);


    // Send the value when signupBtn is clicked to call signup_button_clicked
      pid = fork();
        if (pid == 0){
            g_signal_connect(signupBtn,"clicked",G_CALLBACK(signup_button_clicked),showEmail);
        }
        else {
            chat_write(client,showEmail);
        }

    // Create a new GTK box
    GtkWidget *box; box = gtk_box_new(GTK_ORIENTATION_VERTICAL,2);

    // Place all the values in the box one after the other.
    gtk_box_pack_start(GTK_BOX(box),showEmail,FALSE,FALSE,0); 
    gtk_box_pack_end(GTK_BOX(box),signupBtn,FALSE,FALSE,0); 
    gtk_box_pack_end(GTK_BOX(box),emailEntry,FALSE,FALSE,0);


    // Add box to the window
    gtk_container_add(GTK_CONTAINER(window),box); 

    // Display window
    gtk_widget_show_all (window);
 } 




int main(int argc, char **argv)
{   

    // pthread_t thread_a;
    // pthread_t thread_b;
    // int rc;
    struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
    char buf[1024] = { 0 };
    int s, bytes_read;
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

    char y='y';
    // accept one connection
    client = accept(s, (struct sockaddr *)&rem_addr, &opt);
    ba2str( &rem_addr.rc_bdaddr, buf );
    fprintf(stderr, "Connection from %s\n", buf);
    memset(buf, 0, sizeof(buf));
    printf("Do you want to connect?(y/n) ");
    char i;
    scanf("%c", &i);
    if (i==y){
      
            GtkApplication *app;
            int status;
            app = gtk_application_new ("com.hackthedeveloper", G_APPLICATION_FLAGS_NONE);
            g_signal_connect (app, "activate", G_CALLBACK(activate), NULL);
            status = g_application_run(G_APPLICATION(app), argc, argv);
            g_object_unref (app);
    }
    else{ 
        char message[11]="No connect";
        if (write(client, message, sizeof(message)) < 0) {
            perror("Uh oh");
        }
        printf("Closing\n");
    }
    close(client);
    close(s);
    return 0;
}

