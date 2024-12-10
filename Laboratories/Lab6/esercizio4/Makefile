# Specify the module name
obj-m := esercizio4.o
obj-m += startstop.o
startstop-objs := start.o stop.o

# List all source files that make up the module
esercizio4-objs := init.o exit.o string_rev.o

# Specify the kernel build directory
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(CURDIR)

# Build instructions
all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean

