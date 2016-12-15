#ifndef _TGA_H
#define _TGA_H

#include <stdint.h>
#include <memory>
#include <string>

namespace GFX {

typedef struct {
	uint8_t header[6];
	unsigned int size;             // Amount Of Memory Needed To Hold The Image
	unsigned int h;                // Height Of Image                 
	unsigned int w;                // Width Of Image              
	unsigned int bpp;              // Number Of BITS Per Pixel (24 Or 32)
	std::shared_ptr<uint8_t> data; // Pixels
} TGA;

std::shared_ptr<TGA> loadTGA(const std::string &file);

}

#endif

