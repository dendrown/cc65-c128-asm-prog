TARGET := sprite
STARTADDR := 4864

CONFIG := c128-asm.cfg
ALL_ASM := $(wildcard *.asm) $(wildcard *.inc)

all: $(ALL_ASM)
	cl65 -C $(CONFIG) --start-addr $(STARTADDR) -o $(TARGET) -l $(TARGET).lst $<
	@ls -l $(TARGET)

clean:
	rm -f $(TARGET) *.lst *.o
