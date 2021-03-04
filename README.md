# Project-1-Accio
Chenrong/Chen Su
NUID: 43124659
Server:
	The server that I've designed utilize the TCP IPv4 socket that binds and listen for incoming files on a specific 
	port defined by the user, and advancing to a particular working directory for receiving the incoming file for that connection,
	while storing the file in a customized name format. Morever, the client socket will be closing after the file transmission is complete,
	,a timeout of 10 sec occured while conncting to the client, or recived a SIGQUIT/SIGTERM signal from the terminal.
	
Client:
	The client that I've designed Utilize the TCP IPv4 Socket to send a file to the server socket with destination port,address, target file
	from command line arguement, with the timeout limit of connection and transmission of 10 seconds. Moreover, for the implementation of 
	timeout limitation, this client socket has been designed into non-blocking mode in order to utilize the select() for timeout testing.
	
Problems:
	The first problem for me is initiliy I think that we have to determine if the fize size is larger than 1MB, and trying to send the file all at once,
	therefore I was trying to set a limitation of file size, and I was running into struggles when I was trying to determine the size of the file from the server's perspective. 
	The way I solve the problem is that I asked a question to my TA and Bhvana and she clear me out that we are not required to set a specific limitation
	on the file size. Therefore, I change my perspective to divide the files down into small chunks and send the chunk till all data is send, similar princile
	are applied on the server's end
	
	The second problem is that I couldn't figure out a way to determine a timeout limit for my client and server, originally I was thinking that I should
	do a form of system.time and do the calculation from that, however it's not working for me pretty well, therefore I've done some research on the way
	that can be implemented, and I found the procedure of using select() with non-blocking mode of socket. When I implemented such procedure, it is still
	not working for me properly, therefore I consulted Bhvana again and she give me a suggestion to change the socket mode before I made the connection,
	and such procedure worked but generated some more error and now my socket aren't able to transfer any file, therefore I've done some research on the 
	non-blocking mode implementation of socket and reliazed that I was missing some components, after I implemented my misssing pieces, problem is solved.
	
Additional Libraries:
	errno.h
	sys/select.h
	csignal.h
	fstream.h	
	sys.stat.h
	time.h
	fcntl.h
	
	
Additional Site Referenced:
https://www.codeproject.com/Tips/168704/How-to-set-a-socket-connection-timeout
https://www.scottklement.com/rpg/socktimeout/timingOut.html
https://www.ibm.com/support/knowledgecenter/SSLTBW_2.1.0/com.ibm.zos.v2r1.hala001/orgblockasyn.html
https://pubs.opengroup.org/onlinepubs/009695399/functions/connect.html
