TARGET_FOLDER = build/iso/EFI/BOOT
TARGET = $(TARGET_FOLDER)/BOOTX64.EFI

OVMF_PATH = /usr/share/ovmf/x64/OVMF.4m.fd


all: clean $(TARGET)

run: all
	@qemu-system-x86_64 -bios $(OVMF_PATH) -m 1024M -drive format=raw,file=fat:rw:./build/iso -net none

clean:
	@rm -fr build/*
	@$(MAKE) clean -C bootloader/

usb: all
	@mount /dev/sda /mnt/usb
	@mkdir -p /mnt/usb/EFI/BOOT
	@mv $(TARGET) /mnt/usb/EFI/BOOT/BOOTX64.EFI
	@umount /mnt/usb/

$(TARGET): bootloader/$(TARGET)
	@mkdir -p $(TARGET_FOLDER)
	@cp bootloader/$(TARGET) $(TARGET)

bootloader/$(TARGET):
	@$(MAKE) -C bootloader/
