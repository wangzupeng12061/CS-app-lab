#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "mydemodrv"
#define MYDEMODRV_ADD 0x01
#define MYDEMODRV_SUB 0x02
#define MYDEMODRV_MAX 0x03

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name wzp");
MODULE_DESCRIPTION("Demo Linux device driver");

static int mydemodrv_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "mydemodrv: device opened.\n");
    return 0;
}

static int mydemodrv_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "mydemodrv: device released.\n");
    return 0;
}

static ssize_t mydemodrv_read(struct file *file, char __user *buf,
                              size_t count, loff_t *ppos)
{
    int ret = 0;
    int val = 0;

    // Read the previously written integer value from the driver's private data
    val = *((int *)file->private_data);

    // Copy the value back to user space
    if (copy_to_user(buf, &val, sizeof(int)))
    {
        printk(KERN_ERR "mydemodrv: failed to read data.\n");
        ret = -EFAULT;
    }

    return ret;
}

static ssize_t mydemodrv_write(struct file *file, const char __user *buf,
                               size_t count, loff_t *ppos)
{
    int ret = 0;
    int val = 0;

    // Copy the integer value from user space to the driver's private data
    if (copy_from_user(&val, buf, sizeof(int)))
    {
        printk(KERN_ERR "mydemodrv: failed to write data.\n");
        ret = -EFAULT;
    }
    else
    {
        *((int *)file->private_data) = val;
    }

    return ret;
}

static long mydemodrv_ioctl(struct file *file, unsigned int cmd,
                            unsigned long arg)
{
    int ret = 0;
    int val1 = 0;
    int val2 = 0;
    int result = 0;

    // Read the two integer values and the operation code from user space
    if (copy_from_user(&val1, (int __user *)arg, sizeof(int)))
    {
        printk(KERN_ERR "mydemodrv: failed to read arg1.\n");
        return -EFAULT;
    }

    if (copy_from_user(&val2, (int __user *)arg + 1, sizeof(int)))
    {
        printk(KERN_ERR "mydemodrv: failed to read arg2.\n");
        return -EFAULT;
    }

    // Perform the requested operation
    switch (cmd)
    {
    case MYDEMODRV_ADD:
        result = val1 + val2;
        break;
    case MYDEMODRV_SUB:
        result = val1 - val2;
        break;
    case MYDEMODRV_MAX:
        result = val1 > val2 ? val1 : val2;
        break;
    default:
        printk(KERN_ERR "mydemodrv: invalid command.\n");
        return -EINVAL;
    }
    // Copy the result back to user space
    if (copy_to_user((int __user *)arg + 2, &result, sizeof(int)))
    {
        printk(KERN_ERR "mydemodrv: failed to write result.\n");
        ret = -EFAULT;
    }

    return ret;
}

static const struct file_operations mydemodrv_fops = {
    .owner = THIS_MODULE,
    .read = mydemodrv_read,
    .write = mydemodrv_write,
    .open = mydemodrv_open,
    .release = mydemodrv_release,
    .unlocked_ioctl = mydemodrv_ioctl,
};

static struct miscdevice mydemodrv_misc_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEVICE_NAME,
    .fops = &mydemodrv_fops,
};

static int __init mydemodrv_init(void)
{
    int ret = 0;
    // Register the misc device
    ret = misc_register(&mydemodrv_misc_device);
    if (ret)
    {
        printk(KERN_ERR "mydemodrv: failed to register device.\n");
        return ret;
    }

    printk(KERN_INFO "mydemodrv: device registered with minor number %d.\n",
           mydemodrv_misc_device.minor);

    return 0;
}

static void __exit mydemodrv_exit(void)
{
    misc_deregister(&mydemodrv_misc_device);
    printk(KERN_INFO "mydemodrv: device unregistered.\n");
}

module_init(mydemodrv_init);
module_exit(mydemodrv_exit);
