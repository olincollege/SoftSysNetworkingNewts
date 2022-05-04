#include<gtk/gtk.h>


GtkWidget *emailLabel, *emailEntry, *signupBtn, *grid;

gchar buffer[10000];

void signup_button_clicked(GtkWidget *wid,gpointer data)
 {  
    // Get the text from the email entry 
    const gchar *emailData = gtk_entry_get_text(GTK_ENTRY(emailEntry)); 

    // Append previous data to current data
    // strcpy(buffer, "hello");
    strcat(buffer, emailData);
    strcat(buffer, "\n");
    g_printf("%s\n", buffer);

    // Print out the text sent to data to the label emailData
    gtk_label_set_text(GTK_LABEL(data), (const gchar *) buffer); 

    // Set the email entry box to blank
    gtk_entry_set_text(GTK_ENTRY(emailEntry),""); 
 } 

static void activate (GtkApplication* app, gpointer user_data)
 {  
     // Create a new window
    GtkWidget *window;
    window = gtk_application_window_new (app);

    // Set title and size
    gtk_window_set_title (GTK_WINDOW (window), "User Input");
    gtk_window_set_default_size (GTK_WINDOW (window), 500, 400);

    // Create widget for email entry.
    GtkWidget *showEmail; 

    // Create an entry for email.
    emailEntry = gtk_entry_new(); 

    // Create button called Send
    signupBtn = gtk_button_new_with_label("Send");

    // Create a starting label
    showEmail = gtk_label_new("Welcome to your chat!\n");

    // Set alignment to left (1.0 for right)
    gtk_label_set_xalign (showEmail, 0.0);


    // Send the value when signupBtn is clicked to call signup_button_clicked
    g_signal_connect(signupBtn,"clicked",G_CALLBACK(signup_button_clicked),showEmail);

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


 int main(int argc,char **argv)
 {
    GtkApplication *app;
    int status;
    app = gtk_application_new ("com.hackthedeveloper", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref (app);
    return status;
 }