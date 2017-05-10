#include "unp.h"
#include <sys/socket.h>
#include <sys/types.h>

void dg_echo(int sersock,SA *cliaddr, socklen_t clilen)
{
	char buf[MAXLINE] = {0};
	ssize_t n;
	socklen_t len;
	for(;;){
		len = clilen;
		n = Recvfrom(sersock,buf,MAXLINE,0,cliaddr,&len);
		Sendto(sersock,buf,n,0,cliaddr,len);	
	}	
}

int main(int argc,char *argv[])
{
	int sersock;
	struct sockaddr_in seraddr,cliaddr;

	bzero(&seraddr,sizeof(seraddr));
	sersock = Socket(AF_INET,SOCK_DGRAM,0);

	seraddr.sin_family = AF_INET;
	seraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	seraddr.sin_port = htons(SERV_PORT);
	
	Bind(sersock,(SA *)&seraddr,sizeof(seraddr));

	dg_echo(sersock,(SA*)&cliaddr,sizeof(cliaddr));
	exit(0);
}
