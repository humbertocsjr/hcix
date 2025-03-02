MAKEDIR = make --no-print-directory $@ -C

all clean:
	@$(MAKEDIR) devkit
	@$(MAKEDIR) boot
	@$(MAKEDIR) library
	@$(MAKEDIR) kernel
	@$(MAKEDIR) distro

run: all
	@qemu-system-i386 -m 4 -fda distro/1440.flp -hdc distro/10404.hd