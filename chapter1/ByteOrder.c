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
	exit(0);
}
