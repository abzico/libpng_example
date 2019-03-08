/// libpng demo
/// Wasin Thonkaew, abzi.co
///
#include <stdio.h>
#include <stdlib.h>
#include "test-libpng.h"

// define debugging information to spit out from libpng
// higher number, more info out
// valid value is 0 to 3
#define PNG_DEBUG 3

void abort_(const char* s, ...)
{
  va_list args;
  va_start(args, s);
  vfprintf(stderr, s, args);
  fprintf(stderr, "\n");
  va_end(args);
  abort();
}

void free_image_data(png_bytepp data, int height)
{
	for (int y=0; y<height; ++y)
	{
		free(data[y]);
		data[y] = NULL;
	}
	free(data);
}

bool write_ppm_p3_file(const char* file_name, const png_bytepp data, int rowbytes, int width, int height)
{
	FILE* fp = fopen(file_name, "wb");
	if (fp == NULL)
	{
		fprintf(stderr, "%s file cannot be opened for writing\n", file_name);
		return false;
	}

	// convert to opaque pointer as we will access arbitrary byte ourselves
	void** cdata = (void**)data;

	// header of .ppm P3 file
	fprintf(fp, "P3\n%d %d\n%d\n", width, height, 255);
	for (int y=0; y<height; ++y)
	{
		for (int x=0; x<width; ++x)
		{
			// grab packed color value
			unsigned int packed_color = *(unsigned int*)(cdata[y] + x*sizeof(unsigned int));
			// remember handle it in little-endian
			fprintf(fp, "%d %d %d\n", packed_color & 0xFF, packed_color >> 8 & 0xFF, packed_color >> 16 & 0xFF);
		}
		fprintf(fp, "\n");
	}

	// close file
	fclose(fp);
	fp = NULL;

	return true;
}

bool write_ppm_p6_file(const char* file_name, const png_bytepp data, int rowbytes, int width, int height)
{	
  FILE* fp = fopen(file_name, "wb");
	if (fp == NULL)
	{
		fprintf(stderr, "%s file cannot be opened for writing\n", file_name);
		return false;
	}

	// convert to opaque pointer as we will access arbitrary byte ourselves
	void** cdata = (void**)data;

	// header of .ppm P6 file
	fprintf(fp, "P6\n%d %d\n%d\n", width, height, 255);
	for (int y=0; y<height; ++y)
	{
		for (int x=0; x<width; ++x)
		{
			// grab packed color value
			unsigned int packed_color = *(unsigned int*)(cdata[y] + x*sizeof(unsigned int));
			// remember handle it in little-endian
      // png stores image data in MSB (most significant bit or big endian) as well as ppm
      fprintf(fp, "%c%c%c", packed_color & 0xFF, packed_color >> 8 & 0xFF, packed_color >> 16 & 0xFF);
		}
	}

	// close file
	fclose(fp);
	fp = NULL;

	return true;

}

png_bytepp read_png_file(const char* file_name, int* rst_rowbytes, int* rst_width, int* rst_height)
{
	// to hold first 8 bytes reading from png file to check if it's png file
  unsigned char header[8];

	// open file
	// note: libpng tells us to make sure we open in binary mode
  FILE *fp = fopen(file_name, "rb");
  if (fp == NULL)
    abort_("%s file cannot be opened for reading", file_name);
  
	// read the first 8 bytes (more bytes will make it more accurate)
	// but in case we want to keep our file pointer around after this
	// libpng suggests us to only read 8 bytes
	const int cmp_number = 8;
  if (fread(header, 1, cmp_number, fp) != cmp_number)
  {
		fprintf(stderr, "read %s file error", file_name);
		
		// close file
		fclose(fp);
		fp = NULL;
		return NULL;
  }
  
	// check whether magic number matches, and thus it's png file
	if (png_sig_cmp(header, 0, cmp_number) != 0)
	{
		// it's not PNG file
		fprintf(stderr, "%s file is not recognized as png file\n", file_name);
		
		// close file
		fclose(fp);
		fp = NULL;
		return NULL;
	}

	// create png structure
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL)
	{
		// cannot create png structure
		fprintf(stderr, "cannot create png structure\n");

		// close file
		fclose(fp);
		fp = NULL;
		return NULL;
	}

	// create png-info structure
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
		fprintf(stderr, "cannot create png info structure\n");

		// clear png resource
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		
		// close file
		fclose(fp);
		fp = NULL;
		return NULL;
	}

  // we need to set jump callback to handle error when we enter into a new routing before the call to png_*()
  // defined as convenient for future if you every call this in different routine
  // note: if use, need to call in routine that return any pointer type
