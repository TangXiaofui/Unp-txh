#include "unp.h"

#define NDG 2000
#define DGLEN 1400

void dg_cli(FILE *in,int serfd,const SA* seraddr,socklen_t serlen)
{
	int i = 0; 
	char buf[DGLEN];
	for(i = 0 ; i < NDG ; ++i)
		Sendto(serfd,buf,DGLEN,0,seraddr,serlen);
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
