#include "unp.h"

void str_cli(FILE *fp,int sersock)
{
	char sendbuf[MAXLINE],recvbuf[MAXLINE];
	int maxfd;
	fd_set rset;
	int stdineof = 0;
	int n;
	FD_ZERO(&rset);
	for(;;)
	{
		if(!stdineof)
			FD_SET(fileno(fp),&rset);
		FD_SET(sersock,&rset);
		maxfd = max(sersock,fileno(fp))+1;
		Select(maxfd,&rset,NULL,NULL,NULL);
		if(FD_ISSET(sersock,&rset))
		{
			if( (n = read(sersock,recvbuf,MAXLINE) == 0))
			{
				if(stdineof)
					return ;
				else
					err_sys("server terminated");
			}
			Write(fileno(fp),recvbuf,n);
		}

		if(FD_ISSET(fileno(fp),&rset))
		{
			if(read(fileno(fp),sendbuf,MAXLINE) == 0)
			{
				stdineof = 1;
				FD_CLR(fileno(fp),&rset);
				Shutdown(sersock,SHUT_WR);
				continue;
			}
			Writen(sersock,sendbuf,strlen(sendbuf));
		}
	}
}


int main(int argc,char *argv[])
{
	if(argc < 2)
		err_quit("usage cli IPAddress");
	
	struct sockaddr_in servsock;
	int sock;
	sock = Socket(AF_INET,SOCK_STREAM,0);

	bzero(&servsock,sizeof(servsock));

	servsock.sin_family = AF_INET;
	servsock.sin_port = htons(SERV_PORT);
	Inet_pton(AF_INET,argv[1],&servsock.sin_addr);

	Connect(sock,(SA*)&servsock,sizeof(servsock));

	str_cli(stdin,sock);
	exit(0);
}
