#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/slab.h>
#include<linux/fs.h>
#include<linux/uaccess.h>
#include<linux/kfifo.h>

#define VIR_DEV_NAME "VirtualDisk"
#define VIR_DEV_MAJOR 246
#define BUFFER_SIZE 128

#define WRBUFSIZE _IOW(VIR_DEV_MAJOR, 1, int)
#define REBUFSIZE _IOR(VIR_DEV_MAJOR, 2, int)

char *internal_buf;
struct kfifo fifo;

/*
    unsigned int val;
    int ret;
    int i = 0;
        
    while(!kfifo_is_full(&fifo)) {
        kfifo_in(&fifo, &i, sizeof(i));
        ++i;
    }
    while(!kfifo_is_empty(&fifo)) {
        ret = kfifo_out(&fifo, &val, sizeof(val));
    }

*/

int vir_open(struct inode *inode, struct file *filep) {
    int number = MINOR(inode->i_rdev);
    printk("[CHR_DEV] Virtual Character Device Open: Minor Number: %d\n", number);
    return 0;
}
ssize_t vir_write(struct file *filep, const char *buf, size_t count, loff_t *f_ops) {
    copy_from_user(internal_buf, buf, count);
    internal_buf[count] = 0;
    printk("[CHR_DEV] write data : %s\n", internal_buf);
    return count;
}
ssize_t vir_read(struct file *filep, char *buf, size_t count, loff_t *f_ops) {
    copy_to_user(buf, internal_buf, count);
    printk("[CHR_DEV] read data: %s\n", internal_buf);
    return count;
}
long vir_ioctl(struct file *filep, unsigned int cmd, unsigned long arg) {
    switch (cmd) {
        case 
    }
    return 0;
}
int vir_release(struct inode *inode, struct file *filep) {
    printk("[VirtualDisk] Virtual Disk Device Release\n");
    return 0;
}
struct file_operations vir_fops = {
    .owner = THIS_MODULE,
    .write = vir_write,
    .read = vir_read,
    .open = vir_open,
    .release = vir_release,
    .unlocked_ioctl = vir_ioctl,
};
int vir_init(void) {
    int registration;
    internal_buf = kmalloc(BUFFER_SIZE, GFP_KERNEL);
    printk("VirtualDisk201812505 device driver registered\n");

    if(kfifo_alloc(&fifo, 128, GFP_KERNEL))  //init buffer_size : 128
        printk(KERN_ERR"error kfifo_alloc\n");

    registration = register_chrdev(VIR_DEV_MAJOR, VIR_DEV_NAME, &vir_fops);
    if(registration < 0)
        return registration;
    return 0;
}
void vir_exit(void) {
    printk("VirtualDisk201812505 device driver unregistered\n");
    kfifo_free(&fifo);
    Unregistration_chrdev(VIR_DEV_MAJOR, VIR_DEV_NAME)
}

module_init(vir_init);
module_exit(vir_exit);

MODULE_LICENSE("GPL");

