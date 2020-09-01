#include <pthread.h>
#include<stdio.h>
int main()
{
pthread_attr_t tattr;
size_t size;
int ret;

/* getting the stack size */
ret = pthread_attr_getstacksize(&tattr, &size);
fprintf(stderr," Returned the Stack size ######## [[[[ %d ]]]] \n",(int)size);
return 0;
}
