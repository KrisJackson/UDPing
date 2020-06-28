# UDP Echo Client / Server

This project contains two C programs that uses User Datagram Protocol (UDP) to act as a network echo client and server pair.

**Ping Program (client)**: This program creates an array, sends it using UDP to the Pong server, and waits to receive a modified version of the array from the server. After the communication has completed, the program prints the elapsed time for the communication round trip.

More specifically, the program:
* creates a char array of N elements, where N may be specified as a command-line option. 
* uses getaddrinfo() to get the IP address, port number, etc. of the pong server (using IPv4 for network protocols)
* calls socket() using the information returned from getaddrinfo()
* loops over the number of ping packets to be sent.
* computes the total and average times spent communicating

Loop for ping packets does the following:
1. sends the array elements in a single packet to the pong server
2. waits to receive the reply from the pong server
3. validate the results sent from the pong server
4. prints out the round-trip time

**Pong Program (server)**: After startup, the program listens for UDP ping packets from the client program. Once the packet has been received, it modifies the values slightly and sends back the new array.

More specifically, the program:
* uses getaddrinfo() to get the information needed to create an IPv4 UDP server socket to listen on any available address on a port number (specified through the command line).
* calls socket() using the information returned from getaddrinfo().
* calls bind() to have the socket listen for incoming packets
* loops over the number of ping packets received.

Loop for ping packets does the following:
1. receives the array elements in a single packet from the ping client
2. adds 1 to every element in the array
3. sends the array back in a UDP packet to the ping client

## Sample Output
Server run in one window:
```
$ ./pong -n 10 -p 8000
pong[0]: received packet from 127.0.0.
pong[1]: received packet from 127.0.0.
pong[2]: received packet from 127.0.0.
pong[3]: received packet from 127.0.0.
pong[4]: received packet from 127.0.0.
pong[5]: received packet from 127.0.0.
pong[6]: received packet from 127.0.0.
pong[7]: received packet from 127.0.0.
pong[8]: received packet from 127.0.0.
pong[9]: received packet from 127.0.0.
```
And the client in another window:
```
$ ./ping -h localhost -p 8000 -n 10
ping[0] : round-trip time: 0.131 ms
ping[1] : round-trip time: 0.123 ms
ping[2] : round-trip time: 0.070 ms
ping[3] : round-trip time: 0.060 ms
ping[4] : round-trip time: 0.048 ms
ping[5] : round-trip time: 0.053 ms
ping[6] : round-trip time: 0.040 ms
ping[7] : round-trip time: 0.049 ms
ping[8] : round-trip time: 0.040 ms
ping[9] : round-trip time: 0.053 ms
no errors detected
time to send 10 packets of 100 bytes 0.667 ms (0.067 avg per packet)
```

***This project was assigned in my Computer Networking course, and this README was originally created by the Computer Science department at Rhodes College. Parts of this README has been edited to fit the format of this repo. Files in this repo contain my code, and should not be used or copied for any reason other than to assess my programming skills.***
