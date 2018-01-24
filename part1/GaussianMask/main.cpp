#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void Gaussian1(float sigma)
{
	int dim = cvRound(2 * 4 * sigma + 1);
	if (dim < 3) dim = 3;
	if (dim % 2 == 0) dim++;
	int dim2 = dim / 2;

	Mat kernel(dim, dim, CV_32F);

	for (int j = 0; j < dim; j++) {
		for (int i = 0; i < dim; i++) {
			float x = i - dim2;
			float y = j - dim2;
			kernel.at<float>(j, i) = exp(-(x*x + y*y) / (2 * sigma*sigma)) / (2 * CV_PI*sigma*sigma);
		}
	}

	cout << "Gaussian kernel by 2D Gaussian equation:" << dim << " x " << dim << endl;
	cout << kernel << endl;
}

void Gaussian2(float sigma)
{
	int dim = cvRound(2 * 4 * sigma + 1);
	if (dim < 3) dim = 3;
	if (dim % 2 == 0) dim++;
	int dim2 = dim / 2;

	Mat kernel1(dim, 1, CV_32F);

	for (int i = 0; i < dim; i++) {
		float x = i - dim2;
		kernel1.at<float>(i, 0) = exp(-(x*x) / (2 * sigma*sigma)) / (sqrt(2 * CV_PI)*sigma);
	}

	Mat kernel = kernel1 * kernel1.t();

	cout << "Gaussian kernel by 1D Gaussian equation:" << dim << " x " << dim << endl;
	cout << kernel << endl;
}

void Gaussian3(float sigma)
{
	int dim = cvRound(2 * 4 * sigma + 1);
	Mat kernel1 = getGaussianKernel(dim, sigma, CV_32F);

	Mat kernel = kernel1 * kernel1.t();

	cout << "Gaussian kernel by OpenCV getGaussianKernel():" << dim << " x " << dim << endl;
	cout << kernel << endl;
}

int main()
{
	float sigma;
	cout << "Enter standard deviation: ";
	cin >> sigma;

	Gaussian1(sigma);
	Gaussian2(sigma);
	Gaussian3(sigma);

	return 0;
}