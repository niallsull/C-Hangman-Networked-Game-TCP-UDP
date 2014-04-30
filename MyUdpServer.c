/*
**  hangman pure built in, had in a separate function, but wasn't calling it somehow
*/
#include <stdbool.h>
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
# define NUM_OF_WORDS (sizeof (word) / sizeof (word [0]))
#define MYPORT "4950"	// the port users will be connecting to

#define MAXBUFLEN 100
 char *word [] = {
 # include "words"
 };

typedef struct 
{
	int id;
	int state;
	int lives;
	char * whole_word;
	char part_word[20];
	int word_len;
}Game;

int gameNumber=0;
Game games[10];
char buf[MAXBUFLEN];


void getWord()
{
	
}

bool checkId(int id)  // if id greater than zero, is a real player
{
	if(id<0)
	{
		return false;
	}
	
	return true;
}
// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{
	
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int numbytes;
	struct sockaddr_storage their_addr;
	void Hangman(int id,int letter);  // wouldn't work
	socklen_t addr_len;
	char s[INET6_ADDRSTRLEN];
	int nextId=1;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; 
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, MYPORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can

	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("listener: socket");
			continue;
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("listener: bind");
			continue;
		}

		break;
	}
	
	if (p == NULL) {
		printf("Bind failed\n");
		return 2;
	}

	freeaddrinfo(servinfo);

	printf("listener: waiting to recvfrom...\n");

	addr_len = sizeof their_addr;
	
	while(1)
	{
		numbytes=recvfrom(sockfd,buf,MAXBUFLEN-1,0,(struct sockaddr *)&their_addr, &addr_len);  // recieve clients
		
		int i=0;
		int j=0;
		while((buf[j]!=' ') && (buf[j]!='\0'))
		{
			j++;
		}
		
		int id;
		char idbuf[j];
		for(i=0;i<j;i++)
			idbuf[i]=buf[i];
		
		id=atoi(idbuf); // interpretid
		
		bool player=checkId(id); // and check if a player
		
		if(player==true)  // continue game
		{
			
			int g;
		int t;
		for(t=0;t<gameNumber;t++)
		{
			// find which game slot they belong to
			
			if(id==games[t].id)
			{
				g=t;
				
				t=gameNumber+10; // break
			}		
		}	
		
		char guess=buf[j+1]; // get guess
		
		int correct=0;
		for(t=0;t<games[g].word_len;t++)
		{
			if(games[g].whole_word[t]==guess)
			{
				games[g].part_word[t]=guess;
				correct=1;
				
			}
		}

		if(correct==0)// take away life if word doesn't contain letter
			games[g].lives=games[g].lives-1;
			
		correct=1;
		for(t=0;t<games[g].word_len;t++)
		{  // then check if they have fully guessed the word
			if(games[g].whole_word[t]!=games[g].part_word[t])
			{
				correct=0;
			}
		}

		if(correct==1)  // if they won 
			numbytes=sprintf(buf,"Win %s %d\n",games[g].part_word,games[g].lives);

		else if(games[g].lives > 0)  // still have chance
			numbytes=sprintf(buf,"%s %d\n",games[g].part_word,games[g].lives);
		else   // if didn't win and no chances, obviously lost
			numbytes=sprintf(buf,"Lose %s %d\n",games[g].whole_word,games[g].lives);
			printf("%s\n",buf);
		}

		// create new game ++ id;		
		else
		{
			printf("into game\n");
			
			Game g;
			// make a whole new game
			g.id=nextId;
				nextId++;
			srand(time(NULL));

			g.whole_word=word[rand() % NUM_OF_WORDS];

			//int i;
			g.word_len = strlen(g.whole_word);

			g.lives=12;
			int l=0;
			while(l<g.word_len)
			{	
				
				g.part_word[l]='-';
				l++;
				
			}
			
				

			g.part_word[l] = '\0';

			g.state=0; // not even used
			games[gameNumber]=g;
			gameNumber++;
			gameNumber=gameNumber%10;
			// full blank word to client
			numbytes=sprintf (buf, "%d %s %d \n",g.id, g.part_word, g.lives);
			printf("%s\n",buf);
		}


		int c=sendto(sockfd,buf,numbytes,0,(struct sockaddr *)&their_addr, addr_len);
		//printf("sent to client :  %s\n",buf);
	}
close(sockfd);
	return 0;


}

void Hangman(int id,int letter)  // hangman function that never got called
{
	printf("actually in hangan\n");
	int g;
	int i;
	for(i=0;i<gameNumber;i++)
	{
		if(id==games[i].id)
		{
			g=i;
			break;
		}	
	}	
	
	char guess=buf[letter];
	printf("%c\n",guess);
	int correct=0;
	for(i=0;i<games[g].word_len;i++)
	{
		if(games[g].whole_word[i]==guess)
		{
			games[g].part_word[i]=guess;
			correct=1;
			printf("%s %c\n",games[g].whole_word,guess);
		}
	}

	if(correct==0)
		games[g].lives=games[g].lives-1;

	sprintf(buf,"%s %d\n",games[g].part_word,games[g].lives);
	
	return;
}
