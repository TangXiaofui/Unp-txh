#include "unp.h"

int main(int argc,char* argv[])
{
	int sersock;
	struct sockaddr_in seraddr,cliaddr;

	if(argc < 2)
		err_sys("usage: cli <IPAddress>");
	
	sersock = Socket(AF_INET,SOCK_DGRAM,0);
	bzero(&seraddr,sizeof(seraddr));
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(SERV_PORT);
	Inet_pton(AF_INET,argv[1],&seraddr.sin_addr);

	Connect(sersock,(SA*)&seraddr,sizeof(seraddr));

	socklen_t	len = sizeof(cliaddr);
	Getsockname(sersock,(SA*)&cliaddr,&len);
	printf("local address %s\n",Sock_ntop((SA*)&cliaddr,len));
	exit(0);
}
