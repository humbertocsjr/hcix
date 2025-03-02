OUT = installboot pfstool

all: $(OUT)
	@echo >> /dev/null
clean:
	@rm -f $(OUT)

pfstool: $(wildcard patuafs/*.h) $(wildcard patuafs/*.c)
	@cc -o $@ $^

%: %.c Makefile
	@cc -o $@ $<