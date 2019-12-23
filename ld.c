#include <stdio.h>
#include <unistd.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <sys/syscalls.h>

static unsigned long *__sys_call_table;

static void *find_systable(){
    unsigned long *sctable;
    unsigned long i = (unsigned long)sys_close;
    while (i < ULONG_MAX){
        /* code */
        sctable = (unsigned long) i;

    }
    
}

static int __init ld_init(void){
    printk("Loaded module");
    __sys_call_table = find_systable();

    printk("Found Table?????");
    //injectwrite();
    return 0;
}

static void __exit ld_exit(void){

    return;
}

module_init(ld_init);
module_exit(ld_exit);
