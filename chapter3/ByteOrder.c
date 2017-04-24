#include "unp.h"


int main(int argc,char *argv[])
{
	union{
		short s;
		char c[sizeof(short)];
	}un;

	un.s = 0x0102;
	if(sizeof(un) == 2)
	{
		if(un.c[0] == 1 && un.c[1] == 2)
			printf("big\n");
		else if(un.c[1] == 1 && un.c[0] == 2)
			printf("little\n");
		else
			printf("unknow\n");	
	}
	// net big-end
	printf("host Byte = %.4p\n",0x0102);
	printf("htons(0x0102) = %.4p\n",htons(0x0102));
	printf("htonl(0x0102) = %.8p\n",htonl(0x0102));

	printf("ntohs(0x0201) = %.4p\n",ntohs(0x0201));
	printf("ntohs(0x0102) = %.4p\n",ntohs(0x0102));
	printf("ntohl(0x0102) = %.8p\n",ntohl(0x0102));

	exit(0);
}
