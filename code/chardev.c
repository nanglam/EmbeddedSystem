#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/slab.h>
#include<linux/fs.h>
#include<linux/uaccess.h>

#define CHR_DEV_NAME "chardev"
#define CHR_DEV_MAJOR 257
#define BUFFER_SIZE 128

static char *internal_buf;

int chr_open(struct inode *inode, struct file *filep) {
    int number = MINOR(inode->i_rdev);
    printk("Device Open : %d\n",number);
    return 0;
}
ssize_t chr_write(struct file *filep, const char *buf, size_t count, loff_t *f_ops) {
    copy_from_user(internal_buf, buf, count);
    internal_buf[count] = 0;
    printk("Write Data : %s\n", internal_buf);
    return count;
	// int bytes_write = 0;

    // /* If we're at the end of the message, return 0 signifying end of file */
    // if(*internal_buf == 0)
    //   return 0;

    // /* Acually put the data into the buffer */
    // while (count && *internal_buf)
    // {
    //   /*
    //   *   The buffer is in the user data segment, not the kernel
    //   *   segment so "*" assignment won't work. We have to use
    //   *   put_user which copies data from the kernel data segment to
    //   *   the user data segment.
    //   */

    //   get_user(*(internal_buf++), buf++);

    //   count--;
    //   bytes_write++;
    // }

    // /* Read functions return the number of bytes put into the buffer */
    // return bytes_write;

	
}
ssize_t chr_read(struct file *filep, char *buf, size_t count, loff_t *f_ops) {
    copy_to_user(buf, internal_buf, count);
    printk("Read Data : %s\n", internal_buf);
    return count;
    // /* Number of bytes actually written to the buffer */
    // int bytes_read = 0;

    // /* If we're at the end of the message, return 0 signifying end of file */
    // if(*internal_buf == 0)
    //   return 0;

    // /* Acually put the data into the buffer */
    // while (count && *internal_buf)
    // {
    //   /*
    //   *   The buffer is in the user data segment, not the kernel
    //   *   segment so "*" assignment won't work. We have to use
    //   *   put_user which copies data from the kernel data segment to
    //   *   the user data segment.
    //   */

    //   put_user(*(internal_buf++), buf++);

    //   count--;
    //   bytes_read++;
    // }

    // /* Read functions return the number of bytes put into the buffer */
    // return bytes_read;
}
long chr_ioctl(struct file *filep, unsigned int cmd, unsigned long arg) {
    switch(cmd) {
        case 0 : printk("a"); break;
    }
    return 0;
}
int chr_release(struct inode *inode, struct file *filep) {
    printk("Virtual Char Device Release\n");
    return 0;
}
struct file_operations chr_fops = {
    .owner = THIS_MODULE,
    .write = chr_write,
    .read = chr_read,
    .open = chr_open,
    .release = chr_release,
    .unlocked_ioctl = chr_ioctl,
};
int chr_init(void) {
    int registration;
    internal_buf = kmalloc(BUFFER_SIZE, GFP_KERNEL);

    printk("Good\n");
    registration = register_chrdev(CHR_DEV_MAJOR, CHR_DEV_NAME, &chr_fops);
    if(registration < 0)
        return registration;
    printk("Number : %d\n", registration);
    return 0;
}
void chr_exit(void) {
    printk("Unregistration\n");
}
module_init(chr_init);
module_exit(chr_exit);
MODULE_LICENSE("GPL");

