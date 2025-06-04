#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

MODULE_AUTHOR("Nilabha Saha");
MODULE_DESCRIPTION("A simple Hello World LKM");
MODULE_LICENSE("Dual MIT/GPL");
MODULE_VERSION("0.1");

static int __init hello_world_lkm_init(void){
	pr_info("Hello Linux, my love >3\n");
	return 0;
}

static void __exit hello_world_lkm_exit(void){
}

module_init(hello_world_lkm_init);
module_exit(hello_world_lkm_exit);
