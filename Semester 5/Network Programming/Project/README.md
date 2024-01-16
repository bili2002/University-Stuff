
The project consists of three runnable parts - Server, Client, and CreateTest each in a separate folder. The server receives an array from the client, sorts it multithreaded with merge sort, and returns it to the client.
The client-server communication is implemented with the Selector paradigm.

Also, there is a comparison between the number of threads and the runtime of the communication, which you can look at in Timing.xlsx.

You can compile each part by using the Makefiles in the folders.

To run the Server:
```./Server [Port]```

To run the Client:
```./Client [FileWithData][Port][IP]```
