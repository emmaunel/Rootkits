#include <sys/types.h> 
#include <sys/param.h> 
#include <sys/proc.h> 
#include <sys/module.h> 
#include <sys/sysent.h> 
#include <sys/kernel.h> 
#include <sys/systm.h>
#include <sys/syscall.h>
#include <sys/sysproto.h>

/* Hijack the mkdir system call and gives info */
static int mkdir_hook(struct thread *t, void *syscall_args){
    // args: char *path, int mode
    struct mkdir_args *uap;
    uap = (struct mkdir_args *) syscall_args;

    char path[255];
    size_t done;
    int error;

    /* copyinstr(const void *uaddr, void *kaddr, size_t len, size_t *done) */
    error = copyinstr(uap->path, path, 255, &done);
    if(error != 0){
        return(error);
    }

    /* Print a debug message */
    uprintf("Injecting mkdir command...\n");
    uprintf("The directory %s will be created with the following permissions: %o\n", path, uap->mode);
    
    return(sys_mkdir(t, syscall_args));
}

static int potato_handler(struct thread *td, void *arg){

    /* hooking mkdir syscall */
    sysent[SYS_mkdir].sy_call = (sy_call_t *) mkdir_hook;
    return(0);
}

/* Our new system call */
static struct sysent potato_sysent = {
    0, /* number of argurment */
    potato_handler /* implementing function */
};

static int offset = 216;

static int load(struct module *module, int cmd, void *arg){
    int error = 0;
    switch (cmd){
        case MOD_LOAD:
            /* code */
            uprintf("Module Loaded");
            break;
        case MOD_UNLOAD:
            uprintf("Module Unloaded");
            break;
        default:
            error = EOPNOTSUPP;
            break;
    }
    return(error);
}

SYSCALL_MODULE(potat0, &offset, &potato_sysent, load, 0);

