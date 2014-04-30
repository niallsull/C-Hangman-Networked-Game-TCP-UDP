/* Hangclient.c - Client for hangman server.  */
#include "tcpFunctions.h"
 #include <stdio.h>
 #include <sys/types.h>
 #include <sys/socket.h>
 #include <netinet/in.h>
 #include <netdb.h>
 #include <sys/select.h>
 #include <sys/time.h>

 # define LINESIZE 80
 # define HANGMAN_TCP_PORT "1066"
 #define max(a,b) a>b?a:b
 
 
 // get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

 int main (int argc, char * argv [])
 {
 
 
 if (argc != 2) {
	    fprintf(stderr,"give ip address of server\n");
	    exit(1);
	}
	

	int sockfd, numbytes,count;  
	char i_line[LINESIZE];
 	 char o_line[LINESIZE];
	struct addrinfo hints, *servinfo, *p;
	int rv;
	
	//unspecified stream, tcp
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol= IPPROTO_TCP;
	if ((rv = getaddrinfo(argv[1], HANGMAN_TCP_PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}
	printf("%d \n",rv);
	p=servinfo;
	// take the first socket that connects
	do{
		sockfd = MySocket(p->ai_family,p->ai_socktype, p->ai_protocol);
		printf("%d",sockfd);
		if(sockfd<0)
			continue;

		if(MyConnect(sockfd,p->ai_addr,p->ai_addrlen)!=-1)
			break;

		close(sockfd);
	}while((p=p->ai_next) != NULL);
	
	
	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}
	
char s[INET6_ADDRSTRLEN];
	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),s, sizeof s);
	printf("client: connecting to %s\n", s);
	
	freeaddrinfo(servinfo);  // free up some memory
	
	fd_set rset;  // read set
	
	FD_ZERO(&rset);  // make it all zero
	
	int maxfdp1;
	for(;;)
	{

		FD_SET(sockfd,&rset);  // we want to read the socket, and stdin
		FD_SET(fileno(stdin),&rset);
		maxfdp1=max(fileno(stdin),sockfd)+1;
		select(maxfdp1,&rset,NULL,NULL,NULL); // check the read set

		if(FD_ISSET(sockfd,&rset)){  // is socket needs to be read
			count = read (sockfd, i_line, LINESIZE);
			write (1, i_line, count);
		}

		if(FD_ISSET(fileno(stdin),&rset))  // if stdin needs to be read
		{
			count = read (0, o_line, LINESIZE);//0 = STDIN
			if(count>0)				
				write (sockfd, o_line, count);
			else{  // if we recieve nothing presume finished
				close(sockfd);
				close(0);
			}
		}	

	}


	close(0);
 }
	
	
	
