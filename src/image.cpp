#include <image.h>
#include <utilities.h>
#include <stdio.h>
#include <jpeglib.h>
#include <stdlib.h>
#include <stdexcept>


namespace Mo {

Image::Image(const char* filename) {
  readJpegFile(filename);
}

Image::~Image() {}

void Image::save(const char* filename) {
  saveJpegFile(filename);
}

int Image::width() const {
  return width_;
}

int Image::height() const {
  return height_;
}

int Image::pitch() const {
  return pitch_;
}

int Image::numComponents() const {
  return numComponents_;
}

void Image::readJpegFile(const char *filename) {
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  JSAMPROW row_pointer[1];

  FILE *infile = fopen( filename, "rb");
  unsigned long location = 0;

  if (!infile) {
    throw std::runtime_error(std::string("Error opening jpeg file ") + filename);
  }

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, infile);
  jpeg_read_header(&cinfo, TRUE);
  jpeg_start_decompress(&cinfo);

  width_ = cinfo.image_width;
  height_ = cinfo.image_height;
  pitch_ = cinfo.image_width * cinfo.num_components;
  numComponents_ = cinfo.num_components;

  pixelData_.resize(cinfo.output_width *
                    cinfo.output_height * cinfo.num_components);
  row_pointer[0] = (unsigned char *)malloc(
      cinfo.output_width * cinfo.num_components);
  while (cinfo.output_scanline < cinfo.image_height) {
    jpeg_read_scanlines(&cinfo, row_pointer, 1);
    for (unsigned int i = 0;
         i < cinfo.image_width * cinfo.num_components; ++i) {
      pixelData_[location++] = row_pointer[0][i];
    }
  }
  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  free(row_pointer[0]);
  fclose(infile);
}

void Image::saveJpegFile(const char *filename) {
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;

  JSAMPROW row_pointer[1];
  FILE *outfile = fopen( filename, "wb" );

  if (!outfile) {
    throw std::runtime_error(std::string("Failed to open file ") + filename);
  }
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);
  jpeg_stdio_dest(&cinfo, outfile);

  cinfo.image_width = width_;
  cinfo.image_height = height_;
  cinfo.input_components = numComponents_;
  cinfo.in_color_space = JCS_RGB;

  jpeg_set_defaults(&cinfo);
  cinfo.dct_method = JDCT_FLOAT;
  jpeg_set_quality(&cinfo, 15, TRUE);
  jpeg_start_compress( &cinfo, TRUE );
  while (cinfo.next_scanline < cinfo.image_height) {
    row_pointer[0] = &pixelData_[cinfo.next_scanline *
                                 cinfo.image_width * cinfo.input_components];
    jpeg_write_scanlines(&cinfo, row_pointer, 1);
  }
  jpeg_finish_compress(&cinfo);
  jpeg_destroy_compress(&cinfo);
  fclose(outfile);
}

} // namespace Mo

