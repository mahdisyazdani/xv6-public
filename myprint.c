#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "mmu.h"
#include "param.h"
#include "proc.h"
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
      struct proc * p = malloc(sizeof(struct proc));
	p->name[0] = 'm';
	p->name[1] = 'a';
	p->name[2] = 's';
	p->name[3] = 0;
      processstate(p);
      printf(1, p->name);
      int fd = open("output", O_CREATE|O_RDWR);
      write(fd, p, sizeof(struct proc));
      close(fd);
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
