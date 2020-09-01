/* compile with:   gcc -lpthread -o thread-limit thread-limit.c */
	/* originally from: http://www.volano.com/linuxnotes.html */
	/* 2009.11.01 - added option to set stack size (sru) stefan@whocares.de */
	
	#include <stdio.h>
	#include <unistd.h>
	#include <pthread.h>
	#include <bits/local_lim.h>
	
	#define MAX_THREADS 100000
	int i;
	
	void run(void) {
	  char c;
	  if (i < 10)
	    printf("Address of c = %u KB\n", (unsigned int) &c / 1024);
	  sleep(60 * 60);
           }
	
      int main(int argc, char *argv[]) {
	  int rc = 0;
	  size_t size = 0;
	  int bla;
	  pthread_t thread[MAX_THREADS];
	  pthread_attr_t attr;
	
	  if (argc < 2) {
	        pthread_attr_init(&attr);
	        bla = pthread_attr_getstacksize(&attr, &size);
	        printf("Using system default stack size of %u bytes.\n", size);
	  } else {
	        bla = atoi(argv[1]);
	        if (bla < PTHREAD_STACK_MIN) {
	           size = PTHREAD_STACK_MIN;
	           printf("Too small stack size given, using PTHREAD_STACK_MIN (%u bytes)\n", size);
	        } else {
	           size = bla;
	           printf("Setting thread stack size to %u bytes.\n", size); 
	        }
	  }
	
	  printf("Creating threads ...\n");
	  for (i = 0; i < MAX_THREADS && rc == 0; i++) {
	    pthread_attr_init(&attr);
	    pthread_attr_setstacksize(&attr, size);
	    rc = pthread_create(&(thread[i]), &attr, (void *) &run, NULL);
	    if (rc == 0) {
	      pthread_detach(thread[i]);
	      if ((i + 1) % 100 == 0)
	        printf("%i threads so far ...\n", i + 1);
	    }
	    else
	      printf("Failed with return code %i creating thread %i.\n",
	             rc, i + 1);
	  }
	  return (0);
}
