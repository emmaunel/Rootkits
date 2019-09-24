#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/syscalls.h>

static int __init backdoor_init(void){
	printk("[file] module loaded\n");
}

static void __exit backdoor_exit(void){
	printk("[file] module removed\n");
}

module_init(backdoor_init);
module_exit(backdoor_exit);
