# Network Programming in C
### Vedaant Kuchhal and Charlie Babe
## Goals of the Project
The goal of our project was to gain experience in network programming in C by creating a chat interface between two computers communicating over Bluetooth. The chat interface would allow users to communicate without the need for routers and network infrastructure as long as the two computers were physically close enough to each other to allow for a Bluetooth connection. Our lower bound goal was to implement a basic terminal chat interface using socket programming, while our stretch goal was to implement a GUI for the chat communication. Our *stretch* stretch goal was to implement the functionality of the iOS feature 'Airdrop' in C.
## Our Learning Goals
**Vedaant:** My main goal was to learn how network communications work not only in the ‘hard’ coding sense, but also in a general systems/process sense so that I can quickly grasp how to do implementations in other languages and contexts. In addition, I wanted to build a project that has more ‘real world’ interactiveness. My previous project was a Shell, and while I enjoyed it and learned a lot, this project I wanted to gear myself more towards a project that has more polished applications. Additionally, as we moved into the GUI-building phase of our project, I also became interested in learning GTK and how app development/learning a new library can be done most effectively.
**Charlie:** My primary goal was to learn more about network communication protocols, and learn about how wireless transmission protocols like WiFi and Bluetooth work. I also want to learn more about the security of wireless transmission protocols and how trust can be established in a world where wireless transmissions can be viewed. In addition, I wanted to build a project that I could potentially see having real world impact because I feel more motivated by working on novel problems and solutions since I am actually contributing something to the world.
 
