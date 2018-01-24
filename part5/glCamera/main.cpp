#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include "opencv2/core/opengl.hpp"
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include <iostream>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

using namespace cv;
using namespace std;

Mat frame;
const String win_name = "opengl";

void on_opengl(void* param);
void drawFrame();
void drawTriangle();

int main()
{
	VideoCapture cap(0);

	if (!cap.isOpened()) {
		cerr << "Failed to open camera!" << endl;
		return -1;
	}

	int frame_width = (int)cap.get(CAP_PROP_FRAME_WIDTH);
	int frame_height = (int)cap.get(CAP_PROP_FRAME_HEIGHT);
	cout << "width: " << frame_width << ", height: " << frame_height << endl;

	namedWindow(win_name, WINDOW_OPENGL);
	resizeWindow(win_name, frame_width, frame_height);
	setOpenGlContext(win_name);
	setOpenGlDrawCallback(win_name, on_opengl, NULL);

	for (;;) {
		cap >> frame;
		if (frame.empty()) {
			cerr << "Empty frame!" << endl;
			break;
		}

		updateWindow(win_name);

		if (waitKey(30) >= 0)
			break;
	}

	return 0;
}

void on_opengl(void* param)
{
	// OpenCV 라이브러리 내에서 자동으로 아래와 같은 형태로 호출됨. 
	// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	// on_opengl(...)
	// SwapBuffers(...)

	drawFrame();
	drawTriangle();
}

void drawFrame()
{
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	glColor3f(1.0, 1.0, 1.0);

	// 카메라 입력 프레임을 텍스쳐로 가져오기
	cv::ogl::Texture2D texture;
	texture.copyFrom(frame, true);
	texture.bind();

	// 창 전체 크기에 맞게 텍스쳐를 출력
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, +1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(+1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(+1.0f, +1.0f);
	glEnd();
}

void drawTriangle()
{
	glLoadIdentity();

	glBegin(GL_TRIANGLES);
	glColor3f(1, 0, 0);		glVertex2f(+0.0f, +0.4f);
	glColor3f(0, 1, 0);		glVertex2f(-0.4f, -0.4f);
	glColor3f(0, 0, 1);		glVertex2f(+0.4f, -0.4f);
	glEnd();
}
