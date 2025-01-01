MACHINE = netduinoplus2

TOOLCHAIN = arm-none-eabi

AS = $(TOOLCHAIN)-as
CC = $(TOOLCHAIN)-gcc
LD = $(TOOLCHAIN)-ld
SIZE = $(TOOLCHAIN)-size
OBJCOPY = $(TOOLCHAIN)-objcopy
OBJDUMP = $(TOOLCHAIN)-objdump

ASMFLAGS = -mcpu=cortex-m4
ASMFLAGS += -g

CFLAGS = -mcpu=cortex-m4
CFLAGS += -g
CFLAGS += -Wall
CFLAGS += -O2

LD_SCRIPT = linker_script
LD_FLAGS = -T $(LD_SCRIPT)

TARGET = main.elf

C_SRC = main.c
ASM_SRC = startup.s
SRC = $(ASM_SRC) $(C_SRC) 

OBJS = $(ASM_SRC:%.s=%.o) $(C_SRC:%.c=%.o) 

all: $(TARGET) size Makefile

$(TARGET): $(OBJS) $(LD_SCRIPT) Makefile
	$(LD) $(OBJS) $(LD_FLAGS) -o $@

%.o: %.s Makefile 
	$(AS) $(ASMFLAGS) $< -o $@

%.o: %.c Makefile 
	$(CC) $(CFLAGS) -c $< -o $@


.PHONY: size
size: $(TARGET)
	@echo "Generated ELF file"
	@$(SIZE) $<

.PHONY: run
run:
	qemu-system-arm -M $(MACHINE) -kernel $(TARGET) -serial mon:stdio -nographic

.PHONY: clean
clean :
	rm -f $(TARGET) $(OBJS)
