#include "Filters.h"
#include "Kernel.h"
#include <vector>
#include <array>
#include <algorithm>


void RedFilter::Apply(image_data& imgData)
{
	int firstLine, firstColumn, lastLine, lastColumn;

	filterBorders_.left_ == 0 ? firstColumn = 0 : firstColumn = imgData.w / filterBorders_.left_;
	filterBorders_.right_ == 0 ? lastColumn = 0 : lastColumn = imgData.w / filterBorders_.right_;

	filterBorders_.up_ == 0 ? firstLine = 0 : firstLine = imgData.h / filterBorders_.up_;
	filterBorders_.down_ == 0 ? lastLine = 0 : lastLine = imgData.h / filterBorders_.down_;

	for (int i = firstLine; i < lastLine; i++) {
		int k = i * imgData.w * imgData.compPerPixel;

		for (int j = firstColumn; j < lastColumn; j++) {
			int x = k + j * imgData.compPerPixel;
			imgData.pixels[x] = 255;
			imgData.pixels[x + 1] = 0;
			imgData.pixels[x + 2] = 0;
		}

	}
}

void BlWhFilter::Apply(image_data& imgData)
{
	int firstLine, firstColumn, lastLine, lastColumn;

	filterBorders_.left_ == 0 ? firstColumn = 0 : firstColumn = imgData.w / filterBorders_.left_;
	filterBorders_.right_ == 0 ? lastColumn = 0 : lastColumn = imgData.w / filterBorders_.right_;

	filterBorders_.up_ == 0 ? firstLine = 0 : firstLine = imgData.h / filterBorders_.up_;
	filterBorders_.down_ == 0 ? lastLine = 0 : lastLine = imgData.h / filterBorders_.down_;

	for (int i = firstLine; i < lastLine; i++) {
		int k = i * imgData.w * imgData.compPerPixel;

		for (int j = firstColumn; j < lastColumn; j++) {
			int x = k + j * imgData.compPerPixel;

			int val = imgData.pixels[x] * 0.3 + imgData.pixels[x + 1] * 0.6 + imgData.pixels[x + 2] * 0.1;

			imgData.pixels[x] = val;
			imgData.pixels[x + 1] = val;
			imgData.pixels[x + 2] = val;

			/*
			int sum=0;

			for (int count = 0; count < 3; count++) {
				sum += imgData.pixels[x + count];
			}

			sum /= 3;

			for (int count = 0; count < 3; count++) {
				imgData.pixels[x + count] = sum;
			}
			*/

		}
	}
}

void ThresholdFilter::Apply(image_data& imgData)
{

	int firstLine, firstColumn, lastLine, lastColumn;

	filterBorders_.left_ == 0 ? firstColumn = 0 : firstColumn = imgData.w / filterBorders_.left_;
	filterBorders_.right_ == 0 ? lastColumn = 0 : lastColumn = imgData.w / filterBorders_.right_;

	filterBorders_.up_ == 0 ? firstLine = 0 : firstLine = imgData.h / filterBorders_.up_;
	filterBorders_.down_ == 0 ? lastLine = 0 : lastLine = imgData.h / filterBorders_.down_;


	BlWhFilter bwFilter(filterBorders_);
	bwFilter.Apply(imgData);


	std::vector<int> buff;

	for (int i = firstLine; i < lastLine; i += size_) {
		for (int j = firstColumn; j < lastColumn; j += size_) {

			for (int line = 0; line < size_; line++) {
				for (int column = 0; column < size_; column++) {
					if (((i + line) < lastLine) && ((j + column) < lastColumn)) {
						int x = ((i + line) * imgData.w + (j + column)) * imgData.compPerPixel;
						buff.push_back(imgData.pixels[x]);
					}
					else {
						buff.push_back(0);
					}
				}
			}

			std::sort(buff.begin(), buff.end());

			int med = buff[size_ * size_ / 2];
			int update = 0;


			for (int line = 0; line < size_ && update < size_ * size_ / 2; line++) {
				for (int column = 0; column < size_ && update < size_ * size_ / 2; column++) {

					if (((i + line) < lastLine) && ((j + column) < lastColumn)) {
						int x = ((i + line) * imgData.w + (j + column)) * imgData.compPerPixel;

						if (imgData.pixels[x] < med) {
							for (int k = 0; k < 3; k++) {
								imgData.pixels[x + k] = 0;
							}
							update++;
						}
					}
					else {
						update++;
					}
				}
			}
			for (int line = 0; line < size_ && update < size_ * size_ / 2; line++) {
				for (int column = 0; column < size_ && update < size_ * size_ / 2; column++) {

					if (((i + line) < lastLine) && ((j + column) < lastColumn)) {
						int x = ((i + line) * imgData.w + (j + column)) * imgData.compPerPixel;

						if (imgData.pixels[x] == med) {
							for (int k = 0; k < 3; k++) {
								imgData.pixels[x + k] = 0;
							}
							update++;
						}
					}
				}
			}

			buff.clear();
		}
	}
}

