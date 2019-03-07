CC := gcc
OUT := test-libpng
CFLAGS = -std=c99
LFLAGS = -lpng

DEPS := \
	test-libpng.o

.PHONY: all clean

all: $(DEPS)

test-libpng.o: test-libpng.c
	$(CC) $< $(CFLAGS) $(LFLAGS) -o $(OUT)

clean:
	rm -f *.o
	rm -f $(OUT)
