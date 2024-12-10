BOOT_IMG=ubuntu.img
UEFI_IMG=efi.img
NVRAM=varstore.img

boot:
	qemu-system-aarch64 \
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
