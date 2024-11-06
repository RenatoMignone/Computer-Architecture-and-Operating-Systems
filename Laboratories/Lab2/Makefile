ARMGNU ?= arm-none-eabi

AARCH = -mcpu=cortex-m4
AOPS = -g $(AARCH)
COPS = -Wall -g -O2 $(AARCH)

main=main
startup=startup
ld_script=linker_script

main.bin : $(startup).o $(main).o $(ld_script)
	$(ARMGNU)-ld $(startup).o $(main).o -T $(ld_script) -o $(main).elf
	$(ARMGNU)-objdump -D $(main).elf > $(main).list
	$(ARMGNU)-objcopy $(main).elf -O binary $(main).bin

startup.o : startup.s
	$(ARMGNU)-as $(AOPS) $(startup).s -o $(startup).o

main.o : main.c 
	$(ARMGNU)-gcc -c $(COPS) $(main).c -o $(main).o

run:
	qemu-system-arm -M netduinoplus2 -kernel $(main).elf -serial mon:stdio -nographic

clean :
	rm -f *.o
	rm -f *.elf
	rm -f *.bin
	rm -f *.list

