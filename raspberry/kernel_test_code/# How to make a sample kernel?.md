# How to make a sample kernel?
1. send the file to the raspberry 
2. make a small make file
```
obj-m += XXX.o
all:
    make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
    make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
```