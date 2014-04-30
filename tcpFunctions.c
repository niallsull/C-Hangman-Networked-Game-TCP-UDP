// my tcp functions
#include "tcpFunctions.h"

/////////////////////////////////////////////////////////////////////////////
int MySocket(int domain,int type, int proto)
{
 int good=socket(domain,type,proto);
 
 if(good==-1)
 {
             printf("error creating client socket\n");
             exit(0);
 }            
 else
 {
     return good;
 }   
}
/////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////
int MyBind(int sock, struct sockaddr *server, int serverSize)
{
 int good=bind(sock,server,serverSize);
 
 if(good==-1)
 {
             printf("error binding socket \n");
             exit(0);
 }
 else
 {
     return good;
 }
     
    
}

////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
int MyListen(int sock, int backlog)
{
    
    int good=listen(sock,backlog);
    if(good==-1)
    {
                printf("error listening\n");
                exit(0);
    }
    else
    {
        return good;
    }
}
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
MyConnect(int sock, const struct sockaddr* cli ,int length)
{
    int good=connect(sock,cli,length);
    if(good==-1)
    {
                printf("error connecting \n");
                exit(0);
    }
    else
    {
                printf("connected\n");
    }
}
//////////////////////////////////////////////////////////////////////////

int MyAccept(int sock,struct sockaddr *cli,int *length)
{
 int good=accept(sock,cli,length);
 if(good==-1)
 {
             printf("error accepting\n");
             exit(0);
 }
 else
 {
     return good;
 }   
}
///////////////////////////////////////////////////////////////////////////
