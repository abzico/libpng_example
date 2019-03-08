#ifndef TEST_LIBPNG_H_
#define TEST_LIBPNG_H_

#include <stdbool.h>
#include <stdarg.h>
#include <png.h>

///
/// Print text to stderr then abort.
///
/// \param s text to print on stderr
///
extern void abort_(const char* s, ...);

///
/// Free image data pointer as loaded from libpng.
/// User should supply a proper full height of such image as previously loaded
/// in order to free whole image properly.
///
/// \param data image data pointer
/// \param height height of image
///
extern void free_image_data(png_bytepp data, int height);

///
/// write into .ppm file as P3 format (see http://netpbm.sourceforge.net/doc/ppm.html)
/// take image data `data` then write into .ppm file `file_name`
/// note: used for testing with png with fully opaque, if such image has transparency it might have weird result
///
/// \param file_name file name of output file to write into
/// \param data image data pointer to read from then write into output file, this will ignore its alpha component
/// \param rowbytes total bytes per row of image. This information can be get from reading png's info struct.
/// \param width width of image
/// \param height height of image
/// \return true if writing is successful, otherwise returned false for failure
///
extern bool write_ppm_p3_file(const char* file_name, const png_bytepp data, int rowbytes, int width, int height);

///
/// write into .ppm file as P6 format (see http://netpbm.sourceforge.net/doc/ppm.html)
/// take image data `data` then write into .ppm file `file_name`
/// note: used for testing with png with fully opaque, if such image has transparency it might have weird result
///
/// \param file_name file name of output file to write into
/// \param data image data pointer to read from then write into output file, this will ignore its alpha component
/// \param rowbytes total bytes per row of image. This information can be get from reading png's info struct.
/// \param width width of image
/// \param height height of image
/// \return true if writing is successful, otherwise returned false for failure
///
extern bool write_ppm_p6_file(const char* file_name, const png_bytepp data, int rowbytes, int width, int height);

///
/// read png (RGBA format) file both for its info, and image data, then return image data out via `read_image_data`
/// returned pointer to image data needs to be freed with free_image_data() function.
///
/// \param file_name image file name to read
/// \param rst_rowbytes to be filled in for image's total bytes in each row
/// \param rst_width to be filled in for image's width
/// \param rst_height to be filled in for image's height
/// \return pointer to image's data if successfully read, otherwise NULL is returned.
///
extern png_bytepp read_png_file(const char* file_name, int* rst_rowbytes, int* rst_width, int* rst_height);

///
/// write png (RGBA format) file into output `file_name`.
///
/// \param file_name output file name to write into
/// \param data image data
/// \param width width of image
/// \param height height of image
/// \return true if writing is successful, otherwise return false
///
extern bool write_png_file(const char* file_name, const png_bytepp data, int width, int height);

///
/// generate colored image data in RGBA format with specified color for all pixel as width x height image but alpha is 0xFF.
///
/// \param width width of image data to generate
/// \param height height of image data to generate
/// \param r red color component value
/// \param g green color component value
/// \param b blue color component value
/// \return image data
///
extern png_bytepp generate_color_image(int width, int height, unsigned char r, unsigned char g, unsigned char b);

///
/// generate colored image data in RGBA format with specified color for all pixel as width x height image; with margin as size to
/// render as full transparent.
///
/// \param width width of image data to generate
/// \param height height of image data to generate
/// \param margin margin to render as full transparent around the edge of the image
/// \param r red color component value
/// \param g green color component value
/// \param b blue color component value
/// \return image data
///
extern png_bytepp generate_color_imagea(int width, int height, int margin, unsigned char r, unsigned char g, unsigned char b);

#endif
