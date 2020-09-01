/* A simple server in the internet domain using TCP
   demonstrating how to send structure over sockets ******/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include<pthread.h>
#include<errno.h>
#include<time.h>
#include<malloc.h>
#include <math.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include<sys/ioctl.h>
static unsigned char  *read_ptr ;
#define PORT 7000
enum { MaxProtBuf = 1652 };

#pragma pack(push,1)
typedef struct 
{
  int seq;
  char timestamp[64];
  char system_name[16];
  char op_name[16];
  short int number_param;
  char parameter_name[32][16];
  char Argument_Ch1[32][16];
  char Argument_Ch2[32][16];
 } cmd;

typedef struct
{
  char antenna_name[10];
  cmd  CMD;
} ANT_CMD;
#pragma pack(pop)
typedef struct
{ 
  char ant_name[20];
  char sys_name[20];
} ACK;

ssize_t  readn(int fd, void *vptr, ssize_t maxlen);
void printcmd(cmd *c1);
void printcmd(cmd *c1)
{
   int i;
        printf("we Recv %d %s %s %s\n",c1->seq,c1->timestamp,c1->system_name,c1->op_name);
       
        for(i=0;i<c1->number_param;i++)
        {
          printf("%s %s %s\n",c1->parameter_name[i],c1->Argument_Ch1[i],c1->Argument_Ch2[i]);
        } 
        //printf(" Size of Struct is ######## %d\n",sizeof(cmd));
     
}


ssize_t readn(int fd, void *vptr, ssize_t nbytes)
{
  ssize_t nread;
  ssize_t l, n = 0;
  extern unsigned char  *read_ptr ;

  read_ptr = ( unsigned char  *) vptr;
  while (n < nbytes) {
       if (nbytes - n > MaxProtBuf) l = MaxProtBuf; else l = nbytes - n;

   //    nread = read(fd,read_ptr+n,l);

      // so_block(fd,1); 
        nread = recv(fd,read_ptr+n,l,MSG_DONTWAIT);
      // so_block(fd,0); 
     
      // ssize_t n = recv(fd,read_ptr,nbytes,0); 
     //ioctl (fd, I_FLUSH, FLUSHR);  /*Flush the data from Socket  */ 
        fdatasync(fd);

    if (nread < 0) {
      if (errno == EINTR || errno == EWOULDBLOCK) nread = 0;
        return -1;
    } else if (nread == 0) {
      break;
    }
     n+=nread;
  }
  return (n);
}


int main(int argc, char *argv[])
{
     int sockfd, sfd,n,yes=1;
     socklen_t clilen;
     struct sockaddr_in serv_addr, cli_addr;
     int i;

     ANT_CMD *e1;
     e1 = malloc(sizeof(ANT_CMD));
 
      cmd *c;
      c = malloc(sizeof(cmd));
     
      ACK *a1;
      a1 = malloc(sizeof(ACK));

     
   
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
       fprintf(stderr,"ERROR opening socket\n");
    if(setsockopt(sockfd, SOL_SOCKET , SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
          error(" Error\n");
    } 
     bzero((char *) &serv_addr, sizeof(serv_addr));
    // portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(PORT);

     if (bind(sockfd, (struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
              { 
               fprintf(stderr,"ERROR binding socket\n");
              }

     listen(sockfd,5);

     clilen = sizeof(cli_addr);
     
         fprintf(stderr,"### SERVER WANTING FOR GUI CLIENT TO CONNECT #####\n");
     while(1)
     {
       sfd = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);
       fprintf(stderr,"#### Client=> %s Port %d\n",inet_ntoa(cli_addr.sin_addr),ntohs(cli_addr.sin_port));
       
          bzero(e1,sizeof(ANT_CMD));
          n = read(sfd,e1,sizeof(ANT_CMD));
           fprintf(stderr,"Size of Command structure is %d\n",sizeof(ANT_CMD));
           fprintf(stderr,"Antenna Name is %s\n",e1->antenna_name);
           fprintf(stderr,"Operation Name is %s\n",e1->CMD.op_name);
           c = &e1->CMD;
            printcmd(c); 
          /*  while(readn(sfd,e1,sizeof(ANT_CMD))!=sizeof(ANT_CMD));
            fprintf(stderr,"Size of Command structure is %d\n",sizeof(ANT_CMD));
            fprintf(stderr,"Antenna Name is %s\n",e1->antenna_name);
            fprintf(stderr,"Operation Name is %s\n",e1->CMD.op_name);
            c = &e1->CMD;
            printcmd(c);*/
        /*  if(n !=sizeof(ANT_CMD))
          { fprintf(stderr," ERROR READING COMMAND FROM GUI\n"); }
          else
          {
            fprintf(stderr,"ERROR READING socket\n");
            fprintf(stderr,"Size of Command structure is %s\n",sizeof(ANT_CMD));
            fprintf(stderr,"Antenna Name is %s\n",e1->antenna_name);
            fprintf(stderr,"Operation Name is %s\n",e1->CMD.op_name);
            c = &e1->CMD;
            printcmd(c);
           } */ 

         if (sfd < 0) 
            fprintf(stderr,"ERROR Accept socket\n");

           strcpy(a1->ant_name,e1->antenna_name);
           strcpy(a1->sys_name,e1->CMD.system_name);
           n = write(sfd,a1,sizeof(ACK));
        
           if (n < 0)
                fprintf(stderr,"ERROR writing socket\n");
            else
            printf("we wrote on the socket %s %s\n",a1->ant_name,a1->sys_name);
            close(sfd);
      }
        close(sockfd);
     return 0; 
}
