#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/kfifo.h>
#include <linux/wait.h>
#include <linux/sched.h>

#define DEVICE_NAME "kfifo_buffer"
#define CLASS_NAME  "kfifo"

static int    major_number;
static struct class*  kfifo_class  = NULL;
static struct device* kfifo_device = NULL;

static DEFINE_KFIFO(kfifo_buffer, char, 32);
static DECLARE_WAIT_QUEUE_HEAD(read_queue);
static DECLARE_WAIT_QUEUE_HEAD(write_queue);

static int     kfifo_open(struct inode *, struct file *);
static int     kfifo_release(struct inode *, struct file *);
static ssize_t kfifo_read(struct file *, char *, size_t, loff_t *);
static ssize_t kfifo_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations fops =
{
   .open = kfifo_open,
   .read = kfifo_read,
   .write = kfifo_write,
   .release = kfifo_release,
};

static int __init kfifo_buffer_init(void){
   major_number = register_chrdev(0, DEVICE_NAME, &fops);
   if (major_number < 0){
      printk(KERN_ALERT "Kfifo_buffer failed to register a major number\n");
      return major_number;
   }
   kfifo_class = class_create(THIS_MODULE, CLASS_NAME);
   if (IS_ERR(kfifo_class)){
      unregister_chrdev(major_number, DEVICE_NAME);
      printk(KERN_ALERT "Failed to register device class\n");
      return PTR_ERR(kfifo_class);
   }
   kfifo_device = device_create(kfifo_class, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);
   if (IS_ERR(kfifo_device)){
      class_destroy(kfifo_class);
      unregister_chrdev(major_number, DEVICE_NAME);
      printk(KERN_ALERT "Failed to create the device\n");
      return PTR_ERR(kfifo_device);
   }
   return 0;
}

static void __exit kfifo_buffer_exit(void){
   device_destroy(kfifo_class, MKDEV(major_number, 0));
   class_unregister(kfifo_class);
   class_destroy(kfifo_class);
   unregister_chrdev(major_number, DEVICE_NAME);
}

static int kfifo_open(struct inode *inodep, struct file *filep){
   try_module_get(THIS_MODULE);
   return 0;
}

static int kfifo_release(struct inode *inodep, struct file *filep){
   module_put(THIS_MODULE);
   return 0;
}

static ssize_t kfifo_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
   int ret;
   size_t read_len;
   unsigned int copied;
   printk(KERN_INFO "Process %d is trying to read from the buffer\n", current->pid);
   wait_event_interruptible(read_queue, !kfifo_is_empty(&kfifo_buffer));
   printk(KERN_INFO "Buffer is not empty now, process %d can read from the buffer\n", current->pid);

   read_len = min(len, kfifo_len(&kfifo_buffer));
   
   ret = kfifo_to_user(&kfifo_buffer, buffer, read_len, &copied);

   wake_up_interruptible(&write_queue);
   printk(KERN_INFO "Process %d has read %zu bytes from the buffer\n", current->pid, read_len);

   return ret ? ret : read_len;
}


static ssize_t kfifo_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
   int ret;
   size_t write_len;
   unsigned int copied;
   printk(KERN_INFO "Process %d is trying to write to the buffer\n", current->pid);
   wait_event_interruptible(write_queue, !kfifo_is_full(&kfifo_buffer));
   printk(KERN_INFO "Buffer is not full now, process %d can write to the buffer\n", current->pid);

   write_len = min(len, kfifo_avail(&kfifo_buffer));
   
   ret = kfifo_from_user(&kfifo_buffer, buffer, write_len, &copied);

   wake_up_interruptible(&read_queue);
   printk(KERN_INFO "Process %d has written %zu bytes to the buffer\n", current->pid, write_len);

   return ret ? ret : write_len;
}

module_init(kfifo_buffer_init);
module_exit(kfifo_buffer_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("wzp");
MODULE_DESCRIPTION("A simple Linux kfifo buffer driver");
MODULE_VERSION("1.0");
