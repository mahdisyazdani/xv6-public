#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
//mahdis

int
main(void)
{
  int x = 1;
  int i = 0 ;
  while(x == 1){
    printf(1,"add");
    sleep(10);
    if(i == 5){
      myprint();
      exit();
    }
    i++;
  }
 return 0;
}
 /*     
  int fd;
  fd = open("mahdis", O_CREATE|O_RDWR);
if(fd >= 0){
    cprintf("creat myexample succeeded; ok\n");
  } else {
    cprintf("error: creat myexample failed!\n");
    exit();
  }
  for(i = 0; i < 16; i++){
    if(write(fd, &saveproc->name, 1) != 1){
      cprintf("error: write aa new file failed\n");
      exit();
    }
    
  }
  cprintf("writes ok");
  close(fd);*/
