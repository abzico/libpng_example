CC := gcc
OUT := test-libpng
CFLAGS = -std=c99 -Wall -I.
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
	rm -f opaque-p3.ppm
	rm -f opaque-p6.ppm
	rm -f opaque-png.png
	rm -f trans-png.png
	rm -f manual-png.png
	rm -f manual-png2.png
