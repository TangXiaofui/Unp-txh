#include <stdio.h>

typedef void testfunc(int);

typedef void (*test(int t,void (*f)(int)))(int);

void basefunc(int a)
{
	printf("%d\n",a);	
}

testfunc *function(int t,testfunc *tf)
{
	tf(t);
	printf("function\n");
	return tf;
}

int main(void )
{
	test* t = function;
	t(1,basefunc)(2); 
	return 0;	
}
