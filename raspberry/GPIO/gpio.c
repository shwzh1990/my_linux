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

#define GPIO_BASE (0xfe200000)
#define GPSET0_OFFSET (0x1C)
#define GPCLR0_OFFSET (0x28)
#define GPIO_PUP_PDN_CNTRL_REG0GPIO_OFFSET (0xE4)
volatile unsigned int *FSEL0 ; 
volatile unsigned int *GPSET0 ; 
volatile unsigned int *GPCLR0 ;
volatile unsigned int *GPIO_PUP_PDN_CNTRL_REG0GPIO; 





static int gpio_open(struct inode* inode, struct file *file)
{
     //step 1 get physical address
    FSEL0 = (unsigned int*)ioremap(0xfe200000, 1); //map to the physical gpio
    GPSET0 = (unsigned int*)ioremap(GPIO_BASE + GPSET0_OFFSET, 1); 
    GPCLR0 = (unsigned int*)ioremap(GPIO_BASE + GPCLR0_OFFSET, 1);
    GPIO_PUP_PDN_CNTRL_REG0GPIO = (unsigned int*)ioremap(GPIO_BASE + GPIO_PUP_PDN_CNTRL_REG0GPIO_OFFSET, 1);
    if(!FSEL0 || !GPSET0 || !GPCLR0 || !GPIO_PUP_PDN_CNTRL_REG0GPIO )
       printk("cannot get the memory!\n");
    
    printk("gpio_open is on\n");
    *FSEL0 |= (1<<0);                                //gpio 0 output
    *GPIO_PUP_PDN_CNTRL_REG0GPIO |= 0x01;            //gpio 0 pull-up
    return 0;

}
ssize_t gpio_write(struct file *file, const char __user *buff, size_t count, loff_t *loff_t)
{
   uint8_t val;
   copy_from_user(&val, buff, 1);
   printk("%d",count);
   if(val == 1)
   {
      *GPSET0 |= (1<<0);
      printk("GPIO0 turn to 1!\n");
   }
   else
   {
     *GPCLR0 |= (1<<0);
     printk("GPIO0 turn to 0!\n");
   }

    return 0;
}


static struct file_operations gpio_fops = {
     .owner = THIS_MODULE,
     .open = gpio_open,
     .write = gpio_write
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
  iounmap(FSEL0);
  iounmap(GPSET0);
  iounmap(GPCLR0);
  iounmap(GPIO_PUP_PDN_CNTRL_REG0GPIO);
  printk("Good bye cruel world\n");

}
module_init(gpio_init);
module_exit(gpio_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("gpio");
MODULE_DESCRIPTION("Just for test");
MODULE_VERSION("V0.1");

