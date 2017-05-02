#include "unp.h"


void sig_child(int signo)
{
	pid_t pid;
	int stat;
	while( (pid = waitpid(-1,&stat,WNOHANG))>0)
	{
		printf("child %d terminate\n",pid);
	}
	return ;
}

void str_echo(int sockfd)
{
	ssize_t n;
	char buf[MAXLINE];
again:	
	while( (n = read(sockfd,buf,MAXLINE)) > 0){
		printf("%s\n",buf);
		Writen(sockfd,buf,n);
	}	
	if(n < 0 && errno == EINTR)
		goto again;
	else if(n < 0)
		err_sys("str_echo:read error");
}


int main(void)
{
	int sock,clisock;
	struct sockaddr_in servaddr,cliaddr;
	socklen_t len = sizeof(struct sockaddr_in);
	pid_t childpid;
	sock = Socket(AF_INET,SOCK_STREAM,0);
	
	bzero(&servaddr,len);
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	Signal(SIGCHLD,sig_child);
	Bind(sock,(SA*)&servaddr,len);
	Listen(sock,LISTENQ);
	
	for(;;){
		int clilen = sizeof(cliaddr);
		if((clisock = accept(sock,(SA* )&cliaddr,&clilen)) < 0)
		{
			if(errno == EINTR)
				continue;
			else
				err_sys("accept error");	
		}
		childpid = Fork();
		if(childpid == 0)
		{
			Close(sock);
			str_echo(clisock);
			exit(0);
		}
		Close(clisock);
	}
	exit(0);		
}
