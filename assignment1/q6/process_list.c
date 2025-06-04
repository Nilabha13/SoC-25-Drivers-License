#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>

MODULE_AUTHOR("Nilabha Saha");
MODULE_DESCRIPTION("A module which print details of each process.");
MODULE_LICENSE("Dual MIT/GPL");
MODULE_VERSION("0.1");

static int __init process_list_init(void){
	struct task_struct *p;
	char buff[128];

	rcu_read_lock();
	for_each_process(p){
		memset(buff, 0, 128);
		snprintf(buff, 128, "PID: %-10d State: %-8ld Name: %s", p->pid, p->state, p->comm);
		pr_info("%s", buff);

		cond_resched();
	}
	rcu_read_unlock();

	return 0;
}

static void __exit process_list_exit(void){
}

module_init(process_list_init);
module_exit(process_list_exit);
