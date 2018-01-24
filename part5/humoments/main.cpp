#pragma warning(disable: 4819)

#include <opencv2/opencv.hpp>
#include <stdio.h>

using namespace cv;

const char* filename[] = { "spade1.bmp", "spade2.bmp" , "spade3.bmp" , "star.bmp" };

int main()
{
	Mat src;

	for (int i = 0; i < 4; i++) {
		src = imread(filename[i], IMREAD_GRAYSCALE);
		if (src.empty()) {
			printf("Image load failed!\n");
			return -1;
		}

		double hu[7] = { 0., };
		Moments m = moments(src, true);
		HuMoments(m, hu);

		printf("[%i: %10s] ", i, filename[i]);
		for (int k = 0; k < 7; k++) {
			printf("%7.3f  ", fabs(log(fabs(hu[k]))));
		}
		printf("\n");
	}

	getchar();

	return 0;
}