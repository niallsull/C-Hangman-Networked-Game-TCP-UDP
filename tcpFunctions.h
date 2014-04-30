// function prototypes // 
#ifndef TCPFUNCTIONS_H
#define TCPFUNCTIONS_H
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>


int MySocket(int domain,int type, int proto);

int MyBind(int sock, struct sockaddr *server, int serverSize);
int MyListen(int sock, int backlog);
MyConnect(int sock, const struct sockaddr *cli ,int length);
int MyAccept(int sock, struct sockaddr *cli, int *length);

#endif
