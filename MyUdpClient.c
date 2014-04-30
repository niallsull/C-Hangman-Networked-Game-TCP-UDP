/*
** 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define SERVERPORT "4950"	// the port users will be connecting to

int main(int argc, char *argv[])
{
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int numbytes;
	char buf[10];
	char readbuf[100];
	struct sockaddr_storage their_addr;	
	socklen_t addr_len = sizeof their_addr;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	if ((rv = getaddrinfo(argv[1], SERVERPORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and make a socket
	
	p=servinfo;
	do{
		sockfd=MySocket(p->ai_family,p->ai_socktype,p->ai_protocol);
		if(sockfd>=0)
			break;
	
	}while((p->ai_next)!=NULL);

	freeaddrinfo(servinfo);
	
	if (p == NULL) {
		printf("can't get server details\n");
		exit(0);
	}
	
	int count=sprintf(buf,"-1");  // LOOKING FOR A GAME , send negative id
	buf[count]='\0';
	if ((numbytes = sendto(sockfd,  buf,count, 0,p->ai_addr, p->ai_addrlen)) == -1) {
		perror("talker: sendto");
		exit(1);
	}
	// then get back our id from the server
	numbytes=recvfrom(sockfd,readbuf,100,0,p->ai_addr, &p->ai_addrlen);
	
	if(numbytes==-1){
		printf("recieved failed\n");
	}
	
	int id;
	int i=0;
	
	
		int j=0;
		while((readbuf[j]!=' '))   // find where the space ends, id will only take up one r two digits anyway
		{
			j++;
		}
		
		
		char idbuf[j];
		for(i=0;i<j;i++)
			idbuf[i]=readbuf[i];
		
		id=atoi(idbuf);  // we'll send this id at the start of any string, follwed by our guess
	
	//printf("id %d\n",id);
	printf("recv %s\n",readbuf);
	
	while(1)
	{	
		char send[10];
		count = read (0, send, 10);
		count = sprintf(buf,"%d %s",id,send);
		
		numbytes = sendto(sockfd, buf,count, 0,p->ai_addr, p->ai_addrlen);
	
		numbytes=recvfrom(sockfd,readbuf,100,0,p->ai_addr, &p->ai_addrlen);
		if(numbytes==-1){
		printf("recieved failed %d\n",numbytes);
		}
		printf("%s\n",readbuf);
		
		if((readbuf[0]=='L') || (readbuf[0]=='W'))  // we won or lost, so exit the game
			break;
	}
	
	printf("talker: sent %d bytes to %s\n", numbytes, argv[1]);
	close(sockfd);

	return 0;
}
