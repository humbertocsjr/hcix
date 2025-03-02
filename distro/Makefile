OUT = 1440.flp 10404.hd
BOOT_FLOPPY = ../boot/floppy.boot
KERNEL = ../kernel/hcix.kern
INSTALLBOOT = ../devkit/installboot

all: $(OUT)
	@echo >> /dev/null

clean:
	@rm -f $(OUT)


mk%.flp: 
	@dd if=/dev/zero of=$(patsubst mk%,%,$@) bs=1024 count=$(patsubst mk%.flp,%,$@) status=none

1440.flp: $(INSTALLBOOT) $(BOOT_FLOPPY) $(KERNEL) Makefile
	@make --no-print-directory mk$@
	@$(INSTALLBOOT) $@ $(KERNEL) $(BOOT_FLOPPY) 80 2 18

%.hd: $(BOOT_FLOPPY) $(KERNEL) Makefile
	@minixfs mkfs $@ -s $(patsubst %.flp,%,$@) -2 -n 30
	@minixfs mkdir $@ /Applications
	@minixfs mkdir $@ /System
	@minixfs mkdir $@ /System/Kernel
	@minixfs mkdir $@ /Users
	@minixfs mkdir $@ /Volumes
	@minixfs mkdir $@ /Library
	@minixfs add $@ $(KERNEL) /System/Kernel/hcix.kern
	@minixfs add $@ $(BOOT_FLOPPY) /System/Kernel/floppy.boot