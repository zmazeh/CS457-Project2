#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>

int sockfd, newsockfd, portno;

void sig_handler(int signal)
{
	if(signal == SIGINT)
	{
		printf("\nclosing sockets\n");
		close(sockfd);
		close(newsockfd);
		exit(1);
	}
}

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

typedef struct chainLink 
{
	char SSaddr[256];
	int SSport;
}chainLink;

typedef struct packetUp
{
	short urlLen;
	short chainLen;	
	char URL[256];
	struct chainLink chain[10];

}packetUp;

typedef struct packetDown{
	short dataLen;
	int data;//not sure if this is the right type it should be
}packetDown;

int main(int argc, char * argv[]){
	int c;
    char *pValue = "49155";
    struct chainLink chain[10];
	while((c = getopt(argc,argv,"p:")) !=-1){
     		switch (c)
     		{
     			case 'p':
     				pValue = optarg;
     				break;
     			default:
     				error("ERROR: Flag not supported");
     				break;
     		}
	
     }
     portno = atoi(pValue);
     if(portno <= 0 || portno > 65535){
		fprintf(stderr,"ERROR: Invalid port number\n");
		exit(1);
	}
	
	if (signal(SIGINT, sig_handler) == SIG_ERR)
		printf("\ncan't catch SIGINT\n");
     socklen_t clilen;
     struct addrinfo hints, *res;
     struct sockaddr_in serv_addr, cli_addr;
     struct hostent *server;
     int n;
     int err;
     int sock;
     memset(&hints, 0, sizeof(hints));
     hints.ai_family = AF_UNSPEC; 
     hints.ai_socktype = SOCK_DGRAM;
     char hostname[1024];
     hostname[1023] = '\0';
     gethostname(hostname, 1023);
     err = getaddrinfo(hostname,"12345",&hints,&res);
     sock = socket(res->ai_family,res->ai_socktype,0);
    if(sock < 0){
        perror("socket");
        return 1;
    }
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
	    error("ERROR opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
		error("ERROR on binding");
    struct sockaddr_in *addr;
	addr = (struct sockaddr_in *)res->ai_addr;
	fprintf(stdout,"My host name is %s. Listening on port %d\n", hostname, portno); //print out IP here
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0) 
		error("ERROR on accept");
	printf("Connected...\n");
	//int n;
	while(1){
		//on connection take in URL and chain args
		//struct *Packet ssInfo;
		//n = read(newsockfd, ssInfo, 256/*size = not sure yet */);
		
		//create a thread using pthread_create and pass the args
		/*If the chain list is empty:
    		The ss uses the system call system() to issue a wget to retrieve the file specified in the URL.
    		Reads the file in small chunks and transmits it back to the previous SS. The Previous SS also receives the file in chunks.
    		Once the file is completely transmitted, the ss should tear down the connection.
    		Erase the local copy and go back to listening for more requests.
    	If the chain list is not empty:
    		Uses a random algorithm such as rand() function to select the next SS from the list.
    		Remove the current ss details from the chain list and send the url and chainlist to the next ss.
    		Wait till you receive the fill from the next ss.
    		Reads the file in small chunks and transmits it back to the previous SS. The Previous SS also receives the file in chunks.
    		Once the file is completely transmitted, the ss should tear down the connection.
    		Erase the local copy and go back to listening for more requests.
		*/

		
		
		
		
	}
	
return 0;
}
