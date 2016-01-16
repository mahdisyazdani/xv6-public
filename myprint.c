#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "mmu.h"
#include "param.h"
#include "proc.h"

struct trapframe {
  // registers as pushed by pusha
  uint edi;
  uint esi;
  uint ebp;
  uint oesp;      // useless & ignored
  uint ebx;
  uint edx;
  uint ecx;
  uint eax;

  // rest of trap frame
  ushort gs;
  ushort padding1;
  ushort fs;
  ushort padding2;
  ushort es;
  ushort padding3;
  ushort ds;
  ushort padding4;
  uint trapno;

  // below here defined by x86 hardware
  uint err;
  uint eip;
  ushort cs;
  ushort padding5;
  uint eflags;

  // below here only when crossing rings, such as from user to kernel
  uint esp;
  ushort ss;
  ushort padding6;
};

int
main(void)
{
	int i = 1 ;
	while(1){
		printf(1,"%d\n", i);
		sleep(10);
		if(i == 5){
			// this var holds proc struct
			struct proc * p = malloc(sizeof(struct proc));

			// this function returns the proc struct (which has the address of the trapframe)
			processstate(p);
			int size = p->sz;
		
			// this is the memory (page aligned) used by the process
			void * mem = malloc(size);

			memorydump(mem, size);

			// this file holds the proc and trapframe structs
			int fd = open("proc.bin", O_CREATE|O_RDWR);
			write(fd, p, sizeof(struct proc));
			write(fd, p->tf, sizeof(struct trapframe));
			printf("continuing...");
			close(fd);

			// this one has the memory dump
			int fm = open("mem.bin", O_CREATE|O_RDWR);
			write(fm, mem, size);
			close(fm);

			exit();
		}
		i++;
	}
	exit();
	return 0;
}

