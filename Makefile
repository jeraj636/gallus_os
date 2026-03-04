BOOTLOADER_PATH = build/iso/EFI/BOOT/BOOTX64.EFI build/tmp/BOOTX64.EFI

OVMF_PATH = /usr/share/ovmf/x64/OVMF.4m.fd

CC = x86_64-w64-mingw32-gcc
C_FLAGS = -ffreestanding
C_INCLUDE = -I/usr/x86_64-w64-mingw32/include/efi -I/usr/x86_64-w64-mingw32/include/efi/x86_64 -I/usr/x86_64-w64-mingw32/include/efi/protocol

L_FLAGS = -nostdlib -shared -Wl,-dll -Wl,--subsystem,10 -e efi_main
L_LIBS = -L/usr/x86_64-w64-mingw32/lib

BUILD_DIR = build/tmp
ISO_DIR = build/iso/EFI/BOOT

TARGET = $(ISO_DIR)/BOOTX64.EFI

all: $(TARGET)

run: $(TARGET)
	@qemu-system-x86_64 -bios $(OVMF_PATH) -m 1024M -drive format=raw,file=fat:rw:./build/iso -net none

usb: $(TARGET)
	mount /dev/sda /mnt/usb/
	cp $(TARGET) /mnt/usb/EFI/BOOT/BOOTX64.EFI
	umount /mnt/usb/

# memmap.c
$(BUILD_DIR)/memmap.o: src/bootloader/lib/memmap.c
	@mkdir -p build/tmp
	@$(CC)  $(C_FLAGS) $(C_INCLUDE) -c src/bootloader/lib/memmap.c -o build/tmp/memmap.o 

# error.c
$(BUILD_DIR)/error.o: src/bootloader/lib/error.c
	@mkdir -p build/tmp
	@$(CC)  $(C_FLAGS) $(C_INCLUDE) -c src/bootloader/lib/error.c -o build/tmp/error.o 


# boot.c
$(BUILD_DIR)/boot.o: src/bootloader/boot.c
	@mkdir -p build/tmp
	@$(CC)  $(C_FLAGS) $(C_INCLUDE) -c src/bootloader/boot.c -o build/tmp/boot.o 

# End .EFI filea
$(TARGET): $(BUILD_DIR)/boot.o $(BUILD_DIR)/memmap.o $(BUILD_DIR)/error.o
	@mkdir -p $(ISO_DIR)
	@$(CC) $(L_FLAGS) -o $(TARGET) $(BUILD_DIR)/boot.o $(BUILD_DIR)/memmap.o $(BUILD_DIR)/error.o $(L_LIBS) -lefi


