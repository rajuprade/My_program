/******************************************************************
 * chartest.c
 * C test program for char.c module for MCM Communication
 * requires char module and RS232 / RS485 converter
******************************************************************/

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/io.h>       
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define DEVICE "/dev/mcmdriver"
//#define SERIAL_PORT 0x3f8
#define MCM_ADDR 0x00
#define MAXSIZ 70

/*------------ display function-----------------------------------------*/

void disp_answer(char *buf_ptr)
{
 int i, length;

 length = *(buf_ptr+1) + (*(buf_ptr+2)<<8) + 1; //addr mcm
 printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
 printf("MCM COMMAND SUCCESSFUL\n");
 printf("RESPONSE FROM MCM =====>\n");
 for (i=0; i<length; i++) 
 printf("%x\n",(unsigned char) *(buf_ptr+i));
 printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
 printf("\n");
}

/*------------ checksum function-------------------------------------*/
char cksum(char *buff)
{
 int length, i;
 char CS=0;

 length = *(buff) + (*(buff+1)<<8);
 for(i=0; i<length; i++) CS += *(buff+i);
 return CS;
}

/*--------------Add Check Sum function---------------------------------*/
void add_cksum(char buff[])
{
 int length, i;
 char CS=0;

 length = buff[1] + (buff[2]<<8);
 for(i=1; i<length; i++) CS += *(buff+i);
 CS = ~CS+1;
 buff[length] = CS;
}

/*------------ Write & read function----------------------------------*/
void write_read(char *buff, char *buf_ptr0)
{
  int fd, i;
  long bytes=0;

  buff[0]=*buf_ptr0;
  printf("Mcm addr %d\n", buff[0]);
  add_cksum(buff);  
  fd = open(DEVICE, O_RDWR);
  //perror("OPen"); //1st perror prints: Success, nexts : Illegal Seek ??
  for (i=0;i<1;i++)
  {
    write(fd, buff, 100);
    bytes = read(fd, buf_ptr0, 100);
    printf("%d: read %ld bytes, cs %x\n", i, bytes, cksum(buf_ptr0+1));
    disp_answer(buf_ptr0);
    //sleep(1);
  }
  close(fd);
  //perror("CLose");
}

/*--------------MCM Commands --------------------------------------*/ 
void read_mode(char *buf_ptr0)
{
 char buff[]={0,9,0,0,2,0,1,0,4,0};

 printf("read_mode\n");
 write_read(buff, buf_ptr0);
}

void read_version(char *buf_ptr0)
{
 char buff[]={0,9,0,0,2,0,1,0,3,0};

 printf("read_version\n");
 write_read(buff, buf_ptr0);
}

void read_dmask_32b(char *buf_ptr0)
{
 char buff[]={0,9,0,0,2,0,1,0,2,0};

 printf("read_dmask_32b\n");
 write_read(buff, buf_ptr0);
}

void read_dmask_16b(char *buf_ptr0)
{
 char buff[]={0,9,0,0,2,0,1,0,1,0};

 printf("read_dmask_16b\n");
 write_read(buff, buf_ptr0);
}

void read_an_mask(char *buf_ptr0)
{
 char buff[]={0,9,0,0,2,0,1,0,0,0};

 printf("read_an_mask\n");
 write_read(buff, buf_ptr0);
}

void set_dmask_32b(char *buf_ptr0)
{
 char buff[]={0,0xd,0,0,1,0,5,0,3,0xff,0xff,0xff,0xff,0};

 printf("set_dmask_32b\n");
 write_read(buff, buf_ptr0);
}

void set_dmask_16b(char *buf_ptr0)
{
 char buff[]={0,0xb,0,0,1,0,3,0,2,0xff,0xff,0};

 printf("set_dmask_16b\n");
 write_read(buff, buf_ptr0);
}

void set_an_mask(char *buf_ptr0)
{
 char buff[]={0,0x11,0,0,1,0,9,0,1,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0};

 printf("set_an_mask\n");
 write_read(buff, buf_ptr0);
}

void set_scan_mode(char *buf_ptr0)
{
 char buff[]={0,0xa,0,0,1,0,2,0,0,1,0};

 printf("set_scan_mode\n");
 write_read(buff, buf_ptr0);
}

void set_idle_mode(char *buf_ptr0)
{
 char buff[]={0,0xa,0,0,1,0,2,0,0,0,0};

 printf("set_idle_mode\n");
 write_read(buff, buf_ptr0);
}

void nul_cmd(char *buf_ptr0)
{
 char buff[]={0,8,0,0,0,0,0,0,0};

 printf("nul_cmd\n");
 write_read(buff, buf_ptr0);
}

void reboot(char *buf_ptr0)
{
 char buff[]={0,8,0,0,5,0,0,0,0};

 printf("reboot\n");
 write_read(buff, buf_ptr0);
}                    

/*------------ execute cmd function------------------------------------*/
void exec_cmd(int cmd, char *buf_ptr0)
{                                     
 switch(cmd)
 {
  case 0 : nul_cmd(buf_ptr0);break;
  case 1 : set_idle_mode(buf_ptr0);break;
  case 2 : set_scan_mode(buf_ptr0);break;
  case 3 : set_an_mask(buf_ptr0);break;
  case 4 : set_dmask_16b(buf_ptr0);break;
  case 5 : set_dmask_32b(buf_ptr0);break;
  case 6 : read_an_mask(buf_ptr0);break;
  case 7 : read_dmask_16b(buf_ptr0);break;
  case 8 : read_dmask_32b(buf_ptr0);break;
  case 9 : read_version(buf_ptr0);break;
  case 10 : read_mode(buf_ptr0);break;
  case 11 : reboot(buf_ptr0);break;
  default : break;
 }
}


int main(int argc, char *argv[])
{
 char *buf_ptr0;
 int i,cmd=0;

 buf_ptr0 = malloc(100);
 //ioperm(SERIAL_PORT, 8, 1);
 //perror("\nioperm :");
 
 for (i=0;i<3;i++) {
 printf("\nEnter Mcm Address:\t");
 fflush(stdout);
 scanf("%x", (int *)buf_ptr0);      
 printf("\nEnter Command:\t");
 fflush(stdout);
 scanf("%d", &cmd);      
  
 exec_cmd(cmd, buf_ptr0);
 printf("*****************************************\n");
 }
 free(buf_ptr0);
 return 1;
}



