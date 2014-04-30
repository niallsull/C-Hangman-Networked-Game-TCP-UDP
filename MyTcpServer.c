// tcp server
#include "tcpFunctions.h"
#include <sys/wait.h>
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
#include "Hangman.h"
 # define HANGMAN_TCP_PORT "1066"
 extern time_t time ();

 // get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

 
 int main()
{


	int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
	struct addrinfo hints, *serverinfo, *p;
	struct sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	struct sigaction sa;
	int yes=1;
	char s[INET6_ADDRSTRLEN];
	int rv;
	
	pid_t pid;
	void sig_chld(int);
	
	bzero(&hints, sizeof (struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP
	
	if ((rv = getaddrinfo(NULL, HANGMAN_TCP_PORT, &hints, &serverinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}
	
	
	p=serverinfo;
	
	do{ // find a socket that we can bind to
	
		sockfd = MySocket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if(sockfd<0)
			continue;
			
			
		int success=MyBind(sockfd, p->ai_addr, p->ai_addrlen);
		if(success>=0)
			break;
	
	
	}while((p=p->ai_next) != NULL);
	
	
	
	freeaddrinfo(serverinfo);
	
	
	int goodListen=MyListen (sockfd, 5);
	
	 signal(SIGCHLD, sig_chld);
	 
	 while (1) {
 		sin_size = sizeof their_addr;
 		// accept incoming clients
 		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1) {
			perror("accept");
			continue;
		}
 		
		// fork 
		if( (pid = fork()) ==0 )
		{
			close(sockfd);
			play_hangman(new_fd,new_fd);
			close(new_fd);
			exit(0);
		}
		//
 		//play_hangman (fd, fd);
 		close (new_fd);
 	}
}

void sig_chld(int signo)      // deal with zombie children
{
	pid_t pid;
	int stat;

	while( (pid=waitpid(-1, &stat, WNOHANG)) > 0)
		printf("child %d terminated\n", pid);
	return;
}
