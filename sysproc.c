#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

/*
#include "my_user.h"
#include "syscall.h"
#include "fcntl.h"

*/
//extern struct proc *proc;
int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;
  
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// Return the address of the PTE in page table pgdir
// that corresponds to virtual address va.  If alloc!=0,
// create any required page table pages.
// (Copied from vm.c)
static pte_t *walkpgdir(pde_t *pgdir, const void *va, int alloc)
{
	pde_t *pde;
	pte_t *pgtab;
	
	pde = &pgdir[PDX(va)];
	if(*pde & PTE_P){
		pgtab = (pte_t*)p2v(PTE_ADDR(*pde));
	} else {
		if(!alloc || (pgtab = (pte_t*)kalloc()) == 0)
			return 0;
		// Make sure all those PTE_P bits are zero.
		memset(pgtab, 0, PGSIZE);
		// The permissions here are overly generous, but they can
		// be further restricted by the permissions in the page table 
		// entries, if necessary.
		*pde = v2p(pgtab) | PTE_P | PTE_W | PTE_U;
	}
	return &pgtab[PTX(va)];
}

// given an address and a size, copies all the 
// user space (used by the process) to save in the file
// (Mostly copied from vm.c -- copyuvm())
static int memory_dump(void *p, uint sz)
{
	pte_t *pte;
	uint pa, i;
	for(i = 0; i < sz; i += PGSIZE){
		if((pte = walkpgdir(proc->pgdir, (void *) i, 0)) == 0)
			panic("checkpoint_mem: pte should exist");
		if(!(*pte & PTE_P))
			panic("checkpoint_mem: page not present");
		pa = PTE_ADDR(*pte);
		memmove(p + i, (char*)p2v(pa), PGSIZE);
		cprintf("write successfuly at %p\n", p + i);
	}
	
	*((struct trapframe *) (p + sz)) = *(proc->tf);
	
	return 0;
}

// System call for getting process struct
int
sys_processstate(void)
{

	int ch;
    argptr(0, (char **) &ch, sizeof(ch));
	struct proc * pr = (struct proc *) ch;
	save_state(pr);
	return 0;
}

// System call for getting user space for the process
int
sys_memorydump(void)
{
	char * mem;
	int size;
	argptr(0, &mem, sizeof(mem));
	argint(1, &size);
	memory_dump((void *) mem, size);
	return 0;
}

// System call for switching to the given process's file
int 
sys_loadproc(void)
{
	struct proc * p;

	char * file;
	argptr(0, (char **) &p, sizeof(struct proc));

	argptr(1, &file, 16);

	return loadproc(p, file);
}

