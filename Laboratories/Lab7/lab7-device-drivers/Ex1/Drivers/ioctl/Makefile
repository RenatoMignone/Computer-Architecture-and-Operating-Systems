obj-m += chardev2.o 
TEST_OBJ = userspace_ioctl.o
TEST_BIN = userspace_ioctl

PWD := $(CURDIR) 
 
all: 
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules 
	$(CC) -o $(TEST_BIN) userspace_ioctl.c  # Build the test binary from ioctl.c

 
clean: 
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f $(TEST_BIN) $(TEST_OBJ)
