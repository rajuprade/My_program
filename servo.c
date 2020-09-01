#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<time.h>
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
#include<fcntl.h>
#include <unistd.h>
#define EINTR -1
static unsigned char *write_ptr ;
#define PORT 5500
enum { MaxProtBuf1 = 1638 };



int so_block(int sockfd,int flag)
{ int flg;
     flg = fcntl(sockfd, F_GETFL, &flg);
     if (flg < 0) return flg;
     if (flag){ flg |= O_NDELAY; }
     else { flg &= ~O_NDELAY; }
     return fcntl(sockfd, F_SETFL, flg);
}

ssize_t writen(int fd, void *vptr, ssize_t nbytes)
{
  ssize_t l, nwritten, n = 0;
  extern unsigned char  *write_ptr ;
  int errno;

  write_ptr = ( unsigned char  *) vptr;
  while ( n < nbytes) {

     if (nbytes - n > MaxProtBuf1) l = MaxProtBuf1; else l = nbytes - n;

       so_block(fd,0); 
        nwritten = write(fd,write_ptr+n,l);
    // nwritten = send(fd,write_ptr+n,l, 0);
 //     ioctl (fd, I_FLUSH, FLUSHW); /* Flush the data to Socket */
       so_block(fd,1);

    if (nwritten <= 0) {
      if (errno == EINTR)
        nwritten = 0;
      else
        return -1;
    }
    n+=nwritten;
  }
  return (n);
}

typedef enum {COLDSTART,TRACK,HOLD,POSITION,STOP,CLOSE,STOW,STOWRELEASE,ABORT,RESETHW,
                 READANGLES,READANAVAR,READDIGVAR,READSETPARA,READANTSTATUS,READVERSION,SEND} SERVOCMD;
static char *servocmd[]={"coldstart","track","hold","position","stop","close","stow","stow_release","abort","resethw",
                          "readangles","readanavar","readdigvar","readsetpara","readantstatus","readversion"};
#pragma pack(push,16)
typedef struct 
{
  int seq;
  char timestamp[64];
  char system_name[16]; // Not required in servo communication
 // char cmd_code[4];
  char op_name[16];
  short int number_param;
  char para_name[32][16];
  char para_value[32][16];
 // // Not required in servo communication
 } cmd;

typedef struct
{ 
 int seq;
 char timestamp[64];
 char system_name[16];
 int response_code; // immediate =1 ( ACK or NAK ) event=3 & final =2  data mon = 4
 int response_type;// success =1 or failure = 2
 char response_msg[50];  //Accepted ,not accepted, sysntax error, irrelvant command,incomplete,unknown + Event
 short int num_resp_msg;
  char para_name[32][32];
 char para_value[32][32]; 
} resp;


typedef struct
{
  char antenna_name[10];
  cmd  CMD;
} ANT_CMD;

#pragma pack(pop)


void printcmd(cmd *c1)
{
   int i;
        printf("we wrote on the socket %d %s %s %s\n",c1->seq,c1->timestamp,c1->system_name,c1->op_name);
       
        for(i=0;i<c1->number_param;i++)
        {
          printf("%s %s\n",c1->para_name[i],c1->para_value[i]);
        } 
        printf(" Size of Struct is ######## %d\n",sizeof(cmd));
     
}


