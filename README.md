                                                     
                                                      WeiChat
                                         By Michael Luo and Wei Wen Zhou
**_Description of Project:_**

For our project, we want to create something similar to an IRC(Internet Relay Chat) which is essentially a system for chatting.  Upon starting the program, the user will be prompted to either create an account or log back in.  Afterward, they will be automatically be put into the main room in which their messages will be received and seen by everyone else in the main room.  From there, the user will also be create/enter chat rooms where they can chat with someone one on one.  Like an IRC, the text itself will be very simple due to limitations of the terminal(No pictures, voice calls….etc).  In addition to the main room, we plan on adding additional servers for users to chat on overall as a group which will only relay chat to the people on the channel.

**_Instructions:_**

Compile everything with "make select".  Afterwards, run "./server" on one terminal and add clients with "./client" on other terminals.  On the clients, either type "create" to create an account or type "login" to log back into an existing account.  To exit the program, make sure you use "Ctrl + C" on the clients before doing that to the server.

Once you are logged in, you will automatically be entered into the main room chat server.  In order to switch to a different channel, you will first need to create a channel by using the command "#create _____", in which the ______ could be any port number which is not already in use.  Essentially, you can pick any number besides 9001 (since it is the port number of the main room).  Afterwards, you can join the channel you created or channels other people have created by typing in "#join ______" in which the _____ is the port number of the channel you wish to join.

**_Bugs:_**     
Make sure that you do not accidentally type a port number in #join that does not exist/or created as it will lead to a segmentation fault.
