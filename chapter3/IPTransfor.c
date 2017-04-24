#include "unp.h"


int main(void )
{
	char *ip = "127.0.0.1";
	struct sockaddr_in sa;

	bzero(&sa,sizeof(sa));
		
	inet_aton(ip,&sa.sin_addr);
	
	if(sa.sin_addr.s_addr == inet_addr(ip))
	{
		printf("sin_addr = %ld \n",sa.sin_addr);	
	}
	
	printf("%s\n",inet_ntoa(sa.sin_addr));

	
	struct sockaddr_in sa2;
	char buf[INET_ADDRSTRLEN];
	inet_pton(AF_INET,"127.0.0.1",&sa2.sin_addr);
	inet_ntop(AF_INET,&sa2.sin_addr,buf,INET_ADDRSTRLEN);
	printf("%s\n",buf);

	struct sockaddr_in6 s6a;
	char buf6[INET6_ADDRSTRLEN];
	inet_pton(AF_INET6,"0:0:0:1:0:0:0:0",&s6a.sin6_addr);
	inet_ntop(AF_INET6,&s6a.sin6_addr,buf6,INET6_ADDRSTRLEN);
	printf("%s\n",buf6);	


	printf("%s\n",strerror(errno));	
	exit(0);	
}
