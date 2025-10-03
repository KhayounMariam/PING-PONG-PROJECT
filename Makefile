TARGET = main
OBJS   = boot.o labmain.o dtekv-lib.o pong.o

CC = riscv32-unknown-elf-gcc
AS = riscv32-unknown-elf-gcc
OBJCOPY = riscv32-unknown-elf-objcopy
OBJDUMP = riscv32-unknown-elf-objdump

ARCH = -march=rv32im -mabi=ilp32
CFLAGS = $(ARCH) -Os -ffreestanding -nostdlib -Wall -Wextra
ASFLAGS = $(ARCH)
LDFLAGS = -T dtekv-script.lds -nostdlib -Wl,--gc-sections

all: $(TARGET).elf $(TARGET).bin $(TARGET).elf.txt

$(TARGET).elf: $(OBJS) dtekv-script.lds
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(AS) $(ASFLAGS) -c $< -o $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

$(TARGET).elf.txt: $(TARGET).elf
	$(OBJDUMP) -d -S $< > $@

clean:
	rm -f $(OBJS) $(TARGET).elf $(TARGET).bin $(TARGET).elf.txt

.PHONY: all clean
