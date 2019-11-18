#pragma once
#include "ImageData.h"
#include "Kernel.h"

struct FiltersBorder {
	int up_;
	int left_;
	int down_;
	int right_;

	FiltersBorder(int up, int left, int down, int right) : up_(up), left_(left), down_(down), right_(right) {};
	FiltersBorder() : up_(0), left_(0), down_(1), right_(1) {};
	FiltersBorder(FiltersBorder& fBorders) : up_(fBorders.up_), left_(fBorders.left_), down_(fBorders.down_), right_(fBorders.right_) {};
};

class Filter {
public:
	virtual ~Filter() {};
	virtual void Apply(image_data& imgData) = 0;


	Filter(int up, int left, int down, int right) : filterBorders_(up, left, down, right) {};
	Filter() : filterBorders_() {};
	Filter(FiltersBorder& fBorders) : filterBorders_(fBorders) {};
protected:
	FiltersBorder filterBorders_;
};

class ConvolutionFilter : public Filter {
protected:
	Kernel* ker;
	int size_;
public:
	virtual ~ConvolutionFilter() {};
	virtual void Apply(image_data& imgData) = 0;


	ConvolutionFilter(int up, int left, int down, int right, int size = 3) : Filter(up, left, down, right), size_(size) {};
	ConvolutionFilter(int size = 3) : Filter(), size_(size) {};
	ConvolutionFilter(FiltersBorder& fBorders, int size = 3) : Filter(fBorders), size_(size) {};
};

class RedFilter : public Filter {
public:
	void Apply(image_data& imgData);


	RedFilter(int up, int left, int down, int right) : Filter(up, left, down, right) {};
	RedFilter() : Filter() {};
	RedFilter(FiltersBorder& fBorders) : Filter(fBorders) {};
};

class ThresholdFilter : public Filter {
private:
	int size_;
public:
	void Apply(image_data& imgData);


	ThresholdFilter(int up, int left, int down, int right, int size = 5) : Filter(up, left, down, right), size_(size) {};
	ThresholdFilter(int size = 5) : Filter(), size_(size) {};
	ThresholdFilter(FiltersBorder& fBorders, int size = 5) : Filter(fBorders), size_(size) {};
};

class BlWhFilter : public Filter {
public:
	void Apply(image_data& imgData);


	BlWhFilter(int up, int left, int down, int right) : Filter(up, left, down, right) {};
	BlWhFilter() : Filter() {};
	BlWhFilter(FiltersBorder& fBorders) : Filter(fBorders) {};
};

class EdgeFilter : public ConvolutionFilter {
public:
	void Apply(image_data& imgData);
	~EdgeFilter() {
		delete ker;
	}



	EdgeFilter(int up, int left, int down, int right, int size = 3) : ConvolutionFilter(up, left, down, right, size) {
		ker = new EdgeKernel(size_);
	};
	EdgeFilter(int size = 3) : ConvolutionFilter(size) {
		ker = new EdgeKernel(size_);
	};
	EdgeFilter(FiltersBorder& fBorders, int size = 3) : ConvolutionFilter(fBorders, size) {
		ker = new EdgeKernel(size_);
	};
};

class BlurFilter : public ConvolutionFilter {
public:
	void Apply(image_data& imgData);
	~BlurFilter() {
		delete ker;
	}


	BlurFilter(int up, int left, int down, int right, int size = 3) : ConvolutionFilter(up, left, down, right, size){
		ker = new BlurKernel(size_);
	};
	BlurFilter(int size = 3) : ConvolutionFilter(size){
		ker = new BlurKernel(size_);
	};
	BlurFilter(FiltersBorder& fBorders, int size = 3) : ConvolutionFilter(fBorders, size){
		ker = new BlurKernel(size_);
	};
};