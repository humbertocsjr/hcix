OUT = hcix.kern
SRC = init.asm hcix.c $(wildcard base/*.asm) $(wildcard base/*.c) $(wildcard console/*.asm) $(wildcard console/*.c) $(wildcard disk/*.asm) $(wildcard disk/*.c)
OBJ = $(patsubst %.asm,%.o,$(patsubst %.c,%.o,$(SRC)))

all: $(OUT)
	@echo >> /dev/null

clean:
	@rm -f $(OUT) *.o base/*.o console/*.o disk/*.o

%.o: %.asm Makefile
	@nasm -f elf32 -o $@ $<

%.o: %.c Makefile $(wildcard ../include/*)
	@smlrcc -flat32 -SI ../include/ -c -o $@ $<

%.kern: $(OBJ)
	@smlrcc -flat32 -nobss -origin 0x10000 -o $@ $^ ../library/libhcix.a
