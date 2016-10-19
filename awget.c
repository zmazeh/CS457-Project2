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
		int chainSize;
		int lineNum = 1;
		while(fscanf(fp, "%s",buff) !=EOF){
			
			if(lineNum == 1){
				chainSize = atoi(buff);
			}else{
				char *addr;
				int port;
				struct *chainLink[chainSize];
				 
			}
			printf("%s",buff);
			lineNum++;
			
		}
		printf("chainSize = %d\n",chainSize);
		fclose(fp);
	}
	return 0;
}
