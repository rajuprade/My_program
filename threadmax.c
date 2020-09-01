#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
 
void* thread_proc(void *param)
{
  usleep(10);
  return NULL;
}
 
int main(int argc, char* argv[])
{ 
  int i;
  pthread_t test_thead;
  for(i=1; i<10000; i++){
    int error = pthread_create(&test_thead, NULL, &thread_proc, NULL);
    if (error){
      printf("pthread_create fail in creating thread %d with code: %d\n", i, error);
      break;
    }
  }
  return 0;
}
