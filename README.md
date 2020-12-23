# Simple Talk
Chat between two terminals with this simple chat system.
Connect with UDP by inputing IP and port number.
This program will be multithreaded to allow concurrency.

## Installation
1. Clone the repository
```bash
git clone https://github.com/Toooo123/Simple-Talk.git
```
2. Make the objects and executable file: 
```bash
make
```
### Run
1. Run the executable in two local or remote terminals:
```bash
s-talk [my port #] [remote IP] [remote Port #]
```
2. Start chatting by inputing messages and pressing enter
3. Terminate both session with typing this input in one of the terminals.
```bash
!
```

## Threads
This program will run with six threads.
Four are used for the chat system and two are used for intializing and cleaning up.

| Threads | Purpose | 
| :------ | :------ |
| Main | Starts the other five threads |
| Keyboard Input | Receive inputs from local computer <br/> Sends messages to Sender |
| Sender | Receive messages from Keyboard Input <br/> Sends messages to remote terminal |
| Receiver | Receive messages from remote terminal <br/> Sends messages to Screen Output |
| Screen Output | Receive messages from Receiver <br/> Prints messages to screen |
| Shutdown Manager | Locked when messaging system is active <br/> Active when message system terminates |
