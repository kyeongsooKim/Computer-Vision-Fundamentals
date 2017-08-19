#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(void)
{
	[] {};		// ���� ������ ������ ���� ǥ����
	[]() {};	// ����ִ� ����
	[] {}();	// ȣ����� ����

	[](int a) { cout << a*a << endl; }(4); // ���ڸ� �����Ͽ� ȣ��

	int a = 10;
	[=] { cout << "a = " << a << endl; }(); // Call by value
	[&] { a = 20; }();	// Call by reference
	cout << "a = " << a << endl;

	int b = [&] { return 300; }();  // ��ȯ���� �ִ� ���� ǥ����
	cout << "b = " << b << endl;

	int c = [&]() -> uchar { return 300; }(); // ��ȯ�� Ÿ�� ����
	cout << "c = " << c << endl;

	return 0;
}
