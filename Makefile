# -------- PING-PONG (DTEK-V) --------
# Toolchain
CROSS ?= riscv32-unknown-elf
CC    := $(CROSS)-gcc
AS    := $(CROSS)-gcc
LD    := $(CROSS)-gcc
OBJCOPY := $(CROSS)-objcopy
OBJDUMP := $(CROSS)-objdump

# CPU / ABI
ARCH  := -march=rv32im_zicsr -mabi=ilp32
CSTD  := -std=c11

# Common flags
CFLAGS  := $(ARCH) $(CSTD) -O2 -ffreestanding -nostdlib -fno-builtin -Wall -Wextra -Wno-unused-parameter
ASFLAGS := $(ARCH) -x assembler-with-cpp -ffreestanding -nostdlib
LDFLAGS := $(ARCH) -T dtekv-script.lds -nostdlib -Wl,--gc-sections -Wl,-Map=firmware.map

# Files
SRCS_C  := dtekv-lib.c pong.c
SRCS_S  := boot.S labmain.S
OBJS    := $(SRCS_C:.c=.o) $(SRCS_S:.S=.o)

# Output
ELF  := firmware.elf
BIN  := firmware.bin
HEX  := firmware.hex

.PHONY: all clean size disasm

all: $(ELF) $(BIN) $(HEX) size

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(AS) $(ASFLAGS) -c $< -o $@

$(ELF): $(OBJS) dtekv-script.lds
	$(LD) $(LDFLAGS) $(OBJS) -o $@

$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

$(HEX): $(ELF)
	$(OBJCOPY) -O ihex $< $@

size: $(ELF)
	$(OBJDUMP) -h -d $(ELF) > firmware.lst || true
	$(CROSS)-size $(ELF) || true

disasm: $(ELF)
	$(OBJDUMP) -d -S $(ELF) | less

clean:
	rm -f $(OBJS) $(ELF) $(BIN) $(HEX) firmware.map firmware.lst

