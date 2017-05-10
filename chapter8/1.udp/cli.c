#include "unp.h"

void dg_cli(FILE *in,int serfd,const SA* seraddr,socklen_t serlen)
{
	ssize_t n;
	socklen_t len;
	struct sockaddr replyAddr;
	char recvbuf[MAXLINE],sendbuf[MAXLINE];
	while(Fgets(sendbuf,MAXLINE,in) != NULL)
	{
		Sendto(serfd,sendbuf,strlen(sendbuf),0,seraddr,serlen);
		len = serlen;
		n = Recvfrom(serfd,recvbuf,MAXLINE,0,&replyAddr,&len);
		if(len != serlen || memcmp(&replyAddr,seraddr,len) != 0)
			continue;
		recvbuf[n] = '\0';
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
