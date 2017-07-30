//#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void func1()
{
	// make new image with size designation
	int nRows = 480, nCols = 640;		
	Mat img1(nRows, nCols, CV_8U);		// CV_8U : (8bit unsigned) unsigned char type
										// *caution* : first argument is nRows, not n Cols.
	Mat img2(nRows, nCols, CV_8UC3);	// CV_8UC3 : unsigned char type, 3 channels
	img2.create(nRows, nCols, CV_32F);	// CV_32F : float
	Mat img3(Size(640, 480), CV_8UC3);	// Size(width, length)

	// read a file
	Mat img4 = imread("lenna.bmp", IMREAD_COLOR);

	// print image(Array) information 
	cout << "Width: " << img4.cols << endl;
	cout << "Height: " << img4.rows << endl;
	cout << "Channels: " << img4.channels() << endl;
	cout << "elemSize: " << img4.elemSize() << endl;
	cout << "step[0]: " << img4.step[0] << endl;
	cout << "step[1]: " << img4.step[1] << endl;
}

void func2()
{
	//create and set up the element value simultaneously.

	int nRows = 3, nCols = 4;
	Mat mat1(nRows, nCols, CV_8U, Scalar(128));

	Mat mat2 = Mat::ones(3, 4, CV_8U); 	// create array full of 1
	Mat mat3 = Mat::zeros(3, 4, CV_8U);	// create array full of 0

	// 이미 존재하는 행렬의 원소 값 설정

	mat1.setTo(0);

	// 배열을 이용한 원소 값 초기화. data와 img4 행렬 원소 값은 상호 sync 됨.
	// initialization using array. data and img4 array is mutually synchronized.
	uchar data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	Mat mat4(3, 3, CV_8U, data);

	//data[3] = 100;

	// same work as line 44 + 45. previous example is more common one.
	Mat mat5 = (Mat_<double>(3, 3) << 1, 0, 0, 0, 1, 0, 0, 0, 1);
	cout << "mat5:\n" << mat5 << endl;

	
}

void func3()
{
	// image reference & copy
	Mat img1 = imread("lenna.bmp", IMREAD_GRAYSCALE);

	Mat img2 = img1;			// reference (shallow copying) : using same memory.
	Mat img3 = img1.clone();	// deep copying : using different memory
	Mat img4;
	img1.copyTo(img4);  		// deep copying 

	// partial image reference & copy
	Mat img5 = img1(Rect(100, 100, 200, 200));
	img5.setTo(0); //affect img1 and img2 since img5 is created by shallow copying

	Mat img6 = img4(Rect(100, 100, 200, 200)).clone();
	img6.setTo(255); //it doesn't affect img4 since img6 is created by deep copying
}

void func4()
{
	float data[] = { 1, 1, 2, 3 };
	Mat mat1(2, 2, CV_32F, data);
	cout << "mat1:\n" << mat1 << endl;

	// 역행렬
	Mat mat2 = mat1.inv();
	cout << "mat2:\n" << mat2 << endl;

	// 행렬의 곱셉
	cout << "mat1 * mat2:\n" << mat1 * mat2 << endl;

	// 행렬 원소끼리의 곱셈
	cout << "mat1.mul(mat2):\n" << mat1.mul(mat2) << endl;
}

int main()
{
	func1();
	func2();
	func3();
	func4();

	return 0;
}