#include <linux/gpio.h> 
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hugh J Nuds");
MODULE_DESCRIPTION("A driver to implement Hitlers regime on your pc");
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

struct file_operations switch_fops = {
    .owner = THIS_MODULE,
    .open = switch_open,
    .release = switch_release,
    // .write = switch_write,
    // .read = switch_read,
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