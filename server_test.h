#include<sys/types.h>
#include<sys/socket.h>
#include<sys/time.h>
#include<time.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>
#include<fcntl.h>
#include<netdb.h>
#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/uio.h>
#include<unistd.h>
#include<sys/wait.h>
#include <netinet/tcp.h>

#define PORT 6000
#define ERROR -1

#ifdef HAVE_SYS_SYSCTL_H
#include<sys/sysctl.h>
#endif

#ifdef HAVE_SYS_EVENT_H
#include<sys/event.h>
#endif

#ifdef HAVE_SYS_SELECT_H
#include<sys/select.h>
#endif

#ifdef HAVE_POLL_H
#include<poll.h>
#endif

#ifdef HAVE_STRINGS_H
#include<strings.h>
#endif

#ifdef HAVE_PTHREAD_H
#include<pthread.h>
#endif

#ifdef HAVE_SYS_IOCTL_H
#include<sys/ioctl.h>
#endif
typedef struct
{
  int number_ant;
  char antenna_name[10][10];
} Ant;
typedef struct
{
   pthread_t thread_tid; /* thread id */
   long thread_count;    /* connections handled */
} Thread;

Thread *tptr;

int listenfd ,nthreads = 0;
socklen_t addrlen;
pthread_mutex_t mlock;

#define MAXNCLI 32
 int     clifd[MAXNCLI], iget, iput;
 pthread_mutex_t clifd_mutex;
 pthread_cond_t clifd_cond; 

extern int sockfd;
int sockfd;
socklen_t clilen;
struct sockaddr_in serv_addr, cli_addr;

extern int init_server(void);
int init_server(void);
void thread_make(int,char *);

