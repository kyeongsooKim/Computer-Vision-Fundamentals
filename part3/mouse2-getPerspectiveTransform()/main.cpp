
/**************************************************************************

file name	: mouse2-getPerspectiveTransform()
date		: 2017. 8. 19
writer		: Kyeongsoo Kim
direction	: click the 4 edges of the business card on the picture, then
			  getPerspectiveTransform() transform the perspective so that
			  returned image looks natural.
			  the desired order of click is 1. upper left-most edge ->
			  2. upper right-most edge -> lower right-most edge ->
			  lower left-most edge.
			  

*****************************************************************************/


#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

Mat src, dst;
const String SRC_WND = "src";
Point2f srcQuad[4], dstQuad[4];

void on_mouse(int event, int x, int y, int flags, void*)
{
	static int cnt = 0;

	if (event == EVENT_LBUTTONDOWN) {
		srcQuad[cnt++] = Point2f(x, y);

		circle(src, Point(x, y), 7, Scalar(0, 0, 255), -1);
		imshow(SRC_WND, src);

		if (cnt == 4) {

			//create the dst whose width is 500px and height is 900px
			int h = 900, w = 500;
			dst.create(Size(w, h), CV_8SC3);

			//save 4 points which user click on the image.
			dstQuad[0] = Point2f(0, 0);
			dstQuad[1] = Point2f(w, 0);
			dstQuad[2] = Point2f(w, h);
			dstQuad[3] = Point2f(0, h);

			for (int i = 0; i < 4; i++) {
				cout << i << ": " << srcQuad[i] << " => " << dstQuad[i] << endl;
			}

			//getPersepectiveTransform : return the transformed image.
			Mat pers = getPerspectiveTransform(srcQuad, dstQuad);

			//crop the given transformed image from the line 54. 
			warpPerspective(src, dst, pers, Size(w, h));

			imshow("dst", dst);
		}

	} else if (event == EVENT_LBUTTONUP) {
		if (cnt == 4) {
			setMouseCallback(SRC_WND, 0);
		}
	}
}

int main()
{
	src = imread("namecard.jpg");

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	namedWindow(SRC_WND);
	imshow(SRC_WND, src);

	setMouseCallback(SRC_WND, on_mouse);

	waitKey(0);
	return 0;
}