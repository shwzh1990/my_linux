/*alloc or configure usb_driver structure*/

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/usb/input.h>
#include <linux/hid.h>

static struct input_def *uk_dev;
static char* usb_buff;
static dma_addr_t usb_buf_phys
static struct urb *uk_urb;


static struct usb_device_id usb_mouse_id_table[] = {
    {USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID, USB_INTERFACE_SUBCLASS_BOOT, USB_INTERFACE_PROTOCOL_MOUSE)},
    { }
};

static void usbmouse_as_key_irq(struct urb* urb)
{
  int i;
  static int cnt = 0;
  printk("data cnt %d:", ++cnt);
  for(i = 0; i < len; i++)
  {
    printk("%d",usb_buf[i]);
  }

  usb_submit_urb(uk_urb, GPF_KERNEL);


}



int len;
static int usb_mouse_probe(struct usb_interface *inf, const struct usb_device_id *id)
{
int pipe;
struct usb_device* dev = interface_to_usbdev(inf);
struct usb_host_interface *interface;
struct usb_endpoint_descriptor *endpoint;
interface = intf->cur_altsetting;
endpoint = &interface->endpoint[0].desc;

/*
 *  alloc input structure
 *  configure
 *  register
 *  hardware 
 * */
uk_dev = input_alloc_device();

set_bit(EV_KEY, uk_dev->evbit);

set_bit(EV_REP, uk_dev->evbit);

set_bit(KEY_L, uk_dev->keybit);
set_bit(KEY_S, uk_dev->keybit);
set_bit(KEY_ENTER, uk_dev->keybit);

input_register_device(uk_dev);


/*data transfer 1. source 2 destination 3. length*/
pipe = usb_rcvintpipe(dev, endpoint->bEndpointAddress);


len = endpoint->wMaxPackageSize;

usb_buffer_alloc(dev, len, GFP_ATOMIC,&usb_buf_phys);

uk_urb = usb_alloc_urb(0, GPF_KERNEL);

usb_fill_int_urb(uk_urb, dev, pipe, usb_buf,len,usbmouse_as_key_irq, NULL, endpoint->bInterval);

uk_urb->transfer_dma = usb_buf_phys;
uk_urb->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;


usb_submit_urb(uk_urb,GPF_KERNEL);

return 0;
}

static void usb_mouse_disconnect(struct usb_interface* inf)
{

struct usb_device *dev = interface_to_usbdev(inf);

usb_kill_urb(uk_urb);
usb_free_urb(uk_urb);
usb_buffer_free(usb_device, len, usb_buf, usb_buf_phys);
input_unregister_device(uk_dev);
input_free_device(uk_dev);
return 0;

}


static struct usb_driver usbmouse_as_key_driver = {
    .name = "usbmouse",
    .probe = usb_mouse_probe,
    .disconnect = usb_mouse_disconnect,
    .id_table = usb_mouse_id_table,


};




static int usbmouse_as_key_init(void)
{
  usb_register(&usbmouse_as_key_driver);
  return 0;
}


static void usbmouse_as_key_exit(void)
{

  usb_deregister(&usbmouse_as_key_driver);
}

module_init(usbmouse_as_key_init);
module_exit(usbmouse_as_key_exit);
module_license("GPL");
