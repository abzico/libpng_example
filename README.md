# libpng_example

Sample project using libpng to read / write file with libpng in C.

Make sure you have installed libpng on your machine.

# Build and Run

Build the project with `make`, then it will produces `test-libpng` executable file.

Run it, the program will read input png file `opaque.png` as well as its info header, then use read-image data to write into .ppm (P3 as `opaque-p3.ppm`, and P6 as `opaque-p6.ppm`) files to prove the concept. You should see the same visual of image.

Reading png image that has real transparent will be added later.

# Note

There are 2 input images used

1. `opague.png`
2. `trans.png`

# License
MIT, ABZI (abzi.co)
