#include "unp.h"

int main(int argc,char *argv[])
{
	if(argc < 2)
	{
		err_quit("usage: a.out IPAddress");
	}

	struct sockaddr_in6 sersock;
	int n,sockfd;
	char readLine[MAXLINE];

	if((sockfd = socket(AF_INET6,SOCK_STREAM,0)) < 0)
	{
		err_sys("socket failed");	
	}

	sersock.sin6_family = AF_INET6;
	sersock.sin6_port = htons(13);
	
	if(inet_pton(AF_INET6,argv[1],&sersock.sin6_addr) < 0)
	{
		err_sys("inet_pton failed");
	}

	if(connect(sockfd, (SA*)&sersock, sizeof(struct sockaddr_in6)) < 0){
		err_sys("connect failed");	
	}

	while((n = read(sockfd,readLine,MAXLINE)) > 0)
	{
		readLine[n] = 0;
		if(fputs(readLine,stdout) == EOF)
		{
			err_sys("fputs fail");
		}
	}
	
	exit(0);
}
