#include "unp.h"

void dg_cli(FILE *in,int serfd,const SA* seraddr,socklen_t serlen)
{
	ssize_t n;
	char recvbuf[MAXLINE],sendbuf[MAXLINE];
	Connect(serfd,(SA*)seraddr,serlen);

	while(Fgets(sendbuf,MAXLINE,in) != NULL)
	{
		Write(serfd,sendbuf,strlen(sendbuf));
		n = Read(serfd,recvbuf,MAXLINE);
		recvbuf[n] = 0;
		Fputs(recvbuf,stdout);
	}
}

int main(int argc,char *argv[])
{
	int sersock;
	struct sockaddr_in seraddr;
	
	if(argc < 2)
		err_sys("usage: cli <IPAdress>");
	sersock = Socket(AF_INET,SOCK_DGRAM,0);
	
	bzero(&seraddr,sizeof(seraddr));

	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(SERV_PORT);
	Inet_pton(AF_INET,argv[1],&seraddr.sin_addr);

	dg_cli(stdin,sersock,(SA*)&seraddr,sizeof(seraddr));
	exit(0);
}
