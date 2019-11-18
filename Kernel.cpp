#include "Kernel.h"

Kernel::Kernel(int size): size_(size)
{
	w_.resize(size);
	for (int i = 0; i < size; i++) {
		w_[i].resize(size);
	}
}

std::vector<int> Kernel::Apply(image_data& data, int lineElem, int columnElem)
{
	int x, y;
	
	std::vector<int> result;
	result.resize(3);


	y = lineElem - size_ / 2;
	
	for (int i = 0; i < size_; i++) {
		x = columnElem - size_ / 2;
		for (int j = 0; j < size_; j++) {
			if (x >= 0 && y >= 0 && x < data.w && y < data.h) {
				result[0] += data.pixels[y * data.w * data.compPerPixel + x * data.compPerPixel] * w_[i][j];
				result[1] += data.pixels[y * data.w * data.compPerPixel + x * data.compPerPixel + 1] * w_[i][j];
				result[2] += data.pixels[y * data.w * data.compPerPixel + x * data.compPerPixel + 2] * w_[i][j];
			}
			x++;
		}
		y++;
	}

	return result;
}

EdgeKernel::EdgeKernel(int size) : Kernel(size)
{
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (i == size / 2 && j == size / 2)
				w_[i][j] = size * size;
			else
				w_[i][j] = -1;
		}
	}
}

BlurKernel::BlurKernel(int size) : Kernel(size)
{
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			w_[i][j] = 1;
		}
	}
}
