#include "unp.h"
#include <time.h>

int main(int argc,char *argv[])
{
	int listenfd,clientfd;
	struct sockaddr_in sersock,cliaddr;
	char buff[MAXLINE+1];
	time_t 	ticks;

	listenfd = Socket(AF_INET,SOCK_STREAM,0);

	bzero(&sersock,sizeof(sersock));

	sersock.sin_family = AF_INET;
	sersock.sin_port = htons(9999);
	sersock.sin_addr.s_addr = htonl(INADDR_ANY);

	Bind(listenfd,&sersock,sizeof(sersock));

	Listen(listenfd,LISTENQ);

	int len = sizeof(cliaddr);
	for(;;){
		clientfd = Accept(listenfd,(SA*)&cliaddr,&len);

		printf("connect from %s ,port %d \n",
			Inet_ntop(AF_INET,&cliaddr.sin_addr,buff,sizeof(buff)),
			ntohs(cliaddr.sin_port)	
		);

		ticks = time(NULL);
		snprintf(buff,sizeof(buff),"%.24s\r\n",ctime(&ticks));
		Write(clientfd,buff,strlen(buff));
		Close(clientfd);	
	
	}


	exit(0);	
}
