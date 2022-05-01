1. First install Git and the build dependencies
```
sudo apt install git bc bison flex libssl-dev make libc6-dev libncurses5-dev
```
2. Next get the sources
```
git clone --depth=1 https://github.com/raspberrypi/linux
```
3. install cross compile tool.
   Our raspberry is Rasp 4 64bits so need a 64bits cross compiler
```
sudo apt install crossbuild-essential-arm64
```
4. use cmd below to set the config file
```
    cd linux
    KERNEL=kernel8
    make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- bcm2711_defconfig
```   
5. compile and generate the zImage
```
    make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- Image modules dtbs 
```   
6. insert SD card and use lsblk to check the partition in the sd card:
 
NAME        MAJ:MIN RM   SIZE RO TYPE MOUNTPOINT
sda           8:0    0 931.5G  0 disk 
├─sda1        8:1    0   512M  0 part /boot/efi
└─sda2        8:2    0   931G  0 part /
sr0          11:0    1  1024M  0 rom  
mmcblk0     179:0    0  28.9G  0 disk 
├─mmcblk0p1 179:1    0   256M  0 part /media/shwzh1990/boot
└─mmcblk0p2 179:2    0  28.7G  0 part /media/shwzh1990/rootfs
7. mount the fat32 and ext4 to certain folder
```
mkdir mnt
mkdir mnt/fat32
mkdir mnt/ext4
sudo mount /dev/mmcblk0p1 mnt/fat32
sudo mount /dev/mmcblk0p2 mnt/ext4

```
8. In the linux kernel folder use command "*sudo make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- INSTALL_MOD_PATH=../mnt/ext4 modules_install*" to install the modules on the ext4.
9. sudo cp arch/arm64/boot/dts/broadcom/*.dtb ../mnt/fat32
10. sudo cp arch/arm64/boot/dts/overlays/\*.dtb\* ../mnt/fat32/overlays
11. sudo cp arch/arm64/boot/dts/overlays/README ../mnt/fat32/overlays
12. sudo umount /mnt/fat32 && sudo umount /mnt/ext4
13. we use ssh to commnunicate with raspberry pi
    password:123
14. [reference](https://www.raspberrypi.com/documentation/computers/linux_kernel.html)    
 