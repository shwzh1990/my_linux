/*alloc or configure usb_driver structure*/

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/usb/input.h>
#include <linux/hid.h>


static struct usb_device_id usb_mouse_id_table[] = {
    {USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID, USB_INTERFACE_SUBCLASS_BOOT, USB_INTERFACE_PROTOCOL_MOUSE)},
    { }
};

static int usb_mouse_probe(struct usb_interface *inf, const struct usb_device_id *id)
{
struct usb_device* dev = interface_to_usbdev(inf);
return 0;
}

static void usb_mouse_disconnect(struct usb_interface* inf)
{

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
