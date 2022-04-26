# Network Programming in C
 
### Vedaant Kuchhal and Charlie Babe
 
 
## Goals of Current Project
Gain experience in network programming in C. We will build a chat interface to allow users to communicate over the network. Our lower bound would be to implement a basic terminal chat interface using socket programming, while a stretch goal would be to implement a GUI for the chat communication. A *stretch* stretch goal would be to implement the functionality of the iOS feature ‘Airdrop` in C.
 
## Our Learning Goals
**Vedaant:** My main goal is to learn how network communications work not only in the ‘hard’ coding sense, but also in a general systems/process sense so that I can quickly grasp how to do implementations in other languages and contexts. In addition, I want to build a project that has more ‘real world’ interactiveness. My previous project was a Shell, and while I enjoyed it and learned a lot, this project I want to gear myself more towards a project that has more polished applications.
 
**Charlie:** My primary goal is to learn more about network communication protocols, and learn about how wireless transmission protocols like WiFi and Bluetooth work. I also want to learn more about the security of wireless transmission protocols and how trust is established in a world where wireless transmissions can be sniffed. In addition, I want to build a project that I could potentially see having real world impact because I feel more motivated by working on novel problems and solutions since I am actually contributing something to the world.
 
## What we have done to get started
So far, we have implemented a basic working chat interface over Bluetooth on the terminal. This works by enabling a client device to scan for active servers around it, select which device to connect, and connect to it and send each other texts simultaneously (we achieved this using multi-threading). 
 
We have used the following resources to help us - 
1. Bluetooth sockets programmingL https://people.csail.mit.edu/albert/bluez-intro/x502.html
2. Multi-threading: https://stackoverflow.com/questions/3051009/c-run-two-functions-at-the-same-time
3. *Beej’s Guide to Network Programming*
4. Asking other SoftSys teams for help!
5. Many other websites to build our understanding of network programming and multithreading
 
We anticipate that while our existing resources on Bluetooth sockets programming would be sufficient to help us in that area, we would probably need more resources on figuring out GUIs for our chat interface. After talking to a CA, we were advised to look at [GTK](https://www.gtk.org/docs/installations/linux/) for GUIs.
 
## What we are working on now:
1. Allow the server to accept or deny connections from the client to allow the user operating the server to screen connected clients. Vedaant will be working on this. This task will be completed when the user operating the server can easily screen incoming client connections and select the clients the user wants to chat with.
2. Implement two frames, one with outgoing messages and one with incoming messages to allow for an improved chat experience over a single terminal window. Charlie will be working on this. This task is done when the data streams for the incoming and outgoing messages are split and it is easy for the user to view both streams of messages. Splitting message streams is necessary to ensure that user prompts will not be overwritten by incoming messages.
3. Create a GUI for the peer to peer chat. Charlie will be working on this. This task is done when the user can seamlessly scan, connect and chat with other bluetooth devices running our program without having to issue terminal commands and instead just press buttons.
 

[GitHub](https://github.com/olincollege/SoftSysNetworkingNewts)

[Trello](https://trello.com/invite/b/c4UGdFFN/a49a40527b9a1ebe1be4f3cab3f8d76e/project-management)

