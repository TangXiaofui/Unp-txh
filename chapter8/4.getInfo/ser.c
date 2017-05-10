#include "unp.h"
#include <sys/socket.h>
#include <sys/types.h>

static void recvfrom_int(int);
static int count = 0;

void dg_echo(int sersock,SA *cliaddr, socklen_t clilen)
{
	socklen_t len;
	char mesg[MAXLINE] = {0};
	Signal(SIGINT,recvfrom_int);
	//int n = 220 * 1024;
	//Setsocketopt(sersock,SOL_SOCKET,SO_RECVBUF,&n,sizeof(n));
	for(;;)
	{
		len = clilen;
		Recvfrom(sersock,mesg,MAXLINE,0,cliaddr,&len);
		count ++;
	}
}

static void recvfrom_int(int signo)
{
	printf("receive %d datagrams\n",count);
	exit(0);
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