## What we did:
[Link to Video Demonstration](https://youtu.be/16APHLGk7Kw)<br>
We implemented a client-server Bluetooth chat interface that would allow users to communicate without the need for networking infrastructure like routers. We tried to make our client and server roles as peer-to-peer as possible, but there were still some differences in the roles, and our code structure can broadly be understood by looking at the client and server side individually:
 
### Client
 
1. The client initiates communication and starts by scanning for nearby available Bluetooth devices (servers).
```
int i;
char** my_scan = scan_server();
printf("Select Device: ");
```
2. The nearby Bluetooth devices are detected, the user selects a device based on its index, and establishes a connection.
```
scanf("%i", &i);
printf("Selected: %s\n", my_scan[i]);
client_connection(my_scan[i]);
```
3. Following this, the client finally starts to create a socket and connect to it:
```
s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
 
// set the connection parameters (who to connect to)
addr.rc_family = AF_BLUETOOTH;
addr.rc_channel = (uint8_t) 1;
str2ba( MAC, &addr.rc_bdaddr );
printf("Trying to Connect\n");
 
// connect to server
status = connect(s, (struct sockaddr *)&addr, sizeof(addr));
```
4. Then the client creates two threads to handle reading and writing to the socket:
```
//Create thread for writing data to socket
rc = pthread_create(&threads[1], NULL, Write, (void*)s);
...
//Create thread for reading data from socket
rc1 = pthread_create(&threads[0], NULL, Read, (void*)s);
...
```
5. The read thread reads from the socket and prints the output to the terminal. Before it begins, however, it must check if the server has accepted the client's request to connect (this part will be discussed in more detail in the Server section).
```
//Read first input from the socket
if(read(sock, buf, sizeof(buf)) > 0 ){
//If the string sent is "No connect", close socket connection
   if (strcmp(buf, "No connect") == 0){
       printf("Connection denied\n");
   }
   printf("%s",b); //Print the received message
}
```
6. The write thread gets messages from the user through the terminal and writes the messages to the socket:
```
//Get message from standard input from user
fgets(message, sizeof(message), stdin);
//Write message to socket output
if(write(sock, message, sizeof(message)) < 0 ){
   perror("uh 1oh");}
}
```
### Server
The server waits for clients to connect to it then the user screens those clients to choose what client they want to connect to. The main steps of socket connection were:
1. First creating the socket:
```  s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);```
2. Then binding the socket to a port:
```bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));```
3. Putting the socket into listening mode:
``` listen(s, 1);```
4. Accept 1 connection from a client:
```client = accept(s, (struct sockaddr *)&rem_addr, &opt);```
5. The user then determines if they want to accept a connection from the client by typing a 'y' to connect or a 'n' to not. If the user chooses not to connect, the server writes a message to the client indicating that the server is closing the connection, then the server closed the connection:
```
printf("Do you want to connect?(y/n) ");
scanf("%c", &i);
if (i=='y'){
   //The two read and write threads
   ...
}
else{
   //Close connection
   char message[11]="No connect";
   if (write(client, message, sizeof(message)) < 0) {
       perror("Uh oh");
   }
   close(client);
   close(s);
}
```
6.  If the user decides to allow the connection the server creates two threads to handle reading and writing to the socket, which are the same functions used by the client to read and write to the socket.
### GUI Server
We tried to make a GUI to divide the two streams of incoming and outgoing messages, but we were unable to finish the GUI. The code for this GUI (which we tried to make using GTK) can be found in `GUI_server.c`. To see what it does, run `make GUI_server` and run the executable as if it were `server`, with the client remaining the same. We were able to get messages to send to the app and simultaneously receive messages using a fork, but we were unable to figure out how to make those messages appear in the app. We have included the code since we spent a considerable amount of time working on this but were unable to get past the final hurdle, which seemed to have required a deeper understanding of forking and how the GTK application works.
 
 
## How to Run the Code
 
### What You Need:
1. Two computers that are Bluetooth enabled
2. To use the GUI you must have GTK installed on both computers, GTK can be installed with the command: ```sudo apt-get install build-essential libgtk-3-dev```
 
### Running Code
One computer must be the client and the other computer must be the server. The server must be run first in order for it to be available and accept the client connection.
#### To run the server:
1. Enter the command ```make server```
2. Enter the command ```./server``` to run
#### To run the client:
1. Enter the command ```make client```
2. Enter the command ```./client``` to run
 
 
## Design Decision
One key design decision we made was to use multiple threads to handle the simultaneous reading and writing to the socket. Originally we built the chat interface to alternate between each of the users writing a single message, but this didn't allow users to write or read multiple messages in a row. By implementing threading we were able to have both users simultaneously read and write messages to each other. Unfortunately by making both reading and writing simultaneous and with all of the messages being outputted on the same terminal, reading the messages became more difficult because incoming messages could disrupt whatever message the user was in the process of typing.
 
Overall we believe the thread based approach to reading and writing to the socket was the best way to implement a chat interface, by allowing simultaneous and multiple reads and writes by each user.
 
## Reflection
The goal of our project was to gain experience in network programming in C by creating a chat interface between two computers communicating over Bluetooth. We succeeded in our MVP goal of creating a chat interface in the terminal between two computers over Bluetooth, we had trouble completing our strech goal of a GUI for our chat interface. As for our *stretch* stretch of implementing a similar functionality to 'Airdrop' in C, while we were unable to implement it, though we believe it would be entirely doable, by encoding the files data as ASCII and passing it over the socket and having it reassembled on the over side.
 
As for our learning goals:
**Charlie**: I completed my learning goal by learning more about socket programming, and through the process of scanning Bluetooth devices learned about the security of Bluetooth regarding how much information is advertised by unconnected Bluetooth devices.
 
**Vedaant**: I was able to achieve my learning goal of understanding how sockets work at a basic level, since Bluetooth socket connections involved a lot of self-learning and trying out things as opposed to those over the internet which are more extensively documented and were covered in class. Additionally, I had an unexpectedly eye-opening first dive into how GTK works and what it can potentially look like to start integrating a terminal-based application with a more polished GUI.
 
## Resources:
We have used the following resources to help us -
1) [Bluetooth sockets programming](https://people.csail.mit.edu/albert/bluez-intro/x502.html)
2) [Multi-threading](https://stackoverflow.com/questions/3051009/c-run-two-functions-at-the-same-time)
3) [Beej’s Guide to Network Programming](https://beej.us/guide/bgnet/)
4) [GTK official documentation](https://docs.gtk.org/)
 
[Link to our GitHub](https://github.com/olincollege/SoftSysNetworkingNewts)