#define PNG_READ_SETJMP(png_ptr, info_ptr, fp) \
	/* set jmp */ \
	if (setjmp(png_jmpbuf(png_ptr)))  \
	{ \
		fprintf(stderr, "error png's set jmp for read\n"); \
                                              \
		/* clear png resource */                  \
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);   \
                                                                      \
		/* close file */ \
		fclose(fp);     \
		fp = NULL;      \
		return NULL;    \
	}

  // call this once as all relevant operations all happen in just one routine
  PNG_READ_SETJMP(png_ptr, info_ptr, fp)

	// set up input code
	png_init_io(png_ptr, fp);
	// let libpng knows that we have read first initial bytes to check whether it's png file
	// thus libpng knows some bytes are missing
	png_set_sig_bytes(png_ptr, cmp_number);

	// read file info up to image data
	png_read_info(png_ptr, info_ptr);
	
	// get info of png image
	int width = png_get_image_width(png_ptr, info_ptr);
	int height = png_get_image_height(png_ptr, info_ptr);
	int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	int color_type = png_get_color_type(png_ptr, info_ptr);
	int interlace_type = png_get_interlace_type(png_ptr, info_ptr);
	int channels = png_get_channels(png_ptr, info_ptr);
	int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

	printf("width = %d\n", width);
	printf("height = %d\n", height);
	printf("bit_depth = %d\n", bit_depth);
	switch (color_type)
	{
		case PNG_COLOR_TYPE_GRAY:
			printf("color type = 'PNG_COLOR_TYPE_GRAY' (bit depths 1, 2, 4, 8, 16)\n");
			break;
		case PNG_COLOR_TYPE_GRAY_ALPHA:
			printf("color type = 'PNG_COLOR_TYPE_GRAY_ALPHA' (bit depths 8, 16)\n");
			break;
		case PNG_COLOR_TYPE_PALETTE:
			printf("color type = 'PNG_COLOR_TYPE_PALETTE' (bit depths 1, 2, 4, 8)\n");
			break;
		case PNG_COLOR_TYPE_RGB:
			printf("color type = 'PNG_COLOR_TYPE_RGB' (bit depths 8, 16)\n");
			break;
		case PNG_COLOR_TYPE_RGB_ALPHA:
			printf("color type = 'PNG_COLOR_TYPE_RGB_ALPHA' (bit depths 8, 16)\n");
			break;
	}
	switch (interlace_type)
	{
		case PNG_INTERLACE_NONE:
			printf("interlace type = none\n");
			break;
		case PNG_INTERLACE_ADAM7:
			printf("interlace type = ADAM7\n");
			break;
	}
	switch (channels)
	{
		case 1:
			printf("channels = %d (GRAY, PALETTE)\n", channels);
			break;
		case 2:
			printf("channels = %d (GRAY_ALPHA)\n", channels);
			break;
		case 3:
			printf("channels = %d (RGB)\n", channels);
			break;
		case 4:
			printf("channels = %d (RGB_ALPHA or RGB + filter byte)\n", channels);
			break;
	}
	printf("rowbytes = %d\n", rowbytes);

	// allocate enough and continous memory space to whole entire image
	// note: i think we could allocate continous memory space that result in just png_bytep
	// but for some reason it might due to internal libpng's internal implementation that possibly
	// needs some flexibility in row by row pointer, thus we need to allocate memory space this way
	png_bytepp row_ptr = (png_bytepp)malloc(sizeof(png_bytep) * height);
	for (int y=0; y<height; ++y)
	{
		row_ptr[y] = (png_bytep)malloc(rowbytes);
	}
	// read image data
	png_read_image(png_ptr, row_ptr);

	// clear png resource
	png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);

	// close file
	fclose(fp);
	fp = NULL;

	// return results
	if (rst_rowbytes != NULL)
	{
		*rst_rowbytes = rowbytes;
	}
	if (rst_width != NULL)
	{
		*rst_width = width;
	}
	if (rst_height != NULL)
	{
		*rst_height = height;
	}

	return row_ptr;
}

int main (int argc, char* argv[])
{
	int width = 0, height = 0;
	int rowbytes = 0;

	// read image file
	png_bytepp image_data = read_png_file("opaque.png", &rowbytes, &width, &height);
	// prove of concept, write into .ppm (P3) file
	const char* output_p3_file = "opaque-p3.ppm";
	const char* output_p6_file = "opaque-p6.ppm";
  // P3 format
	if (!write_ppm_p3_file(output_p3_file, image_data, rowbytes, width, height))
	{
		fprintf(stderr, "error writing into .ppm (P3 format) file\n");
	}
	else
	{
		fprintf(stdout, "Check output %s file\n", output_p3_file);
	}

  // P6 format
	if (!write_ppm_p6_file(output_p6_file, image_data, rowbytes, width, height))
	{
		fprintf(stderr, "error writing into .ppm (P6 format) file\n");
	}
	else
	{
		fprintf(stdout, "Check output %s file\n", output_p6_file);
	}

	// done with it, free image data memory space
	free_image_data(image_data, height);
	image_data = NULL;

	return 0;
}