void printresp(resp *r1)
{
   int i;
   fprintf(stderr," Size of Response Struct is ######## %d\n",sizeof(resp));
   fprintf(stderr, "num_resp_msg : %d\n", r1->num_resp_msg);
   if(r1->response_code == 1 )
     fprintf(stderr,"%s\n",r1->response_msg);
    else if(r1->response_code == 2 && r1->response_type == 1)
             fprintf(stderr,"%s\n",r1->response_msg);
    else if(r1->response_code == 2 && r1->response_type == 2)
            fprintf(stderr,"%s\n",r1->response_msg);
    else if(r1->response_code == 3)
        fprintf(stderr,"r1->response_msg : %s\n",r1->response_msg);
    else if(r1->response_code == 4)
          {
            for(i=0; i<r1->num_resp_msg;i++)
            {
              fprintf(stderr,"%s\n",r1->para_name[i]);
              fprintf(stderr,"%s\n",r1->para_value[i]);
            }
           }
   else
    fprintf(stderr,"No Response from Servo system\n");

}
void servo(int cmd,ANT_CMD *a1)
{
   /* ANT_CMD *a1;
   a1= malloc(sizeof(ANT_CMD)); */
      switch(cmd)
    {
      case COLDSTART: a1->CMD.seq= 10;
                      strcpy(a1->CMD.op_name,"coldstart");    // Command ID 40
                     // sprintf(a1->CMD.cmd_code,"%s","0x40"); 
                      a1->CMD.number_param=0;
                      fprintf(stderr,"%s\n",a1->CMD.op_name);
                       break;
      case TRACK:  a1->CMD.seq= 11;
                   strcpy(a1->CMD.op_name,"track"); 
                  //  sprintf(a1->CMD.cmd_code,"%s","0x44");// Command ID 44
                   a1->CMD.number_param=4;
                   strcpy(a1->CMD.para_name[0],"ax");
                   strcpy(a1->CMD.para_value[0],"B");
                  
                   strcpy(a1->CMD.para_name[1],"time"); // Duration for tracking the source
                   strcpy(a1->CMD.para_value[1],"12:00:00");
                  
                   strcpy(a1->CMD.para_name[2],"ang1");
                   strcpy(a1->CMD.para_value[2],"90:00:00");
                   strcpy(a1->CMD.para_name[3],"ang2");
                   strcpy(a1->CMD.para_value[3],"45:00:00");
                   fprintf(stderr,"###### %s\n",a1->CMD.op_name); 
                    break;
      case HOLD:  a1->CMD.seq= 12;
                  strcpy(a1->CMD.op_name,"hold"); // Command ID 46
                 // sprintf(a1->CMD.cmd_code,"%s","0x46");
                  a1->CMD.number_param=1;
                  strcpy(a1->CMD.para_name[0],"ax");
                  strcpy(a1->CMD.para_value[0],"B");
                  fprintf(stderr,"%s\n",a1->CMD.op_name); 
                  break;
      case POSITION: a1->CMD.seq= 13;
                     strcpy(a1->CMD.op_name,"position"); // Command ID 42
                  //  sprintf(a1->CMD.cmd_code,"%s","0x42");
                     a1->CMD.number_param=2;
                     strcpy(a1->CMD.para_name[0],"ax");
                     strcpy(a1->CMD.para_value[0],"B");
                   
                     strcpy(a1->CMD.para_name[1],"ang1");
                     strcpy(a1->CMD.para_value[1],"90:00:00");
                   
                     strcpy(a1->CMD.para_name[2],"ang2");
                     strcpy(a1->CMD.para_value[2],"45:00:00");
                     fprintf(stderr,"%s\n",a1->CMD.op_name); 
                      break;
       case STOP:  a1->CMD.seq= 14;
                  strcpy(a1->CMD.op_name,"stop");          // Command ID 48
                 // sprintf(a1->CMD.cmd_code,"%s","0x48");
                  a1->CMD.number_param=1;
                  strcpy(a1->CMD.para_name[0],"ax");
                   strcpy(a1->CMD.para_value[0],"B");
                   fprintf(stderr,"%s\n",a1->CMD.op_name);
                  break;  
       case CLOSE:  a1->CMD.seq= 15;
                   strcpy(a1->CMD.op_name,"close");   // Command ID 4a
                  //sprintf(a1->CMD.cmd_code,"%s","0x4a");
                   a1->CMD.number_param=0;fprintf(stderr,"%s\n",a1->CMD.op_name);
                   break; 
       case STOW:  a1->CMD.seq= 16;
                  strcpy(a1->CMD.op_name,"stow");   // Command ID 4c
                //  sprintf(a1->CMD.cmd_code,"%s","0x4c");
                  a1->CMD.number_param=1;
                  strcpy(a1->CMD.para_name[0],"ax");
                   strcpy(a1->CMD.para_value[0],"B");
                   fprintf(stderr,"%s\n",a1->CMD.op_name);
                  break;
      case STOWRELEASE:  a1->CMD.seq= 17;
                  strcpy(a1->CMD.op_name,"stow_release"); // Command ID 4e
                 // sprintf(a1->CMD.cmd_code,"%s","0x4e");
                  a1->CMD.number_param=1;
                  strcpy(a1->CMD.para_name[0],"ax");
                  strcpy(a1->CMD.para_value[0],"B");
                  fprintf(stderr,"%s\n",a1->CMD.op_name);
                  break;
      case ABORT:  a1->CMD.seq= 18;
                   strcpy(a1->CMD.op_name,"abort");// Command ID 50
                  // sprintf(a1->CMD.cmd_code,"%s","0x50");
                   a1->CMD.number_param=0;fprintf(stderr,"%s\n",a1->CMD.op_name);
                   break; 
         
      case RESETHW: a1->CMD.seq= 19;
                   strcpy(a1->CMD.op_name,"resethw");// Command ID 6e
                  // sprintf(a1->CMD.cmd_code,"%s","0x6e"); 
                   a1->CMD.number_param=0; fprintf(stderr,"%s\n",a1->CMD.op_name);
                   break; 
      case READANGLES:a1->CMD.seq= 20;
                   strcpy(a1->CMD.op_name,"readangles"); // command ID 30 in present Online
                //   sprintf(a1->CMD.cmd_code,"%s","0x30");
                   a1->CMD.number_param=0;
                   fprintf(stderr,"%s\n",a1->CMD.op_name);
                    break;
      case READANAVAR:a1->CMD.seq= 21;
                   strcpy(a1->CMD.op_name,"readanavar"); // Command ID 32 
                   //sprintf(a1->CMD.cmd_code,"%s","0x32");
                   a1->CMD.number_param=0; 
                   fprintf(stderr,"%s\n",a1->CMD.op_name);
                    break;
      case READDIGVAR:a1->CMD.seq= 22;
                   strcpy(a1->CMD.op_name,"readdigvar"); // Command ID 34 
                 //  sprintf(a1->CMD.cmd_code,"%s","0x34");
                   a1->CMD.number_param=0; 
                   fprintf(stderr,"%s\n",a1->CMD.op_name);
                    break;
     case READSETPARA:a1->CMD.seq= 23;
                   strcpy(a1->CMD.op_name,"readsetpara"); // Command ID 36 
                   //sprintf(a1->CMD.cmd_code,"%s","0x36");
                   a1->CMD.number_param=0; 
                   fprintf(stderr,"%s\n",a1->CMD.op_name);
                    break;
     case READANTSTATUS:a1->CMD.seq= 24;
                   strcpy(a1->CMD.op_name,"readantstatus"); // Command ID 38
                 //  sprintf(a1->CMD.cmd_code,"%s","0x38");
                   a1->CMD.number_param=0; 
                   fprintf(stderr,"%s\n",a1->CMD.op_name);
                    break;
     case READVERSION:a1->CMD.seq= 25;
                   strcpy(a1->CMD.op_name,"readversion"); // Command ID 3a 
                 //  sprintf(a1->CMD.cmd_code,"%s","0x3a") ;
                   a1->CMD.number_param=0; 
                   fprintf(stderr,"%s\n",a1->CMD.op_name);
                    break;
 /*   // Present Online Sub array Controller commands.
    case SENDSACSRC: //send source coords to subarray controller modified to send trk/posn,TMAX
    case TRKSACSRC : //request subarray controller to track source
    case STPSACTRK : // request SAC to stop tracking source
 
    case GOSACOUT :  //    request SAC to track on outer track
    case GOSACINN :  //   request SAC to track on inner track

    case TRKELOFF :   // track elevation offset by IPA 
                      // Close previous track command issue new track command.
                      // Set Track parameter 
    case TRKAZOFF :   // track azimuth offset by IPA
                       // Close previous track command issue new track command. // Set New Track parameter 
    case TRKANTOFF:   // track azimuth, elevation offset by JPA,IPA
                      // Close previous track command issue new track command. // Set New Track parameter 
    case TRKRAOFF :   // track right ascension offset by IPA
                      // Close previous track command issue new track command. // Set New Track parameter 
    case TRKDECOFF:   // track declination offset by IPA
                      // Close previous track command issue new track command. // Set New Track parameter 
    case SCANELSRC :  // scan src in el with derv=ipa,ptime=jpa
                       // Close previous track command issue new track command. // Set New Track parameter 
    case SCANAZSRC :  // scan src in az with derv=ipa,ptime=jpa
                       // Close previous track command issue new track command. // Set New  Track parameter 
    case SCANRASRC :  // scan src in ra with derv=ipa,ptime=jpa
                      // Close previous track command issue new track command. // Set New Track parameter 
    case SCANDEC :    //scan src in dec with derv=ipa,ptime=jpa 
                     // Close previous track command issue new track command. // Set New Track parameter 
    
// Commands in TELESET 
   
    1. sv trkset trkset.dat // Azimuth offset, Elevation offset, Antenna longitude,Antenna Latitude
    2. sv trkaz in // Set Azimuth out or in
    3. sv trkrate // Enter Ra rate, DEc rate & Time reference t0
    4. sv trkon track.dat // track.dat => Source name, Right ascension,Declinasion
    5. sv trkoff // Stop tracking 

*/
    }
     
}

