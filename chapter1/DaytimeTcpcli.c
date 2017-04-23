#include "unp.h"


int main(int argc, char *argv[])
{
	if(argc < 2 )
	{
		err_quit("usage: a.out IPaddress");	
	}

	struct sockaddr_in servaddr;
	int sockfd, n;
	char readLine[MAXLINE+1];
	struct sockaddr_in cliaddr;
	sockfd = socket(AF_INET,SOCK_STREAM,NULL);
	if(sockfd  < 0)
	{
		err_sys("socket error");	
	}
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(9999);
	
	if(inet_pton(AF_INET,argv[1],&servaddr.sin_addr) <= 0)
	{
		err_quit("inet_pton %s failed",argv[1]);	
	}

	if(connect(sockfd,(SA *)&servaddr,sizeof(struct sockaddr_in)) < 0)
	{
		err_sys("connect failed");	
	}

	int len = sizeof(cliaddr);
	Getsockname(sockfd,(SA *)&cliaddr,&len);
	printf("local addr:%s\n",Sock_ntop((SA*)&cliaddr,sizeof(cliaddr)));
	printf("server addr:%s\n",Sock_ntop((SA*)&servaddr,sizeof(servaddr)));
	int c = 0;
	while((n = read(sockfd,readLine,MAXLINE)) > 0)
	{
		c++;
		readLine[n] = 0;
		if(fputs(readLine,stdout) == EOF)
		{
			err_sys("fputs error");	
		}
	}

	if(n < 0)
	{
		err_sys("read failed");	
	}
	printf("times = %d \n", c);
	//disconnect
	exit(0);

}
