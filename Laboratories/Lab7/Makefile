IMG_URL=https://cloud-images.ubuntu.com/jammy/20241004/jammy-server-cloudimg-arm64.img
BOOT_IMG=ubuntu.img
UEFI_IMG=efi.img
NVRAM=varstore.img

export LIBGUESTFS_DEBUG=1 LIBGUESTFS_TRACE=1

all: varstore efi download password

varstore:
	truncate -s 64m $(NVRAM)

efi:
	truncate -s 64m $(UEFI_IMG)
	dd if=/usr/share/qemu-efi-aarch64/QEMU_EFI.fd of=$(UEFI_IMG) conv=notrunc

download:
	wget $(IMG_URL) -O $(BOOT_IMG)

password:
	sudo virt-customize -a ubuntu.img --root-password password:root

boot:
	sudo qemu-system-aarch64 \
	-m 2048 \
	-cpu max \
	-M virt \
	-nographic \
	-drive if=pflash,format=raw,file=$(UEFI_IMG) \
	-drive if=pflash,format=raw,file=$(NVRAM) \
	-drive if=none,file=$(BOOT_IMG),id=hd0 \
	-device virtio-blk-device,drive=hd0 \
	-netdev user,id=net0 \
	-device virtio-net-device,netdev=net0

clean:
	rm -rf *.img
