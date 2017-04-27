#include "unp.h"

void str_cli(FILE *fp,int sersock)
{
	char sendbuf[MAXLINE],recvbuf[MAXLINE];
	while(Fgets(sendbuf,MAXLINE,fp) != NULL){
		Writen(sersock,sendbuf,strlen(sendbuf));
		
		if(Readline(sersock,recvbuf,MAXLINE) == 0){
			err_quit("str_cli:server terminate");
		}	
		Fputs(recvbuf,stdout);
	}
}


int main(int argc,char *argv[])
{
	if(argc < 2)
		err_quit("usage cli IPAddress");
	
	struct sockaddr_in servsock;
	int sock[5];
	int i;
	for(i = 0 ; i < 5 ;++i)
	{
		sock[i] = Socket(AF_INET,SOCK_STREAM,0);

		bzero(&servsock,sizeof(servsock));

		servsock.sin_family = AF_INET;
		servsock.sin_port = htons(SERV_PORT);
		Inet_pton(AF_INET,argv[1],&servsock.sin_addr);

		Connect(sock[i],(SA*)&servsock,sizeof(servsock));
	}
	str_cli(stdin,sock[0]);
	exit(0);
}
