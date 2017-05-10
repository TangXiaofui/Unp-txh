#include "unp.h"

void sig_child(int signo)
{
	pid_t pid;
	int stat;
	while((pid = waitpid(-1,&stat,WNOHANG)) > 0)
	{
		printf("child %d terminate\n",pid);
	}
	return;
}

void str_echo(int connfd)
{
	ssize_t n;
	char buf[MAXLINE];
again:
	while((n = Read(connfd,buf,MAXLINE)) > 0){
		Writen(connfd,buf,n);	
	}	
	if(n < 0 && errno == EINTR)
		goto again;
	if(n < 0)
		err_sys("read error");
}

int main(int argc,char *argv[])
{
	int listenfd,udpfd,connfd;
	const int on = 1;
	struct sockaddr_in seraddr,cliaddr;
	socklen_t clilen;
	char buf[MAXLINE];
	ssize_t n;

	int maxfd,nready;
	fd_set rset;
	pid_t childpid;
	//TCP
	listenfd = Socket(AF_INET,SOCK_STREAM,0);

	Setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
	
	bzero(&seraddr,sizeof(seraddr));
	seraddr.sin_family = AF_INET;
	seraddr.sin_port  = htons(SERV_PORT);
	seraddr.sin_addr.s_addr = htonl(INADDR_ANY);

	Bind(listenfd,(SA*)&seraddr,sizeof(seraddr));

	Listen(listenfd,LISTENQ);

	//UDP
	udpfd = Socket(AF_INET,SOCK_DGRAM,0);
	bzero(&seraddr,sizeof(seraddr));
	seraddr.sin_family = AF_INET;
	seraddr.sin_port  = htons(SERV_PORT);
	seraddr.sin_addr.s_addr = htonl(INADDR_ANY);

	Bind(udpfd,(SA*)&seraddr,sizeof(seraddr));

	Signal(SIGCHLD,sig_child);
	
	FD_ZERO(&rset);
	maxfd = max(listenfd,udpfd)+1;
	for(;;){
		FD_SET(listenfd,&rset);
		FD_SET(udpfd,&rset);
		nready = select(maxfd,&rset,NULL,NULL,NULL);
		if(nready < 0)
		{
			if(errno == EINTR)
				continue;
			else
				err_sys("unknow error");	
		}

		if(FD_ISSET(listenfd,&rset))
		{
			clilen = sizeof(cliaddr);
			connfd = Accept(listenfd,(SA*)&cliaddr,&clilen);
			if((childpid = Fork()) == 0){
				Close(listenfd);
				str_echo(connfd);
				exit(0);
			}
			Close(connfd);
		}

		if(FD_ISSET(udpfd,&rset)){
			clilen = sizeof(cliaddr);
			n = Recvfrom(udpfd,buf,sizeof(buf),0,(SA*)&cliaddr,&clilen);
			Sendto(udpfd,buf,n,0,(SA*)&cliaddr,clilen);	
		}
	}

		
}
