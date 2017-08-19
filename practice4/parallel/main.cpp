#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include "opencv2/core/ocl.hpp"
#include <iostream>

using namespace cv;
using namespace std;

class ParallelThreshold : public ParallelLoopBody
{
public:
	ParallelThreshold(Mat& src, Mat& dst, const double threshold)
		: m_src(src), m_dst(dst), m_threshold(threshold)
	{
		m_dst = Mat::zeros(src.rows, src.cols, src.type());
	}

	virtual void operator ()(const Range& range) const
	{
		for (int r = range.start; r < range.end; r++)
		{
			uchar* pSrc = m_src.ptr<uchar>(r);
			uchar* pDst = m_dst.ptr<uchar>(r);

			for (int x = 0; x < m_src.cols; x++) {
				if (pSrc[x] > m_threshold)
					pDst[x] = 255;
				else
					pDst[x] = 0;
			}
		}
	}

	ParallelThreshold& operator =(const ParallelThreshold &) {
		return *this;
	};

private:
	Mat& m_src;
	Mat& m_dst;
	double m_threshold;
};

int main(int argc, char* argv[])
{
	ocl::setUseOpenCL(true);

	Mat src;
	if (argc < 2)
		src = imread("lenna.bmp", IMREAD_GRAYSCALE);
	else
		src = imread(argv[1], IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	resize(src, src, Size(), 10, 10);
	cout << "Image size: " << src.size() << endl;

	Mat dst;
	double th = 128;
	int64 t1, t2;

	// 1. OpenCV function call
	t1 = getTickCount();

	threshold(src, dst, th, 255, THRESH_BINARY);

	t2 = getTickCount();
	cout << "1. function call: " << (t2 - t1) * 1000 / getTickFrequency() << " ms." << endl;

	// 2. No parallel process
	dst = Mat::zeros(src.rows, src.cols, src.type());
	t1 = getTickCount();

	for (int y = 0; y < src.rows; y++) {
		uchar* pSrc = src.ptr<uchar>(y);
		uchar* pDst = dst.ptr<uchar>(y);

		for (int x = 0; x < src.cols; x++) {
			if (pSrc[x] > th)
				pDst[x] = 255;
			else
				pDst[x] = 0;
		}
	}

	t2 = getTickCount();
	cout << "2. Without parallel_for: " << (t2 - t1) * 1000 / getTickFrequency() << " ms." << endl;

	// 3. cv::parallel_for_ with ParallelLoopBody subclass
	dst = Mat::zeros(src.rows, src.cols, src.type());
	t1 = getTickCount();

	ParallelThreshold parallelThreshold(src, dst, th);
	parallel_for_(Range(0, src.rows), parallelThreshold);

	t2 = getTickCount();
	cout << "3. With parallel_for_ (ParallelLoopBody):  " << (t2 - t1) * 1000 / getTickFrequency() << " ms." << endl;

#ifdef CV_CXX_11
	// 4. cv::parallel_for_ with lambda expression
	dst = Mat::zeros(src.rows, src.cols, src.type());
	t1 = getTickCount();

	parallel_for_(Range(0, src.rows), [&](const Range& range) {
		for (int r = range.start; r < range.end; r++)
		{
			uchar* pSrc = src.ptr<uchar>(r);
			uchar* pDst = dst.ptr<uchar>(r);

			for (int x = 0; x < src.cols; x++) {
				if (pSrc[x] > th)
					pDst[x] = 255;
				else
					pDst[x] = 0;
			}
		}
	});

	t2 = getTickCount();
	cout << "4. With parallel_for_ (lambda expression): " << (t2 - t1) * 1000 / getTickFrequency() << " ms." << endl;
#endif

	imshow("src", src);
	imshow("dst", dst);

	waitKey(0);
	return 0;
}