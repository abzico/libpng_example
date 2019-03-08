# libpng_example

Sample project using libpng to read / write / process png image file in C.

Make sure you have installed [libpng](http://www.libpng.org/pub/png/libpng.html) on your machine.

# Build and Run

Build the project with `make`, then it will produces `test-libpng` executable file.

Use `make clean` to clean generated image files and start over.

# How?

See overview image as follow on what to expect.

![image reference overview](https://github.com/abzico/libpng_example/blob/master/libpng_example_layout.png)

Program demonstrate the following points

1. **Read** input fully opaque .png i.e. `opaque.png` file then use such data to **write** into .ppm image format for both P3 and P6 (as prove of concept because PPM image format is very easy, and quick to work with to debug and validate pixel data we've read), and as well as .png file.
2. **Read** input .png with transparency `trans.png` file then use such data to **write** into .png file.
3. **Generate** fully opaque custom image data by setting pixel values then use it to write into .png file.
4. Same as 3 but generate custom image data with transparency.

# What Stuff Demonstrated in This Project?

* How to work with libpng to read/write and process (generate by hand) image data to produce .png image
* How to write into .ppm both P3 and P6 format (for purpose of prove of concept reading image data from .png file)

# Note

There are 2 input images used

1. `opague.png` - program will produce `opaque-p3.png`, `opaque-p6.png`, `opaque-png.png`
2. `trans.png` - program will produce `trans-png.png`
3. manual - program will produce `manual-png.png` and `manual-png2.png`. The former with full image opaque color, but the latter has edage with size 32 pixels around the image as full transparent leaving the center as opaque color.

PS. Tested on macOS, for Windows, should need a little bit more effort to make a proper build script.

# License
MIT, ABZI (abzi.co)
