#include "types.h"
#include "mmu.h"
#include "param.h"
#include "user.h"
#include "proc.h"
#include "fcntl.h"


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


int main(void){
	struct proc * p = malloc(sizeof(struct proc));
	struct trapframe * tf = malloc(sizeof(struct trapframe));

	int pf = open("proc.bin", O_RDONLY);
	read(pf, p, sizeof(struct proc));
	read(pf, tf, sizeof(struct trapframe));
	close(pf);
	
	p->tf = tf;
	
	int pid = fork();
	if(pid == 0){
		loadproc(p, "mem.bin");
		printf(1,"child finished\n");
		exit();
	}
	else {
		printf(1, "parent pid: %d\n", getpid());
		wait();
		exit();
	}
}

