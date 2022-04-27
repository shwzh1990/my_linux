#include <linux/module.h>
#include <linux/version.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/sched.h>
#include <linux/pm.h>
#include <linux/sysctl.h>
#include <linux/proc_fs.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/input.h>
#include <linux/irq.h>
#include <asm/gpio.h>


struct pin_desc
{
    int irq;
    char *name;
    unsigned int pin;
    unsigned int key_val;

};

struct pin_desc pins_desc[4] = 
{
   {IRQ_EINT0, "s1",S3C2410_GPF0, KEY_L},
   {SIRQ_EINT2,"s2",S3C2410_GPF2,  KEY_S},
   {SIRQ_EINT11,"s3",SC2410_GPG3,  KEY_ENTER},
   {SIRQ_EINT19,"s4",SC2410_GPG11, KEY_LEFTSHIFT},

}


static struct input_dev *buttons_dev;
static struct pin_desc *irq_pd;
static struct time_list buttons_timer;
static irqreturn_t buttons_irq(int irq, void *dev_id)
{
    irq_pd = (struct pins_desc*)dev_id;
    mod_timer(&buttons_timer, jiffies+HZ/100);
    return IRQ_RETVAL(IRQ_HANDLED);


}

static void buttons_timer_function(unsigned long data)
{
    struct pin_desc *pindesc = irq_pd;
    unsigned int pinval;

    if(!pindesc)
        return
    pinval = s3c2410_gpio_getpin(pindesc->pin);

    if(pinval){
        input_event(buttons_dev, EV_KEY, pins_desc->key_val, 0);
        input_sync(buttons_dev);
    }
    else{
        input_event(buttons_dev, EV_KEY, pins_desc->key_val, 1);
        input_sync(buttons_dev);
    }


}


static int buttons_init(void)
{
  //1. input_dev
  buttons_dev = input_allocate_device();  
  //2. config
  //can generate key press event
  set_bit(EV_KEY,buttons_dev->evbit);
  //what thing can be produce in the press event?
  //want to generate ls enter leftshit
  set_bit(KEY_L, buttons_dev->keybit);
  set_bit(KEY_S, buttons_dev->keybit);
  set_bit(KEY_ENTER, buttons_dev->keybit);
  set_bit(KEY_LEFTSHIFT, buttons_dev->keybit);
  
  for(int i = 0; i < 4; i++)
  {
    request_irq(pins_desc[i].iqr, button_irq, IRQT_BOTHEDGE, pin_desc.name,)

  }
  init_timer(&buttons_timer);
  buttons_timer.function = buttons_timer_function;
  add_timer(&buttons_timer);
  //3. register
  input_register_dev()
  //4. hardware

  return 0;
}

static void buttons_exit(void)
{
  int i;
  for (i = 0 ; i < 4; i++)
  {
   free_irq(pins_desc[i].irq);
  }
  del_timer(&buttons_timer);
  input_unregister_device(buttons_dev);
  input_free_device(buttons_dev);
}

module_init(buttons_init);
module_exit(buttons_exit);
module_license("GPL");
