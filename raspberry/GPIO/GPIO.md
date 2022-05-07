# GPIO
Task:
This code present an example on how to do the coding for char devices.

The app code will toggle the GPIO pin.

Hardware:
1. raspberrypi GPIO1
raspberryPi is based on bcm2711 and the GPIO base physical address is 0x7e200000

2.GPIO1 setting is in the GPFSEL0 [datasheet bcm2711 P66](../../../bcm2711-peripherals.pdf)   





Char device:
1. void *ioremap(unsigned long phys_addr, unsigned long size);
   This function is the bridge making the connection between the virual memory and physical memory.
2. register_chrdev_region(dev_t from, unsigned count, const char* name);
   *dev_t from*: 
        the first in the desired range of device numbers. must include the major number give 0 then the kernel will allocate one
   unsigned count:
        the number of the consecutive device numbers required
    const char* name:
        the name of the device of driver.
3. int alloc_chrdev_region (dev_t * dev,unsigned baseminor,unsigned count,
 	const char * name);
    dev_t * dev
        output parameter for first assigned number
    unsgined baseminor
        first of the request range of minor number
    unsigned count
        the number of minor numbers required
    const char* name
        the name of the associated device or driver
4. file operation structure: 
   To provide write /read functions.
5. MKDEV — Creates a value that can be compared to a kernel device number this function is in include/upai/linux/kdev_t.h
6. class_create(owner, name): This function will build a class presenting on the /sys/class folder.
7. device_create
8. use cmd cat /proc/iomem to get the physical address
   00000000-3b3fffff : System RAM
  00008000-00ffffff : Kernel code
  01200000-013ef203 : Kernel data
40000000-fbffffff : System RAM
fd500000-fd50930f : fd500000.pcie pcie@7d500000
fd580000-fd58ffff : fd580000.ethernet ethernet@7d580000
  fd580e14-fd580e1c : unimac-mdio.-19
fe004000-fe00401f : fe004000.txp txp@7e004000
fe007000-fe007aff : fe007000.dma dma@7e007000
fe007b00-fe007eff : fe007b00.dma dma@7e007b00
fe00a000-fe00a023 : fe100000.watchdog watchdog@7e100000
fe00b840-fe00b87b : fe00b840.mailbox mailbox@7e00b840
fe00b880-fe00b8bf : fe00b880.mailbox mailbox@7e00b880
fe100000-fe100113 : fe100000.watchdog watchdog@7e100000
fe101000-fe102fff : fe101000.cprman cprman@7e101000
fe104000-fe104027 : fe104000.rng rng@7e104000
fe200000-fe2000b3 : fe200000.gpio gpio@7e200000
fe201000-fe2011ff : serial@7e201000
  fe201000-fe2011ff : fe201000.serial serial@7e201000
fe206000-fe2060ff : fe206000.pixelvalve pixelvalve@7e206000
fe207000-fe2070ff : fe207000.pixelvalve pixelvalve@7e207000
fe20a000-fe20a0ff : fe20a000.pixelvalve pixelvalve@7e20a000
fe215000-fe215007 : fe215000.aux aux@7e215000
fe216000-fe2160ff : fe216000.pixelvalve pixelvalve@7e216000
fe300000-fe3000ff : fe300000.mmcnr mmcnr@7e300000
fe340000-fe3400ff : fe340000.mmc mmc@7e340000
fe400000-fe407fff : fe400000.hvs hvs@7e400000
fec00000-fec03fff : fec00000.v3d hub
fec04000-fec07fff : fec00000.v3d core0
fec11000-fec1101f : fe100000.watchdog watchdog@7e100000
fec12000-fec120ff : fec12000.pixelvalve pixelvalve@7ec12000
fef00000-fef0000f : fef00000.clock clock@7ef00000
fef00b00-fef00dff : fef04500.i2c auto-i2c
fef04500-fef045ff : fef04500.i2c bsc
fef05b00-fef05dff : fef09500.i2c auto-i2c
fef09500-fef095ff : fef09500.i2c bsc
600000000-63fffffff : pcie@7d500000
  600000000-6000fffff : PCI Bus 0000:01
    600000000-600000fff : 0000:01:00.0
      600000000-600000fff : xhci-hcd
*The range of gpio is fe200000-fe2000b3*


9. copy_from_user(to, from, size):
   needs to use this function to get the data from application side.
### How to test the GPIO code?
copy gpio.c and gpio_app.c to raspberrypi
make gpio.c
insmod gpio.ko
sudo chmod 777 /dev/my_gpio
genernate gpio_app file
./gpio_app on : high level on GPIO0
./gpio_app off: low level on GPIO0


    
