#include "unp.h"
#include <limits.h>
#define OPEN_MAX 1024

int main(int argc,char *argv[])
{
	int i,maxi,listenfd,sockfd,connfd;
	struct sockaddr_in sersock,clisock;
	socklen_t clilen;
	
	struct pollfd client[OPEN_MAX];
	ssize_t n;
	char buf[MAXLINE] = {0};
	int nready;

	listenfd = Socket(AF_INET,SOCK_STREAM,0);
	
	bzero(&sersock,sizeof(sersock));
	sersock.sin_family = AF_INET;
	sersock.sin_port = htons(SERV_PORT);
	sersock.sin_addr.s_addr = htonl(INADDR_ANY);
	
	Bind(listenfd,(SA *)&sersock,sizeof(sersock));	

	Listen(listenfd,LISTENQ);

	client[0].fd = listenfd;
	client[0].events = POLLRDNORM;
	for(i = 1 ; i < OPEN_MAX;++i){
		client[i].fd = -1;	
	}
	maxi = 0;

	for(;;){
		nready = Poll(client,maxi+1,INFTIM);
		if(client[0].revents & POLLRDNORM){
			clilen = sizeof(clisock);
			connfd = Accept(listenfd,(SA*)&clisock,&clilen);
			for(i = 1 ; i < OPEN_MAX ;++i ){
				if(client[i].fd < 0){
					client[i].fd = connfd;
					break;	
				}
			}
			if(i == OPEN_MAX)
				err_quit("too many client");
			client[i].events = POLLRDNORM;
			if(i > maxi)
				maxi = i;
			if(--nready == 0)
				continue;
		}	

		for(i = 0 ; i <= maxi ; ++i){
			if((sockfd = client[i].fd) < 0)
				continue;
			if(client[i].revents & (POLLRDNORM|POLLERR)){
				if( (n = Read(sockfd,buf,MAXLINE)) < 0){
					if(errno == ECONNRESET){
						Close(sockfd);
						client[i].fd = -1;
					}
					else{
						err_sys("read error");
					}
				}
				else if(n == 0){
					Close(sockfd);
					client[i].fd = -1;
				}
				else{
					Writen(sockfd,buf,n);	
				}
				if(--nready == 0)
					break;
			}
		}
	}
}