int main()
{
	int j;
     ANT_CMD *a1;
     a1= malloc(sizeof(ANT_CMD));

     resp *r1;
     r1 = malloc(sizeof(resp));
     
     cmd *c1;
     c1 = malloc(sizeof(cmd));
     
      char cmd1[20], buff[20];
      int sofd,i,sfd1,yes=1,n;
     socklen_t clilen;
     struct sockaddr_in serv_addr, cli_addr;
     time_t t1;
      char stamp1[26];
      time(&t1);
      ctime_r(&t1,stamp1);
      strcpy(a1->CMD.timestamp,stamp1);
      
     sofd = socket(AF_INET, SOCK_STREAM, 0);
     if (sofd < 0) 
       fprintf(stderr,"ERROR opening socket\n");
    if(setsockopt(sofd, SOL_SOCKET , SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
          fprintf(stderr," SOCKET REUSEADDR Error\n");
    } 
     bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(PORT);

     if (bind(sofd, (struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
              { 
               fprintf(stderr,"ERROR binding socket\n");
              }

     listen(sofd,5);

     clilen = sizeof(cli_addr);
   fprintf(stderr,"Size of Response structure %d\n",sizeof(resp));
        
   fprintf(stderr,"### SERVER WANTING FOR GUI CLIENT TO CONNECT #####\n");
   sfd1 = accept(sofd,(struct sockaddr *) &cli_addr,&clilen);
   fprintf(stderr,"#### Client=> %s Port %d\n",inet_ntoa(cli_addr.sin_addr),ntohs(cli_addr.sin_port));

  while(1)
  {
      
      fprintf(stderr,"Enter Servo Command\n");
      scanf("%s",cmd1);
      for (i = 0; i < SEND; i++) 
         {
           if (!strcasecmp(cmd1, servocmd[i]))
            {
             fprintf(stderr,"####### STRING is %s %d\n",cmd1,i);
              strcpy(a1->CMD.system_name,"servo");
             // strcpy(a1->CMD.op_name,cmd1);
              servo(i,a1);
              c1= &a1->CMD;
             // fprintf(stderr,"%s\n",c1->op_name);
  
            //  fprintf(stderr,"%s bytes\n",sizeof(a1));
              //fprintf(stderr,"%s bytes\n",sizeof(cmd));

               write(sfd1,c1,sizeof(cmd));
               printcmd(c1);
             //  fprintf(stderr,"COmmand sent %s\n",c1.op_name);
               /* if((n= writen(sfd1,(unsigned char *)c1,sizeof(cmd)))!=sizeof(cmd))
                                     {  fprintf(stderr," Sending CMD to Servo device\n");}
                                       else
                                      { 
                                         printcmd(c1); bzero(c1,sizeof(cmd));
                                         //bzero(c1,sizeof(cmd));
                                      }*/
                                  
              sleep(1);
              
              read(sfd1,r1,sizeof(resp));// Immediate 
              //fprintf(stderr,"Response from Servo ### %s\n",buff)
               { fprintf(stderr, "r1->seq : %d\n", r1->seq);                   
                 fprintf(stderr, "r1->timestamp : %s\n", r1->timestamp);       
                 fprintf(stderr, "r1->system_name : %s\n", r1->system_name);   
                 fprintf(stderr, "r1->response_code : %d\n", r1->response_code);
                 fprintf(stderr, "r1->response_type : %d\n", r1->response_type);
                 fprintf(stderr, "r1->response_msg : %s\n", r1->response_msg);
                 fprintf(stderr, "r1->num_resp_msg : %d\n", r1->num_resp_msg);  }
              // printresp(r1);
              sleep(1);
              read(sfd1,r1,sizeof(resp)); 
				  fprintf(stderr, "r1->seq : %d\n", r1->seq);                                                                        
                  fprintf(stderr, "r1->timestamp : %s\n", r1->timestamp);                                                            
                  fprintf(stderr, "r1->system_name : %s\n", r1->system_name);                                                        
                  fprintf(stderr, "r1->response_code : %d\n", r1->response_code);                                                    
                  fprintf(stderr, "r1->response_type : %d\n", r1->response_type);                                                    
                  fprintf(stderr, "r1->response_msg : %s\n", r1->response_msg);
                  fprintf(stderr, "r1->num_resp_msg : %d\n", r1->num_resp_msg);
                  for (j=0; j<=r1->num_resp_msg; j++)
						fprintf(stderr, " %s : %s\n", r1->para_name[j], r1->para_value[j]);
 
               //printresp(r1); 
              //fprintf(stderr,"Response from Servo ### %s\n",buff);
              //close(sfd1);
            }
         }
  }
  return 0;    
  
}

