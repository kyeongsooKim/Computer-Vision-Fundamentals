#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat src = imread("cookierun.png");
	Mat templ = imread("item.png");

	if (src.empty() || templ.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	Mat dst = src.clone();

	Mat res, res_norm;
	matchTemplate(src, templ, res, TM_CCOEFF_NORMED);
	normalize(res, res_norm, 0, 255, NORM_MINMAX, CV_8U);

	Mat local_max = res_norm > 220;

	Mat labels;
	int num = connectedComponents(local_max, labels);

	for (int i = 1; i < num; i++) {
		Point max_loc;
		Mat mask = (labels == i);
		minMaxLoc(res, 0, 0, 0, &max_loc, mask);

		cout << max_loc.x << ", " << max_loc.y << endl;

		Rect b_rect = Rect(max_loc.x, max_loc.y, templ.cols, templ.rows);
		rectangle(dst, b_rect, Scalar(0, 255, 255), 2);
	}

//	imshow("src", src);
//	imshow("templ", templ);
//	imshow("res_norm", res_norm);
	imshow("local_max", local_max);
	imshow("dst", dst);

	waitKey(0);
	return 0;
}