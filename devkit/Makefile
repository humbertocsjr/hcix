OUT = installboot

all: $(OUT)
	@echo >> /dev/null
clean:
	@rm -f $(OUT)

%: %.c Makefile
	@cc -o $@ $<