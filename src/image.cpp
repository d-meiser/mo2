#include <image.h>
#include <utilities.h>
#include <stdio.h>
#include <jpeglib.h>
#include <stdlib.h>
#include <stdexcept>


namespace Mo {

Image::Image(const std::string& filename) : quality_(90) {
  readJpegFile(filename.c_str());
}

Image::Image(int width, int height) :
  width_(width), height_(height), pitch_(3 * width), numComponents_(3),
  quality_(90), pixelData_(pitch_ * height_) {
}

Image::~Image() {}

void Image::save(const std::string& filename) {
  saveJpegFile(filename.c_str());
}

unsigned char *Image::getPixelData() {
  if (pixelData_.empty()) {
    return nullptr;
  } else {
    return &pixelData_[0];
  }
}

const unsigned char *Image::getConstPixelData() const {
  if (pixelData_.empty()) {
    return nullptr;
  } else {
    return &pixelData_[0];
  }
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

void Image::setQuality(int quality) {
  quality_ = quality;
}

int Image::quality() const {
  return quality_;
}

bool Image::operator==(const Image& rhs) const {
  if (numComponents_ != rhs.numComponents_) {
    return false;
  }
  if (width_ != rhs.width_) {
    return false;
  }
  if (height_ != rhs.height_) {
    return false;
  }
  double l1Difference = 0;
  for (int i = 0; i != height_; ++i) {
    for (int j = 0; j != width_; ++j) {
      for (int k = 0; k < numComponents_; ++k) {
        int difference =
          (int)pixelData_[(i * width_ + j) * numComponents_ + k] - 
          (int)rhs.pixelData_[(i * width_ + j) * numComponents_ + k];
        l1Difference += std::abs(difference);
      }
    }
  }
  double meanError = l1Difference / (255 * numComponents_ * width_ * height_);
  static const double tolerance = 1.0e-2;
  return meanError < tolerance ;
}

void Image::readJpegFile(const char *filename) {
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  JSAMPROW row_pointer[1];

  FILE *infile = fopen( filename, "rb");

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
  unsigned long location = 0;
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
  jpeg_set_quality(&cinfo, quality_, TRUE);
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

