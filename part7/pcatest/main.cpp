#pragma warning(disable: 4819)

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	// (1) 

	float mat_data[] = { 
		0, 0,
		1, 1,
		2, 1,
		1, 2,
		2, 2,
		3, 3,
	};

	Mat mat(6, 2, CV_32F, mat_data);
	cout << "> samples:\n" << mat << endl;

	Mat cov, mean;
	calcCovarMatrix(mat, cov, mean, COVAR_NORMAL | COVAR_ROWS | COVAR_SCALE);
	cout << "> cov:\n" << cov << endl;
	cout << "> mean:\n" << mean << endl;

	Mat eig_values, eig_vectors;
	eigen(cov, eig_values, eig_vectors);
	cout << "> eig_vectors:\n" << eig_vectors << endl;
	cout << "> eig_values:\n" << eig_values << endl;
	cout << endl;

	// (2) 

	PCA pca(mat, Mat(), PCA::DATA_AS_ROW);
	cout << "> pca.mean:\n" << pca.mean << endl;
	cout << "> pca.eigenvalues:\n" << pca.eigenvalues << endl;
	cout << "> pca.eigenvectors:\n" << pca.eigenvectors << endl;
	cout << endl;
	
	// (3) 

	uchar img_data[] = {
		1, 0, 0, 0,
		0, 1, 1, 0,
		0, 1, 1, 0,
		0, 0, 0, 1,
	};

	Mat img(4, 4, CV_8U, img_data);

	Moments m = moments(img);
	double m00 = m.m00, m10 = m.m10, m01 = m.m01;

	double cx = m10 / m00, cy = m01 / m00;
	cout << "> mass center:" << cx << ", " << cy << endl;

	double mu11 = m.mu11, mu20 = m.mu20, mu02 = m.mu02;
	double inv_m00 = 1. / m00;
	double a = mu20 * inv_m00, b = mu11 * inv_m00, c = mu02 * inv_m00;

	// Calculating orientation
	double theta = atan2(2 * b, a - c) / 2;
	cout << "> theta: " << (theta * 180) / CV_PI << endl;

	return 0;
}