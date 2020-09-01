
#include"server_test.h"
#include<sys/ioctl.h>
#include<pthread.h>
pthread_mutex_t mlock = PTHREAD_MUTEX_INITIALIZER;

int ret =0;
char *name;
int main(int argc ,char **argv)
{
   int i=0;
   char *aname;
   aname = malloc(sizeof(10));
   Ant *antenna_data;
   antenna_data = (Ant *)malloc(sizeof(Ant));
   if(init_server()==ERROR)
    {
        fprintf(stderr," ERROR opening Socket\n");
    }
    
    fprintf(stderr,"Enter number of Antenna in One sub array\n");
    scanf("%d",&antenna_data->number_ant);
    
    fprintf(stderr," Enter names of ANtenna\n");
    for(i=0;i<antenna_data->number_ant;i++)
     {
       scanf("%s",antenna_data->antenna_name[i]);
     }
         
        tptr = calloc(antenna_data->number_ant,sizeof(Thread)); // Allocating memory for threads 
        for(i = 0 ; i <antenna_data->number_ant; i++)
         {
           bzero(aname,sizeof(aname));
           strcpy(aname,antenna_data->antenna_name[i]);
         //  fprintf(stderr," Names of ANtenna is %s\n",aname);
            pthread_mutex_lock(&mlock);
            thread_make(i,aname);
            pthread_mutex_unlock(&mlock);
             usleep(10000);
         }
        /* fprintf(stderr,"Enter any command\n");
         scanf("%s",name);
         if(name)
          {ret=1;}
           else{ret=0;} */
       
         for(; ; )
          pause(); /* everything done by threads */
}

void thread_make(int i,char *ant_name)
{  
   int ret;
   void *thread_main(void *);
   // fprintf(stderr,"======> Going to thread for  Anteena  %s #####\n",ant_name);
   pthread_create(&tptr[i].thread_tid ,NULL ,&thread_main,(void *)ant_name);
   
   return; /* main thread returns */
}


void *thread_main(void *arg)
{
   char ant_n[10];
   //strcpy(ant_n,(char *)arg);
  
   fprintf(stderr,"Opening thread for  Anteena  %s #####\n",(char *)arg);
  for(;;)
  {
   if(ret)
   {
      if(!strcasecmp((char*)arg,name))
      {
          fprintf(stderr,"IT is for me #####\n"); }
   }
 }
   bzero(ant_n,10);
   //
}


int init_server(void)
{
      int reuse =1;
      struct linger wrplinger = { 0 , 0 };
      sockfd = socket(AF_INET, SOCK_STREAM, 0);
      if (sockfd < 0) 
      fprintf(stderr,"ERROR opening socket\n");

      if( setsockopt(sockfd, SOL_SOCKET, SO_LINGER, &wrplinger, sizeof(wrplinger)) == ERROR)
           {
              fprintf(stderr,"setsockopt-SO_LINGER"); 
              return ERROR; 
           } 

      if( setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == ERROR)
           { 
               fprintf(stderr,"setsockopt-SO_REUSEADDR");
               return ERROR; 
           }
     if( setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &reuse, sizeof(reuse)) == ERROR)
             { 
               fprintf(stderr,"setsockopt-SO_KEEPALIVE");
               return ERROR; 
            }
  
     if(setsockopt(sockfd, IPPROTO_TCP,TCP_NODELAY, (char *)&reuse,sizeof(reuse)) == ERROR)
       { 
          fprintf(stderr,"setsockopt_TCP_NODELAY");
          return ERROR;
       }   

       bzero((char *) &serv_addr, sizeof(serv_addr));
  
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(PORT);

     if (bind(sockfd, (struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
              { 
               fprintf(stderr,"ERROR binding socket\n");
              }

     listen(sockfd,5);
     return 0;
}
