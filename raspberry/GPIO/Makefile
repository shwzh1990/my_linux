DIR=~/Documents/learn/linux/linux
obj-m = kernel_test_code.o
all:
	make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- -C $(DIR) M=$(PWD) modules
clean:
	rm -fr modules.order modules.* *.o *.mode.o *.ko *.mod.c *.mod Module.*

