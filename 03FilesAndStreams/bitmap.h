#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <stdint.h>

// BMP format assumes 16bit machines
#pragma pack(push, 2) // Element must start on 16 bit intervals
struct bitmap_file_header {
	char header[2]{ 'B', 'M' };
	int32_t file_size;
	int32_t reserved{ 0 };
	int32_t data_offset;
};
#pragma pack(pop) // Revert the alignment to default

struct bitmap_info_header {
	int32_t header_size{ 40 };
	int32_t width;
	int32_t height;
	int16_t planes{ 1 };
	int16_t bits_per_pixel{ 24 };
	int32_t compression{ 0 };
	int32_t data_size{ 32 };
	int32_t horizontal_resolution{ 2400 };
	int32_t vertical_resolution{ 2400 };
	int32_t colours{ 0 };
	int32_t important_colours{ 0 };
};

struct pixel {
	int8_t blue{ 0 };
	int8_t green{ 0 };
	int8_t red{ 0 };
};

class bitmap {
private:
	int width{ 800 };
	int height{ 600 };
	std::string filename;
	std::vector<pixel> pixels;

	// RAII
	std::ofstream oFile;
public:
	bitmap(std::string filename) : filename(filename), pixels(width * height)
	{
		oFile.open(filename, std::ios_base::out | std::ios_base::binary);
		if (oFile.is_open()) {
			writeHeaders();
		}
	}

	~bitmap()
	{
		oFile.close();
	}

	operator bool()
	{
		return oFile.is_open();
	}

	void clearColor(int r, int g, int b)
	{
		for (auto& pixel : pixels)
		{
			pixel.red = r;
			pixel.green = g;
			pixel.blue = b;
		}
	}

	void drawRect(int x, int y, int w, int h)
	{
		for (size_t j = y; j < height && j < y + h; ++j)
		{
			for (size_t i = x; i < width && i < x + w; ++i)
			{
				size_t index = j * width + i;
				pixel& pixel = pixels[index];
				pixel.blue = 0xff;
				pixel.green = 0xff;
				pixel.red = 0xff;
			}
		}
	}

	void write()
	{
		oFile.write(reinterpret_cast<char*>(pixels.data()), (std::streamsize)pixels.size() * sizeof(pixel));
	}
private:
	void writeHeaders()
	{
		// Setup the info header
		bitmap_info_header infoHeader{};
		infoHeader.width = width;
		infoHeader.height = height;

		// Setup the file header
		bitmap_file_header fileHeader{};
		fileHeader.data_offset = sizeof(bitmap_file_header) + infoHeader.header_size;
		fileHeader.file_size = fileHeader.data_offset + infoHeader.bits_per_pixel * width * height;

		// Write file header
		oFile.write(reinterpret_cast<char*>(&fileHeader), sizeof(bitmap_file_header));
		
		// Write info header
		oFile.write(reinterpret_cast<char*>(&infoHeader), sizeof(bitmap_info_header));
	}
};