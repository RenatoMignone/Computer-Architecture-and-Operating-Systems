# Define the toolchain and flags
AS = arm-none-eabi-as
CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
CPU = -mcpu=cortex-m4
THUMB = -mthumb
CFLAGS = -c $(CPU) $(THUMB) -g
LDFLAGS = -T linker.ld

# Object files
OBJ = startup.o sync_primitives.o library.o main.o 

# Output file
TARGET = main.elf

# Default target to build the ELF file
all: $(TARGET)

# Rule to create startup.o from startup.s
startup.o: startup.s
	$(AS) $(CPU) -g startup.s -o startup.o

# Rule to create sync_primitives.o from sync_primitives.c
sync_primitives.o: sync_primitives.c
	$(CC) $(CFLAGS) sync_primitives.c -o sync_primitives.o

# Rule to create sync_primitives.o from sync_primitives.c
library.o: library.c
	$(CC) $(CFLAGS) library.c -o library.o

# Rule to create main.o from main.c
main.o: main.c
	$(CC) $(CFLAGS) main.c -o main.o

# Rule to link the object files and create the final ELF
$(TARGET): $(OBJ)
	$(LD) $(LDFLAGS) $(OBJ) -o $(TARGET)

# Clean up the generated files
clean:
	rm -f $(OBJ) $(TARGET)
