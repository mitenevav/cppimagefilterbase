#pragma once
#include <vector>
#include "png_toolkit.h"

class Kernel {
public:
	Kernel(int size = 3);
	std::vector<int> Apply(image_data& data, int lineElem, int columnElem);
protected:
	std::vector<std::vector<int>> w_;
	int size_;
};

class EdgeKernel : public Kernel {
public:
	EdgeKernel(int size = 3);
};

class BlurKernel : public Kernel {
public:
	BlurKernel(int size = 3);
};