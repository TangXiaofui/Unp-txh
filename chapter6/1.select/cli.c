#include "unp.h"

void str_cli(FILE *fp,int sersock)
{
	char sendbuf[MAXLINE],recvbuf[MAXLINE];
	int maxfd;
	fd_set rset;
	FD_ZERO(&rset);	
	for(;;){
		FD_SET(fileno(fp),&rset);
		FD_SET(sersock,&rset);
		maxfd = max(fileno(fp),sersock)+1;
		Select(maxfd,&rset,NULL,NULL,NULL);
		if(FD_ISSET(sersock,&rset)){
			if(Readline(sersock,recvbuf,MAXLINE) == 0)
				err_quit("server terminate permatution");
			Fputs(recvbuf,stdout);
		}

		if(FD_ISSET(fileno(fp),&rset)){
			if(Fgets(sendbuf,MAXLINE,fp) == NULL)
				return ;
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
	int i;
	sock = Socket(AF_INET,SOCK_STREAM,0);

	bzero(&servsock,sizeof(servsock));

	servsock.sin_family = AF_INET;
	servsock.sin_port = htons(SERV_PORT);
	Inet_pton(AF_INET,argv[1],&servsock.sin_addr);

	Connect(sock,(SA*)&servsock,sizeof(servsock));

	str_cli(stdin,sock);
	exit(0);
}