void EdgeFilter::Apply(image_data& imgData)
{
	int firstLine, firstColumn, lastLine, lastColumn;

	filterBorders_.left_ == 0 ? firstColumn = 0 : firstColumn = imgData.w / filterBorders_.left_;
	filterBorders_.right_ == 0 ? lastColumn = 0 : lastColumn = imgData.w / filterBorders_.right_;

	filterBorders_.up_ == 0 ? firstLine = 0 : firstLine = imgData.h / filterBorders_.up_;
	filterBorders_.down_ == 0 ? lastLine = 0 : lastLine = imgData.h / filterBorders_.down_;


	BlWhFilter bwFilter(filterBorders_);
	bwFilter.Apply(imgData);


	image_data area;
	area.compPerPixel = imgData.compPerPixel;
	area.h = lastLine - firstLine;
	area.w = lastColumn - firstColumn;
	area.pixels = new stbi_uc[(lastLine - firstLine) * (lastColumn - firstColumn) * imgData.compPerPixel];

	int numElem = 0;
	for (int i = firstLine; i < lastLine; i++) {
		int y = i * imgData.w * imgData.compPerPixel;
		for (int j = firstColumn; j < lastColumn; j++) {
			int x = y + j * imgData.compPerPixel;
			for (int k = 0; k < imgData.compPerPixel; k++) {
				area.pixels[numElem] = imgData.pixels[x + k];
				numElem++;
			}
		}
	}

	EdgeKernel ker(size_);

	for (int i = 0; i < area.h; i++) {
		for (int j = 0; j < area.w; j++) {
			std::vector<int> res;
			int x = ((firstLine + i) * imgData.w + (firstColumn + j)) * imgData.compPerPixel;
			res = ker.Apply(area, i, j);
			for (int k = 0; k < res.size(); k++) {
				if (res[k] > 255)
					res[k] = 255;
				else if (res[k] < 0)
					res[k] = 0;

				imgData.pixels[x + k] = res[k];
			}
		}
	}

	delete[] area.pixels;
}

void BlurFilter::Apply(image_data& imgData)
{
	int firstLine, firstColumn, lastLine, lastColumn;

	filterBorders_.left_ == 0 ? firstColumn = 0 : firstColumn = imgData.w / filterBorders_.left_;
	filterBorders_.right_ == 0 ? lastColumn = 0 : lastColumn = imgData.w / filterBorders_.right_;

	filterBorders_.up_ == 0 ? firstLine = 0 : firstLine = imgData.h / filterBorders_.up_;
	filterBorders_.down_ == 0 ? lastLine = 0 : lastLine = imgData.h / filterBorders_.down_;


	image_data area;
	area.compPerPixel = imgData.compPerPixel;
	area.h = lastLine - firstLine;
	area.w = lastColumn - firstColumn;
	area.pixels = new stbi_uc[(lastLine - firstLine) * (lastColumn - firstColumn) * imgData.compPerPixel];

	int numElem = 0;
	for (int i = firstLine; i < lastLine; i++) {
		int y = i * imgData.w * imgData.compPerPixel;
		for (int j = firstColumn; j < lastColumn; j++) {
			int x = y + j * imgData.compPerPixel;
			for (int k = 0; k < imgData.compPerPixel; k++) {
				area.pixels[numElem] = imgData.pixels[x + k];
				numElem++;
			}
		}
	}

	BlurKernel ker(size_);

	for (int i = 0; i < area.h; i++) {
		for (int j = 0; j < area.w; j++) {
			std::vector<int> res;
			int x = ((firstLine + i) * imgData.w + (firstColumn + j)) * imgData.compPerPixel;
			res = ker.Apply(area, i, j);
			for (int k = 0; k < res.size(); k++) {
				imgData.pixels[x + k] = res[k] / (size_*size_);
			}
		}
	}

	delete[] area.pixels;
}
