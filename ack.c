#include<stdio.h>

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
  cmd  CMD[10];
} ANT_CMD;
typedef struct
{
 int response_type;
 int seq;
 char timestamp[64];
 char system_name[16];
 char Mon_raw[64][8];
 char Mon_sum[32][64];
 short int num_resp_msg;
 char response_message[32][64];
} resp;


typedef struct
{
  char Ant_name[10];
  char Resp_msg[100];
}ACK;

#pragma pack(pop)

#define Acksize sizeof(ACK)
#define Antcmd  sizeof(ANT_CMD)

#define Respsize sizeof(resp)
int main()
{
  fprintf(stderr,"SIZE of ACK is %d ==> %d\n ======> RESP size %d\n",Acksize,Antcmd,Respsize);
}
