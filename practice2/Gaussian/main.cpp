#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(void)
{
	Mat src = imread("lenna.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	namedWindow("src");
	imshow("src", src);

	Mat dst;
	namedWindow("dst");

	for (int sigma = 1; sigma <= 5; sigma++) {
		double start = double(getTickCount());
		
		GaussianBlur(src, dst, Size(0, 0), (double)sigma);
		
		double duration_ms = (double(getTickCount()) - start) * 1000 / getTickFrequency();
		cout << "sigma = " << sigma << ", It took " << duration_ms << " ms." << endl;

		String text = format("sigma = %d", sigma);
		putText(dst, text, Point(10, 30), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255), 1, LINE_AA);
		imshow("dst", dst);
		waitKey(0);
	}
	
	return 0;
}