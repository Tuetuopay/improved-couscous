/**
 * The MIT License (MIT)
 * Copyright (c) 2016 Tuetuopay
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 **/

#include "TGA.h"

#include <stdio.h>
#include <string.h>

// #define SWAP_BGR_TO_RGB

namespace GFX {

bool loadUncompressedTGA(FILE *fi, std::shared_ptr<TGA> tga) {
	// Read the image data
	if (fread(tga->data.get(), 1, tga->size, fi) != tga->size) return false;

#ifdef SWAP_BGR_TO_RGB
	unsigned int bytespp = tga->bpp >> 3;
	// Swap : BGR -> RGB
	for (unsigned int i = 0; i < tga->size; i += bytespp >> 3) {
		// According to NeHe, if you XOR the two values 3 times, you swap them o_O, and it is
		// faster than a temp variable
		tga->data.get()[i] ^= tga->data.get()[i + 2] ^= tga->data.get()[i] ^= tga->data.get()[i + 1];
	}
#endif

	return true;
}
bool loadCompressedTGA(FILE *fi, std::shared_ptr<TGA> tga) {
	unsigned int bytespp = tga->bpp >> 3;

	uint8_t *pixel = new uint8_t[bytespp];
	if (!pixel) return false;

	unsigned int curPixel = 0;
	uint8_t header;

	do {
		// Read chunk header
		if (!fread(&header, 1, 1, fi)) {
			delete[] pixel;
			return false;
		}

		if (header < 128) { // Raw chunk
			header++;
			// Read chunk
			if (fread(tga->data.get() + curPixel * bytespp, 1, header * bytespp, fi) !=
			    header * bytespp) {
				delete[] pixel;
				return false;
			}
#ifdef SWAP_BGR_TO_RGB
			// Swap R and B
			for (unsigned int i = curPixel*bytespp; i < (curPixel+header)*bytespp; i += bytespp) {
				tga->data.get()[i] ^= tga->data.get()[i + 2] ^=
					tga->data.get()[i] ^= tga->data.get()[i + 1];
			}
#endif
		}
		else { // RLE chunk
			header -= 127; // Get rid of RLE bit
			// Read a single pixel
			if (fread(pixel, 1, bytespp, fi) != bytespp) {
				delete[] pixel;
				return false;
			}
			// Fill the pixels
			for (unsigned int i = curPixel*bytespp; i < (curPixel+header)*bytespp; i += bytespp) {
#if SWAP_BGR_TO_RGB
				tga->data.get()[i + 0] = pixel[2];
				tga->data.get()[i + 2] = pixel[0];
#else
				tga->data.get()[i + 0] = pixel[0];
				tga->data.get()[i + 2] = pixel[2];
#endif
				tga->data.get()[i + 1] = pixel[1];
				if (bytespp == 4) tga->data.get()[i + 3] = pixel[3];
			}
		}
		curPixel += header;
	} while (curPixel < tga->size);

	return true;
}

std::shared_ptr<TGA> loadTGA(const std::string &file) {
	uint8_t tgaCompU[] = {0,0, 2,0,0,0,0,0,0,0,0,0},
	        tgaCompC[] = {0,0,10,0,0,0,0,0,0,0,0,0};
	std::shared_ptr<TGA> tga(new TGA);
	bool ret = false;

	FILE *fi = NULL;
	FILE *ftest = NULL;
	if (!(fi = fopen(file.c_str(), "rb"))) return nullptr;

	// Read TGA header
	uint8_t header[12];
	if (fread(header, 1, 12, fi) != 12) goto end;

	// Header
	if (fread(tga->header, 1, sizeof(tga->header), fi) != sizeof(tga->header)) goto end;

	// Compute stuff
	tga->w = tga->header[1] * 0x100 + tga->header[0];
	tga->h = tga->header[3] * 0x100 + tga->header[2];
	tga->bpp = tga->header[4];

	// Check that this is correct
	if (tga->w == 0 || tga->h == 0 || (tga->bpp != 32 && tga->bpp != 24)) goto end;

	tga->size = tga->w * tga->h * (tga->bpp >> 3); // >> 3 <=> / 8
	tga->data = std::shared_ptr<uint8_t>(new uint8_t[tga->size]);
	if (tga->data == nullptr) goto end;

	// Load the TGA
	if      (memcmp(header, tgaCompU, 12) == 0) ret = loadUncompressedTGA(fi, tga);
	else if (memcmp(header, tgaCompC, 12) == 0) ret = loadCompressedTGA(fi, tga);

end:
	fclose(fi);
	return ret ? tga : nullptr;
}

}

