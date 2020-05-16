#include "mm.h"
#include "sched.h"
#include "fork.h"
#include "utils.h"
#include "entry.h"
#include "fair.h"

static int prio = 120;

int copy_process(unsigned long clone_flags, unsigned long fn, unsigned long arg)
{
	preempt_disable();
	struct task_struct *p;

	unsigned long page = allocate_kernel_page();
	p = (struct task_struct *) page;
	struct pt_regs *childregs = task_pt_regs(p);

	if (!p)
		return -1;

	if (clone_flags & PF_KTHREAD) {
		p->thread_info.cpu_context.x19 = fn;
		p->thread_info.cpu_context.x20 = arg;
	} else {
		struct pt_regs * cur_regs = task_pt_regs(current);
		*childregs = *cur_regs;
		childregs->regs[0] = 0;
		copy_virt_memory(p);
	}
	p->flags = clone_flags;
	prio -= 2;
	p->priority = prio;
	p->state = TASK_RUNNING;
	p->thread_info.preempt_count = 1; //disable preemtion until schedule_tail

	p->thread_info.cpu_context.pc = (unsigned long)ret_from_fork;
	p->thread_info.cpu_context.sp = (unsigned long)childregs;
	p->pid = nr_tasks++;

	p->se.cfs_rq = task_cfs_rq(current);

	clear_tsk_need_resched(p);
	set_load_weight(p);

	task_fork_fair(p);

	preempt_enable();
	return p->pid;
}


int move_to_user_mode(unsigned long start, unsigned long size, unsigned long pc)
{
	struct pt_regs *regs = task_pt_regs(current);
	regs->pstate = PSR_MODE_EL0t;
	regs->pc = pc;					//사용자 영역의 startup 함수의 offset을 가리킴 
	regs->sp = 2 *  PAGE_SIZE;  			
	unsigned long code_page = allocate_user_page(current, 0);
	if (code_page == 0)	{
		return -1;
	}
	memcpy(code_page, start, size);
	set_pgd(current->mm.pgd);
	return 0;
}

struct pt_regs * task_pt_regs(struct task_struct *tsk)
{
	unsigned long p = (unsigned long)tsk + THREAD_SIZE - sizeof(struct pt_regs);
	return (struct pt_regs *)p;
}
