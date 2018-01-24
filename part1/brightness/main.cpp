#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat src = imread("lenna.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	Mat dst1(src.rows, src.cols, src.type());
	Mat dst2(src.rows, src.cols, src.type());
	Mat dst3(src.rows, src.cols, src.type());
	Mat dst4(src.rows, src.cols, src.type());

#if 1
	dst1 = src + 100;


	//same as line 21
	//add(src, 100, dst1);
#else
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {

			//wrong example. didn't consider if pixel value is over 255.
			dst1.at<uchar>(i, j) = src.at<uchar>(i, j) + 100;

			//fix the wrong implement like line 33
			int v = src.at<uchar>(i, j) + 100;
			if (v > 255)
				v = 255;
			else if (v < 0)
				v = 0;
			dst2.at<uchar>(i, j) = v;
			
			//using c/c++ grammar
			dst3.at<uchar>(i, j) = v > 255 ? 255 : v < 0 ? 0 : v;

			//using opencv function
			dst4.at<uchar>(i, j) = saturate_cast<uchar>(src.at<uchar>(i, j) + 100);
		}
	}


#endif

	imshow("src", src);
	imshow("dst1", dst1);

	//line 58, 59 and 60 shows the same result as line 55
	//imshow("dst2", dst2);
	//imshow("dst3", dst3);
	//imshow("dst4", dst4);


	waitKey(0);
	return 0;
}