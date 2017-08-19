#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(void)
{
	[] {};		// 가장 간단한 형태의 람다 표현식
	[]() {};	// 비어있는 인자
	[] {}();	// 호출까지 수행

	[](int a) { cout << a*a << endl; }(4); // 인자를 전달하여 호출

	int a = 10;
	[=] { cout << "a = " << a << endl; }(); // Call by value
	[&] { a = 20; }();	// Call by reference
	cout << "a = " << a << endl;

	int b = [&] { return 300; }();  // 반환값이 있는 람다 표현식
	cout << "b = " << b << endl;

	int c = [&]() -> uchar { return 300; }(); // 반환값 타입 지정
	cout << "c = " << c << endl;

	return 0;
}
