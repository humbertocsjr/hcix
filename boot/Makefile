OUT = floppy.boot

all: $(OUT)
	@echo >> /dev/null

clean:
	@rm -f $(OUT)

%.boot: %.asm
	@nasm -f bin -o $@ $<