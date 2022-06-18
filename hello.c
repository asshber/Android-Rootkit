#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/kallsyms.h>
#include <linux/namei.h>

#define enable_reboot 0
MODULE_LICENSE("GPL");
MODULE_VERSION("0.01");

static unsigned long * sys_call_addr;
asmlinkage int (*old_reboot_sys_call)(int,int,int,void*);

asmlinkage int hackers_reboot(int magic1,int magic2,int magic3, void* arg)
{
    if(enable_reboot)
    {
        //magic1=0;
        printk("Reboot Trigged\n");
        return old_reboot_sys_call(magic1,magic2,magic3,arg);
    }
    else
    {
        printk("magic 1 @ %i\n",magic1);
        printk("magic 2 @ %i\n",magic2);
        printk("magic 3 @ %i\n",magic3);
        printk("Project: Blocked Reboot Call\n");
    }
    return EPERM;
}


inline void my_cr0(unsigned long cr0)
{
    asm volatile("mov %0,%%cr0" : "+r"(cr0), "+m"(__force_order));
}

static inline void protect_memory(void)
{
    unsigned long cr0 = read_cr0();
    set_bit(16, &cr0);
    my_cr0(cr0);
}

static inline void unprotect_memory(void)
{
    unsigned long cr0 = read_cr0();
    clear_bit(16, &cr0);
    my_cr0(cr0);
}

static int __init start(void)
{
    sys_call_addr = kallsyms_lookup_name("sys_call_table");

    old_reboot_sys_call=sys_call_addr[__NR_reboot];
    printk("Project:Module: Loaded \n");
    printk("Project:reboot @ 0x%lx\n", old_reboot_sys_call);

    unprotect_memory();
    
    printk("hooked reboot syscall\n");
    sys_call_addr[__NR_reboot] =  hackers_reboot;
    protect_memory();

    return 0;
}

static void __exit end(void)
{
    unprotect_memory();
    printk("Project:restoring all syscall\n");
    sys_call_addr[__NR_reboot]=old_reboot_sys_call;
    protect_memory();
    
    printk("Project:Unloaded \n");
}

module_init(start);
module_exit(end);
