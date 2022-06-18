obj-m:= hello.o
KERNEL_DIR := /home/ashber/new/goldfish
CCPATH_EXT := /home/ashber/prebuilts_gcc_linux-x86_x86_x86_64-linux-android-4.9/bin/x86_64-linux-android-

EXTRA_CFLAGS=-fno-pic
 
all:
	make ARCH=x86_64 CROSS_COMPILE=$(CCPATH_EXT) -C $(KERNEL_DIR) M=$(PWD) EXTRA_CFLAGS=$(EXTRA_CFLAGS) modules
clean:
	make -C $(KERNEL_DIR) M=$(PWD) clean
	rm -rf *.c~
	rm -rf *.o
	rm -f modules.order
