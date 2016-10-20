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

/*void sig_handler(int signal)
{
	if(signal == SIGINT)
	{
		printf("\nclosing sockets\n");
		close(sockfd);
		close(newsockfd);
		exit(1);
	}
}
*/ 
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

typedef struct packetUp
{
	short urlLen;
	short chainLen;	
	char URL[256];
	struct chainLink chain[10];

}packetUp;

int main(int argc, char * argv[]){
	int c;
	char *fileName = "chaingang.txt";
	FILE *fp;
	char buff[256];
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
		
		int chaincount = 0;
		while(fscanf(fp, "%s",buff) !=EOF){
			if(lineNum == 1){
				chainSize = atoi(buff);
				printf("chainSize = %zu\n",chainSize);
				chain = calloc(chainSize, 28);
				printf("sizeof(chain) = %lu\n",sizeof(chain)); //why is sizeof(chain) = 8??  
			}else{
				struct chainLink link;
				if(buffcount % 2 == 0){ //if we are looking at the address
					strcpy(addr,buff);
					addr[strlen(addr)-1] = '\0';
					printf("addr = %s\n",addr);
					strcpy(link.SSaddr,addr);
					printf("link.SSaddr = %s\n", link.SSaddr);
				}else{//if we are looking at the port number
					port = atoi(buff);
					printf("port = %d\n",port);
					link.SSport = port;
					printf("link.SSport = %d\n", link.SSport);

				}
				
				buffcount++;

				
			}
		memcpy(&chain[chaincount],&link,sizeof(link));
		printf("chain%d addr = %s\n", chaincount, chain[chaincount].SSaddr);
		printf("chain%d port = %d\n", chaincount, chain[chaincount].SSport);
		chaincount++;
			lineNum++;
			
		}
		
		free(chain);
		fclose(fp);
		/*printf("chain0 addr = %s\n", chain[0].SSaddr);
		printf("chain0 port = %d\n", chain[0].SSport);
		printf("chain1 addr = %s\n", chain[1].SSaddr);
		printf("chain1 port = %d\n", chain[1].SSport);
		printf("chain2 addr = %s\n", chain[2].SSaddr);
		printf("chain2 port = %d\n", chain[2].SSport);
		*/
	}
	
	return 0;
}
