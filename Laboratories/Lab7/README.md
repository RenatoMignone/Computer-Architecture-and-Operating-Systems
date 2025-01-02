# Set-up
Clone the repo with --recurse-submodules.

In this lab you will develop a Linux device driver to interact with a simulated hardware component in qemu. You will use a generic qemu emulated ARM system called virt to boot a 64 bit Linux 22.04 image. The boot process takes some time, so we will start by setting up everything, so that, while Linux boots, you can work on exercise 1.

### Create required flash volumes
Create a first flash volume to store UEFI boot variables in Non-Volatile RAM (NVRAM) memory

```
truncate -s 64m varstore.img
```

Create a second flash volume containing the ARM UEFI firmware

```
truncate -s 64m efi.img
dd if=/usr/share/qemu-efi-aarch64/QEMU_EFI.fd of=efi.img conv=notrunc
```

### Download Ubuntu image

```
wget https://cloud-images.ubuntu.com/jammy/20241004/jammy-server-cloudimg-arm64.img -O ubuntu.img
```

### Boot Linux on the virt board

```
make boot
```

Now let it cook, open another terminal and start working on exercise 1.

Check periodically the output while working on the exercise. When it stop
printing stuff, try pressing enter and see if you have access to the shell. If
yes run 

```
sudo apt update && sudo apt install -y build-essential
```

We will need them later to compile our kernel module. It will take again some time, so go back to exercise 1.
