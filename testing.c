#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include<string.h>
#include <sys/io.h>       
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <pthread.h>
#include<time.h>
#include<fcntl.h>



int main()
{
   FILE *f;
   int j,k,dwRxSize=46;

   f = fopen("/home/raj/Desktop/FE.txt","a+");
    if(f == NULL)
    fprintf(stderr,"CANNOT OPEN FILE\n");
  for(j=14;j<(dwRxSize);j++)
     {
        k= j-14;
        fprintf(f,"Channel[%d]=>Raw data is 0x%02X\n",k,j);
        fflush(f);
       }

fclose(f);
return 0;
}
