#include "unp.h"


int main(int argc,char *argv[])
{
	int listenfd;
	struct sockaddr_in sersock;
	fd_set rset,allset;
	
	int nready,client[FD_SETSIZE];
	int maxi = -1,maxfd,i;
	struct sockaddr_in clisock;
	int clilen,connfd,sock;
	char buf[MAXLINE] = {0};
	size_t n;

	listenfd = Socket(AF_INET,SOCK_STREAM,0);
	
	bzero(&sersock,sizeof(sersock));
	sersock.sin_family = AF_INET;
	sersock.sin_addr.s_addr = htonl(INADDR_ANY);
	sersock.sin_port = htons(9879);
	
	Bind(listenfd,(SA*)&sersock,sizeof(sersock));
	Listen(listenfd,LISTENQ);

	maxfd = listenfd;
	for(i = 0 ; i < FD_SETSIZE ; ++i)
	{
		client[i] = -1;	
	}

	FD_ZERO(&allset);
	FD_SET(listenfd,&allset);
	

	for(;;)
	{
		rset = allset;
		nready = Select(maxfd+1,&rset,NULL,NULL,NULL);
		if(FD_ISSET(listenfd,&rset)){
			clilen = sizeof(clisock);
			connfd = Accept(listenfd,(SA*)&clisock,(socklen_t*)&clilen);
			
			for(i = 0 ; i < FD_SETSIZE ; ++i){
				if(client[i] < 0)
				{
					client[i] = connfd;
					break;
				}
			}
			if(i == FD_SETSIZE)
				err_sys("too many client");
			FD_SET(connfd,&allset);
			if(i > maxi)
				maxi = i;
			if(connfd > maxfd)
				maxfd = connfd;
			if(--nready == 0)
				continue;
		}
		for(i = 0 ; i <= maxi ; ++i ){
			if((sock = client[i]) < 0)
				continue;
			if(FD_ISSET(sock,&rset)){
				if( (n = Read(sock,buf,MAXLINE)) == 0){
					Close(sock);
					client[i] = -1;
					FD_CLR(sock,&allset);
				}
				else{
					Write(sock,buf,n);
				}
				if( --nready == 0)
					break;
			}
		}
	}
		
}
