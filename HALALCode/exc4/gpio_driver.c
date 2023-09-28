#include <linux/gpio.h> 
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hugh J Nuds");
MODULE_DESCRIPTION("Modulervoks");
MODULE_VERSION("69.420");

const int gpio_num = 16;
const int first_minor = 0;
const int max_devices = 255;
static dev_t devno;
static struct class *switch_class;
static struct cdev switch_cdev;

int switch_open(struct inode *inode, struct file *filep)
{
    int major, minor;
    major = MAJOR(inode->i_rdev);
    minor = MINOR(inode->i_rdev);
    printk("Opening MyGpio Device [major], [minor]: %i, %i\n", major, minor);
    return 0;
}

int switch_release(struct inode *inode, struct file *filep)
{
    int minor, major;

    major = MAJOR(inode->i_rdev);
    minor = MINOR(inode->i_rdev);
    printk("Closing/Releasing MyGpio Device [major], [minor]: %i, %i\n", major, minor);

    return 0;
}

ssize_t switch_write(struct file *filep, const char __user *ubuf, size_t count, loff_t *f_pos)
{
    int len, value;
    char kbuf[12];

    len = count < sizeof(kbuf) ? count : sizeof(kbuf) - 1;

    if (copy_from_user(kbuf, ubuf, len))
        return -EFAULT;

    kbuf[len] = '\0'; // Null-terminate the buffer

    if (sscanf(kbuf, "%d", &value) != 1)
        return -EINVAL;

    gpio_set_value(26, value); // Set the GPIO value
    pr_info("Wrote %i to GPIO\n", value);

    *f_pos += len;

    return len;
}

ssize_t switch_read(struct file *filep, char __user *buf, size_t count, loff_t *f_pos)
{
    // Hvilke trin er det der skal udføres?
    // Hint konvertering fra int til string kan gøres via sprintf() - Tekststrenge frem for binære værdier gør det nemmere at læse værdien i user-space med f.eks. cat.

    char kbuf[12];
    int len, value;
    value = gpio_get_value(gpio_num);
    len = count < 12 ? count : 12; /* Truncate to smallest */
    len = snprintf(kbuf, len, "%i", value); /* Create string */
    unsigned long ret = copy_to_user(buf, kbuf, ++len); /* Copy to user */
    if (ret)
    {
        return -EFAULT;
    }

    *f_pos += len; /* Update cursor in file */

    return len;
}

struct file_operations switch_fops = {
    .owner = THIS_MODULE,
    .open = switch_open,
    .release = switch_release,
    .write = switch_write,
    .read = switch_read,
};

static int switch_init(void)
{ 
    // Request GPIO
    // Set GPIO direction (in or out)
    // Alloker Major/Minor
    // Class Create 
    // Cdev Init
    // Add Cdev

    int err = 0;

    // GPIO ting
    err = gpio_request(gpio_num, "swii");
    if (err < 0)
    {
        //err
    }

    err = gpio_direction_input(gpio_num);
    if (err < 0)
    {
        //err
    }

    err = gpio_direction_output(26, 0);
    if (err < 0)
    {
        //err
    }

    // Major minor nummer halløj
    err = alloc_chrdev_region(&devno, first_minor, max_devices, "switch-driver");
    if(MAJOR(devno) <= 0)
    {
        pr_err("Failed to register chardev\n");
    }

    pr_info("Hello driver got Major %i\n", MAJOR(devno));

    // Create class
    switch_class = class_create(THIS_MODULE, "switch-class");
    if (IS_ERR(switch_class))
    {
        pr_err("Failed to create class");
    }

    // Init and add cdev
    cdev_init(&switch_cdev, &switch_fops);
    err = cdev_add(&switch_cdev, devno, 255);
    if (err)
    {
        pr_err("Failed to add cdev");
    }

    return err;
}

static void switch_exit(void)
{
    // Delete Cdev
    // Unregister Device
    // Class Destroy
    // Free GPIO

    cdev_del(&switch_cdev);
    unregister_chrdev_region(devno, max_devices);
    class_destroy(switch_class);
    gpio_free(gpio_num);

    pr_info("Removed switch driver and fucked your mom");
}

module_init(switch_init);
module_exit(switch_exit);