#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/slab.h>
#include<linux/fs.h>
#include<linux/uaccess.h>
#include<linux/kfifo.h>

#define VD_DEV_NAME "chardev"
#define VD_DEV_MAJOR 256

// ioctl control buf size
//#define WRBUFSIZE _IOW(VD_DEV_MAJOR, 1, int)
//#define REBUFSIZE _IOR(VD_DEV_MAJOR, 2, int)

struct kfifo fifo_buf; // internal buf (kfifo)
static int buf_size = 128; //init buf size (N)
static int read_buf_size = 3; // init read buf size (M)

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

int vd_open(struct inode *inode, struct file *filep) {
    // int number = MINOR(inode->i_rdev);
    // printk("[VirtualDisk] Virtual Disk Device Open: Minor Number: %d\n", number);
    return 0;
}
ssize_t vd_write(struct file *filep, const char *buf, size_t length, loff_t *f_ops) {
    int i;
    char char_buf;
    char *internal_buf; 
    
    internal_buf=kmalloc(length, GFP_KERNEL);
    copy_from_user(internal_buf, buf, length);

    printk("length : %d\n", length);
    printk("buf : %s\n", buf);
    for(i=0; i<length; i++) { 
        if(kfifo_is_full(&fifo_buf)) {
            continue;
            //kfifo_out(&fifo_buf, ) 구현하기
        }
        char_buf = internal_buf[i];
        kfifo_in(&fifo_buf, &char_buf, sizeof(char_buf));
    }
    
    kfree(internal_buf);
    return length;
}
ssize_t vd_read(struct file *filep, char *buf, size_t length, loff_t *f_ops) {
    int i;
    char val;
    char *internal_buf;
    internal_buf=kmalloc(read_buf_size, GFP_KERNEL);
    memset(internal_buf, 0, read_buf_size);


    printk("length : %d\n", length);
    printk("buf : %s\n", buf);
    if(kfifo_is_empty(&fifo_buf)) {
        kfree(internal_buf);
        return length;
    }

    for(i=0; i<read_buf_size; i++){
        if(kfifo_is_empty(&fifo_buf))
            break;
        kfifo_out(&fifo_buf, &val, sizeof(val));
        internal_buf[i] = val;
    }

    copy_to_user(buf, internal_buf, i);
    kfree(internal_buf);
    return length;
}
long vd_ioctl(struct file *filep, unsigned int cmd, unsigned long arg) {
    // switch (cmd) {
    //     case 1 break;
    // }
    return 0;
}
int vd_release(struct inode *inode, struct file *filep) {
    printk("[VirtualDisk] Virtual Disk Device Release\n");
    return 0;
}
struct file_operations vd_fops = {
    .owner = THIS_MODULE,
    .write = vd_write,
    .read = vd_read,
    .open = vd_open,
    .release = vd_release,
    .unlocked_ioctl = vd_ioctl,
};
int vd_init(void) {
    int registration;
    printk("VirtualDisk201812505 device driver registered\n");

    if(kfifo_alloc(&fifo_buf, buf_size, GFP_KERNEL))  //init buffer_size : 128
        printk(KERN_ERR"error kfifo_alloc\n");

    registration = register_chrdev(VD_DEV_MAJOR, VD_DEV_NAME, &vd_fops);
    if(registration < 0)
        return registration;
    return 0;
}
void vd_exit(void) {
    printk("VirtualDisk201812505 device driver unregistered\n");
    kfifo_free(&fifo_buf);
    unregister_chrdev(VD_DEV_MAJOR, VD_DEV_NAME);
}

module_init(vd_init);
module_exit(vd_exit);
MODULE_LICENSE("GPL");