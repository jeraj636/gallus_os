TARGET = build/iso/EFI/BOOT/BOOTX64.EFI

CC = x86_64-w64-mingw32-gcc
C_FLAGS = -ffreestanding
C_INCLUDE = -Isrc/include \
			-I/usr/x86_64-w64-mingw32/include/efi \
			-I/usr/x86_64-w64-mingw32/include/efi/x86_64 \
			-I/usr/x86_64-w64-mingw32/include/efi/protocol

L_FLAGS = -nostdlib -shared -Wl,-dll -Wl,--subsystem,10 -e efi_main
L_LIBS = -L/usr/x86_64-w64-mingw32/lib

OVMF_PATH = /usr/share/ovmf/x64/OVMF.4m.fd

BUILD_DIR = build/tmp
SRC_DIR = src

FILES = lib/error.c lib/memmap.c lib/command_handlers.c boot.c 
OBJECTS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(FILES))


all: clean $(TARGET)
	@rm -fr $(BUILD_DIR)
run: all
	@qemu-system-x86_64 -bios $(OVMF_PATH) -m 1024M -drive format=raw,file=fat:rw:./build/iso -net none

clean:
	@rm -fr build/
	
$(OBJECTS): $(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "Compiling file: $<"
	@$(CC) $(C_FLAGS) $(C_INCLUDE) -c $< -o $@

# End .EFI file
$(TARGET): $(OBJECTS)
	@mkdir -p $(dir $@)
	@echo "Linking EFI: $@"
	@$(CC) $(L_FLAGS) -o $@ $^ $(L_LIBS) -lefi