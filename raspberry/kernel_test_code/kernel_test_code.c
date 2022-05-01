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

static int __init hello_init(void)
{
    printk("hello world");
    return 0;
}
static void __exit hello_exit(void)
{
   printk("Good bye cruel world\n");

}
module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("test code");
MODULE_DESCRIPTION("Just for test");
MODULE_VERSION("V0.1");
