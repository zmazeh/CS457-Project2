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
	char SSaddr[20];
	int SSport;
}chainLink;

//I created a struct chain because the chain is dynamically allocated so I cannot get the size... (this might be redundent...)
typedef struct Chain{
	int size;
	struct chainLink *chain;
}Chain; 


typedef struct packetUp
{
	short urlLen;
	short chainLen;	
	char *URL;
	struct Chain chain;

}packetUp;

int main(int argc, char * argv[]){
	int c;
	char *fileName = "chaingang.txt";
	FILE *fp;
	char buff[256];
	char *location = argv[1];
	//printf("location = %s\n",location);
	if(argc < 2){
		error("Usage: ./awget <URL> [-c chainfile]\nchainfile is optional\n");
	}
	while((c = getopt(argc,argv,"c:")) !=-1){
     		switch (c)
     		{
     			case 'c':
     				fileName = optarg;
     				break;
     			default:
     				error("ERROR: Flag not supported");
     				break;
     		}
	
     }
	fp = fopen(fileName, "r");
	if(fp){
		size_t chainSize;
		int lineNum = 1;
		int buffcount = 0;
		struct chainLink *chain;
		char addr[256];
		int port = 0;
		struct Chain _chain; 
		struct chainLink link;
		int chaincount = 0;
		while(fscanf(fp, "%s",buff) !=EOF){
			if(lineNum == 1){
				chainSize = atoi(buff);
				//printf("chainSize = %zu\n",chainSize);
				chain = malloc (chainSize * sizeof(chainLink));
				_chain.size = chainSize;
				_chain.chain = chain;
				
			}else{
				
				if(buffcount % 2 == 0){ //if we are looking at the address
					strcpy(addr,buff);
					addr[strlen(addr)-1] = '\0';
					//printf("addr = %s\n",addr);
					strcpy(link.SSaddr,addr);
					//printf("link.SSaddr = %s\n", link.SSaddr);
				}else{//if we are looking at the port number
					port = atoi(buff);
					//printf("port = %d\n",port);
					link.SSport = port;
					//printf("link.SSport = %d\n", link.SSport);
					chain[chaincount] = link;
					chaincount++;
				}
				buffcount++;
			}
			lineNum++;			
		}		
		//for testing purposes
		/*int index;
		for(index = 0; index < _chain.size; index++){
			printf("chain%d addr = %s\n", index, chain[index].SSaddr);
			printf("chain%d port = %d\n", index, chain[index].SSport);
		}
		*/
		
		//now to populate the packet to send to the stepping stones
		struct packetUp sendingPacket;
		sendingPacket.urlLen = strlen(location);
		sendingPacket.chainLen = _chain.size;
		sendingPacket.chain = _chain;
		sendingPacket.URL = location;
		
		//now send the packet
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
     	int c;
     	portno = 49155;
     	sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd < 0) 
		    error("ERROR opening socket");
		server = gethostbyname("127.0.0.1");//chose random ip from chain
		if (server == NULL) {
		    fprintf(stderr,"ERROR: no such host\n");
		    exit(0);
		}
		printf("Connecting to server...");
		bzero((char *) &serv_addr, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		bcopy((char *)server->h_addr, 
		     (char *)&serv_addr.sin_addr.s_addr,
		     server->h_length);
		serv_addr.sin_port = htons(portno);
		if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
		{
			printf("\n");
		    error("ERROR connecting");
		}
		printf("Connected!\n");
		printf("Connected to a friend! You send first.\n");
		free(chain);
		fclose(fp);
	}
	
	return 0;
}
