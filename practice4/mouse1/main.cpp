/**************************************************************************

	file name	: mouse1
	date		: 2017. 8. 19
	writer		: Kyeongsoo Kim
	description : practicing how to handle mouse input using setMouseCallback()
	direction	: click random point on the picture, then cmd will print out
				  the exact cordinate of the location you click.


*****************************************************************************/

#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void on_mouse(int event, int x, int y, int flags, void*);

int main(void)
{
	Mat src = imread("ks.jpg", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	imshow("src", src);
	setMouseCallback("src", on_mouse);

	waitKey(0);
	return 0;
}

void on_mouse(int event, int x, int y, int flags, void*)
{
	switch (event) {
	case EVENT_LBUTTONDOWN:
		cout << "EVENT_LBUTTONDOWN: x = " << x << ", y = " << y << endl;
		break;
	case EVENT_LBUTTONUP:
		cout << "EVENT_LBUTTONUP: x = " << x << ", y = " << y << endl;
		break;
	case EVENT_LBUTTONDBLCLK:
		cout << "EVENT_LBUTTONDBLCLK: x = " << x << ", y = " << y << endl;
		break;
	case EVENT_MOUSEMOVE:
		if (flags & EVENT_FLAG_LBUTTON)
			cout << "EVENT_MOUSEMOVE with LButton Down: x = " << x << ", y = " << y << endl;
		if (flags & EVENT_FLAG_CTRLKEY)
			cout << "EVENT_MOUSEMOVE with CTRL key: x = " << x << ", y = " << y << endl;
		break;
	default:
		break;
	}
}
