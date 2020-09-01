#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#define ADDRESS "192.168.4.112"
#define PORT 5000
static char ABC[10] = "C12";

typedef struct 
{
  char Ant_name[10];
  char system[20];
} ant;
typedef struct
{
  char Ant_name[10];
  char Resp_msg[100];
}ACK;
  
int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    
    ant *e1;
    e1 = malloc(sizeof(ant));
    ACK *a1;
    a1 = malloc(sizeof(ACK));
  /*if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    } */
   // portno = atoi(argv[2]);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        fprintf(stderr,"ERROR opening socket\n");
    server = gethostbyname(ADDRESS);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(PORT);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
          {
            fprintf(stderr,"ERROR connecting socket\n"); 
          }

        n = read(sockfd,e1,sizeof(ant));
        printf(" Size of Struct is ######## %d\n",sizeof(ant));
       if (n < 0) 
         fprintf(stderr,"ERROR reading socket\n"); 
      
          printf("Number of bytes read on socket %d\n",n);
          printf("%s %s\n",e1->Ant_name,e1->system);
          if(!strcmp(ABC,e1->Ant_name))
            {
               fprintf(stderr,"#########===> ABC matches =====>\n");
             //  strcpy(a1->Ant_name,"C00");
               sprintf(a1->Resp_msg," ACK from %s Antenna",ABC);
               write(sockfd,a1,sizeof(ACK));
            }
           else
             {
               fprintf(stderr,"#########===> ABC dose not matches =====>\n");
               strcpy(a1->Ant_name,e1->Ant_name);
               sprintf(a1->Resp_msg,"ABC software is for ##### => %s => Antenna\n",ABC);
             //  strcpy(a1->Resp_msg," ABC software is for above Antenna");
               write(sockfd,a1,sizeof(ACK)); 
             }
     
    close(sockfd);
    return 0;
}
