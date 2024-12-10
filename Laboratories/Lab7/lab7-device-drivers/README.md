# Set-up
Clone the repo with --recurse-submodules.

In this lab you will develop a Linux device driver to interact with a simulated hardware component in qemu. You will use a generic qemu emulated ARM system called virt to boot a 64 bit Linux 22.04 image. The boot process takes some time, so we will start by setting up everything, so that, while Linux boots, you can work on exercise 1.

Disclaimer
These steps are simulated in Linux and have the goal to let you test the code you will develop in exercises 2 and 3. Don't become crazy and focus on the exercises,  you can develop your code in your host machine. The only drawback is that you won't be able to access the simulated UART, so you won't be able to test it. You can ask some of your colleagues to copy-paste your code on their Linux if you want to see the output.

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

### Create a password for the root user

If not installed, install virt-customize

```
apt install virt-customize
```

Customize the image's password

```
export LIBGUESTFS_DEBUG=1 LIBGUESTFS_TRACE=1
sudo virt-customize -a ubuntu.img --root-password password:root
```

This will set the root password to "root"

### Boot Linux on the virt board

```
make boot
```

Now let it cook, open another terminal and start working on exercise 1.

Check periodically the output while working on the exercise. It whould take around 5 minutes to boot. When it stop printing stuff, try pressing enter and see if you have access to the shell. If yes run 

```
sudo apt update && sudo apt install -y build-essential
```

We will need them later to compile our kernel module. It will take again some time, so go back to exercise 1.
