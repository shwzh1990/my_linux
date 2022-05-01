/**
 * @file kernel_test_code.c
 * @author your name (you@domain.com)
 * @brief This code is just for linux kernel test purpose.
 * @version 0.1
 * @date 2022-04-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include <linux/proc_fs.h>
#include <asm/io.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
/**
 * @brief 
 * @step 1 use ioremap to map the physical address with kernel
 * @step 2 allocate a class to the gpio.
 * @return int 
 */

unsigned int *FSEL; 



static int gpio_open(struct inode* inode, struct file *file)
{
     //step 1 get physical address
    FSEL = (unsigned int*)ioremap(0xfe200000, 1); //map to the physical gpio
    if(FSEL == NULL)
    {
      printk("Failed to map gpio memory to driver\n");
      return -1;
    }
    printk("gpio_open is on\n");
    *FSEL |= (1<<3); 
    return 0;

}

static struct file_operations gpio_fops = {
     .owner = THIS_MODULE,
     .open = gpio_open
};

static int major;

static struct cdev gpio_cdev;
static struct class *gpio_class;
static dev_t gpio_dev;
static int __init gpio_init(void)
{
    
    // step 2 set char device
    if(major)
    {
        gpio_dev=  MKDEV(major, 0);
        register_chrdev_region(gpio_dev,1,"gpio0");
    } 
    else 
    {
      alloc_chrdev_region(&gpio_dev,0,1,"gpio1");
      major = MAJOR(gpio_dev);

    }
    cdev_init(&gpio_cdev, &gpio_fops);
    cdev_add(&gpio_cdev, gpio_dev, 1);

    gpio_class = class_create(THIS_MODULE, "gpio_class");

    device_create(gpio_class, NULL, gpio_dev, NULL, "my_gpio");

    return 0;
}
static void __exit gpio_exit(void)
{
  device_destroy(gpio_class, gpio_dev);
  class_destroy(gpio_class);
  cdev_del(&gpio_cdev);
  unregister_chrdev_region(MKDEV(major,0),1);
  printk("Good bye cruel world\n");

}
module_init(gpio_init);
module_exit(gpio_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("gpio");
MODULE_DESCRIPTION("Just for test");
MODULE_VERSION("V0.1");
