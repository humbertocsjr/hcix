OUT = ../libhcix.a
SRC = $(wildcard *.asm) $(wildcard *.c)
OBJ = $(patsubst %.asm,%.o,$(patsubst %.c,%.o,$(SRC)))

all: $(OUT)
	@echo >> /dev/null

clean:
	@rm -f $(OUT) *.o

%.o: %.asm Makefile
	@nasm -f elf32 -o $@ $<

%.o: %.c Makefile
	@smlrcc -flat32 -SI ../../include/ -c -o $@ $<

%.a: $(OBJ)
	@ar r $@ $^
