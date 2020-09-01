#include<stdio.h>
typedef struct 
{ 
  int seq; 
  char timestamp[64]; 
  char system_name[16]; // Not required in servo communication 
  char cmd_code[4]; 
  char op_name[16]; 
  short int number_param; 
  char para_name[32][16]; 
  char para_value[32][16]; 
} servocmd; 

 typedef struct 
 { 
   int seq; 
   char timestamp[64]; 
   char system_name[16]; 
   int response_code;     // immediate =1 ( ACK or NAK ) event=3 & final =2  data mon = 4 
  int response_type;      // success =1 or failure = 2 
  char response_msg[50];  //Accepted ,not accepted, sysntax error, irrelvant         							command,incomplete,unknown + Event 
  short int num_resp_msg; 
  char para_name[32][32]; 
  char para_value[32][32];
} servoresp; 

int main()
{
  fprintf(stderr,"%d %d\n",sizeof(servocmd),sizeof(servoresp));
  return 0;
}
