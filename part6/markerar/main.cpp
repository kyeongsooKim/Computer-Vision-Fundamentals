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

Mat frame, templ;

Mat m_intrinsic, m_distortion;
Mat m_rotation, m_translation;
vector<Point3f> m_markerCorners3d;

const int TEMPL_SIZE = 32; 
enum { ROTATE_0 = 0, ROTATE_90, ROTATE_180, ROTATE_270 };

// Camera parameters
float fx = 526.58037684199849f;
float fy = 524.65577209994706f;
float cx = 318.41744018680112f;
float cy = 202.96659047014398f;

const char* win_name = "opengl";

void on_opengl(void* param);
void drawCameraFrame();
void drawAR();
bool detectMarker();
double checkMarker(const Mat& src, const vector<Point> contour, int& rot);
void estimatePose(vector<Point> points, int rot);

int main(int argc, char* argv[])
{
	templ = imread("templ.bmp", IMREAD_GRAYSCALE);
	if (templ.empty()) {
		cerr << "Template image load failed!" << endl;
		return -1;
	}

	resize(templ, templ, Size(TEMPL_SIZE, TEMPL_SIZE));

	VideoCapture cap;
	if (argc < 2) {
		cap.open(0);
	} else {
		cap.open(argv[1]);
	}

	if (!cap.isOpened()) {
		cerr << "Failed to open camera!" << endl;
		return -1;
	}

	int delay = 15;
	if (argc >= 2) {
		int fps = (int)cap.get(CAP_PROP_FPS);
		delay = 1000 / fps;
	}

	int frame_width = (int)cap.get(CAP_PROP_FRAME_WIDTH);
	int frame_height = (int)cap.get(CAP_PROP_FRAME_HEIGHT);
	cout << "width: " << frame_width << ", height: " << frame_height << endl;

	float intrinsic[] = { fx, 0, cx, 0, fy, cy, 0, 0, 1 };
	m_intrinsic = Mat(3, 3, CV_32F, intrinsic);

	m_distortion = Mat::zeros(4, 1, CV_32F);

	m_markerCorners3d.push_back(cv::Point3f(-0.5f, -0.5f, 0));
	m_markerCorners3d.push_back(cv::Point3f(+0.5f, -0.5f, 0));
	m_markerCorners3d.push_back(cv::Point3f(+0.5f, +0.5f, 0));
	m_markerCorners3d.push_back(cv::Point3f(-0.5f, +0.5f, 0));

	m_rotation.create(3, 3, CV_32F);
	m_translation.create(3, 1, CV_32F);

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

		if (waitKey(delay) == 27)
			break;
	}

	return 0;
}

void on_opengl(void*)
{
	drawCameraFrame();

	if (detectMarker()) {
		drawAR();
	}
}

void drawCameraFrame()
{
	glEnable(GL_TEXTURE_2D);
	glColor3f(1.0, 1.0, 1.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	cv::ogl::Texture2D texture;
	texture.setAutoRelease(true);
	texture.copyFrom(frame, true);
	texture.bind();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, +1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(+1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(+1.0f, +1.0f);
	glEnd();
}

void drawAR()
{
	// build projection matrix
	int w = frame.cols;
	int h = frame.rows;

	float nearPlane = 0.01f;  // Near clipping distance
	float farPlane = 100.0f;  // Far clipping distance

	GLfloat projection[16] = { 0.f, };
	projection[0] = -2.0f * fx / w;
	projection[1] = 0.0f;
	projection[2] = 0.0f;
	projection[3] = 0.0f;

	projection[4] = 0.0f;
	projection[5] = 2.0f * fy / h;
	projection[6] = 0.0f;
	projection[7] = 0.0f;

	projection[8] = 2.0f * cx / w - 1.0f;
	projection[9] = 2.0f * cy / h - 1.0f;
	projection[10] = -(farPlane + nearPlane) / (farPlane - nearPlane);
	projection[11] = -1.0f;

	projection[12] = 0.0f;
	projection[13] = 0.0f;
	projection[14] = -2.0f * farPlane * nearPlane / (farPlane - nearPlane);
	projection[15] = 0.0f;

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(projection);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_DEPTH_TEST);

	GLfloat camera_pose[16] = { 0.f, };
	camera_pose[0] = m_rotation.at<float>(0, 0);
	camera_pose[1] = m_rotation.at<float>(0, 1);
	camera_pose[2] = m_rotation.at<float>(0, 2);
	camera_pose[3] = 0;
	camera_pose[4] = m_rotation.at<float>(1, 0);
	camera_pose[5] = m_rotation.at<float>(1, 1);
	camera_pose[6] = m_rotation.at<float>(1, 2);
	camera_pose[7] = 0;
	camera_pose[8] = m_rotation.at<float>(2, 0);
	camera_pose[9] = m_rotation.at<float>(2, 1);
	camera_pose[10] = m_rotation.at<float>(2, 2);
	camera_pose[11] = 0;
	camera_pose[12] = m_translation.at<float>(0, 0);
	camera_pose[13] = m_translation.at<float>(1, 0);
	camera_pose[14] = m_translation.at<float>(2, 0);
	camera_pose[15] = 1.f;
	glLoadMatrixf(camera_pose);

	// draw cube
	static const int coords[6][4][3] = {
		{ { +1, -1, 0 },{ -1, -1, 0 },{ -1, +1, 0 },{ +1, +1, 0 } },
		{ { +1, +1, 0 },{ -1, +1, 0 },{ -1, +1, 2 },{ +1, +1, 2 } },
		{ { +1, -1, 2 },{ +1, -1, 0 },{ +1, +1, 0 },{ +1, +1, 2 } },
		{ { -1, -1, 0 },{ -1, -1, 2 },{ -1, +1, 2 },{ -1, +1, 0 } },
		{ { +1, -1, 2 },{ -1, -1, 2 },{ -1, -1, 0 },{ +1, -1, 0 } },
		{ { -1, -1, 2 },{ +1, -1, 2 },{ +1, +1, 2 },{ -1, +1, 2 } }
	};

	for (int i = 0; i < 6; ++i) {
		glColor3ub(i * 20 + 100, i * 10 + 150, i * 40);
		glBegin(GL_QUADS);
		for (int j = 0; j < 4; ++j) {
			glVertex3d(0.5 * coords[i][j][0], 0.5 * coords[i][j][1], 0.5 * coords[i][j][2]);
		}
		glEnd();
	}

#if 0
	// draw coordinate axis
	static float lineX[] = { 0, 0, 0, 1, 0, 0 };
	static float lineY[] = { 0, 0, 0, 0, 1, 0 };
	static float lineZ[] = { 0, 0, 0, 0, 0, 1 };

	glLineWidth(3.f);
	glBegin(GL_LINES);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3fv(lineX);
	glVertex3fv(lineX + 3);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3fv(lineY);
	glVertex3fv(lineY + 3);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3fv(lineZ);
	glVertex3fv(lineZ + 3);

	glEnd();
#endif

	glDisable(GL_DEPTH_TEST); 
}

bool detectMarker()
{
	double start_tick = (double)getTickCount();

	bool detected = false;

	Mat src;
	cvtColor(frame, src, COLOR_BGR2GRAY);

	Mat src_bin;
	threshold(src, src_bin, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);
//	adaptiveThreshold(src, src_bin, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 101, 0);

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(src_bin.clone(), contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_NONE);

	for (int idx = 0; idx >= 0; idx = hierarchy[idx][0]) {
		// 외곽선이 너무 짧거나 또는 너무 길면 무시.
		if ((contours[idx].size() < 200) || (contours[idx].size() > 240000))
			continue;

		// 모든 외곽선을 다각형 근사화하여 이 중 사각형 모양만 찾음.
		vector<Point> approx;
		approxPolyDP(contours[idx], approx, arcLength(contours[idx], true) * 0.02, true);

		if ((approx.size() == 4) && isContourConvex(approx)) {
			// 계층적으로 차일드 외곽선(내부 외곽선)을 가지고 있지 않으면 무시.
			int inner_idx = hierarchy[idx][2];
			if (inner_idx < 0) continue;

			// 외부&내부 모두 사각형 모양인 경우, 
			vector<Point> approx2;
			approxPolyDP(contours[inner_idx], approx2, arcLength(contours[inner_idx], true) * 0.02, true);

			if (approx2.size() == 4) {
				int rot;
				if (checkMarker(src, approx2, rot) > 0.5f) {
					estimatePose(approx2, rot);
					detected = true;
				}
			}
		}
	}

	double duration_ms = (double(getTickCount()) - start_tick) * 1000 / getTickFrequency();
	cout << "detectMarker() took " << duration_ms << " ms." << endl;

	return detected;
}

double checkMarker(const Mat& src, const vector<Point> contour, int& rot)
{
	Point2f srcQuad[4], dstQuad[4];
	srcQuad[0] = contour[0];
	srcQuad[1] = contour[1];
	srcQuad[2] = contour[2];
	srcQuad[3] = contour[3];

	dstQuad[0].x = 0; dstQuad[0].y = 0;
	dstQuad[1].x = TEMPL_SIZE; dstQuad[1].y = 0;
	dstQuad[2].x = TEMPL_SIZE; dstQuad[2].y = TEMPL_SIZE;
	dstQuad[3].x = 0; dstQuad[3].y = TEMPL_SIZE;


	Mat dst, res;
	Mat per_mat = getPerspectiveTransform(srcQuad, dstQuad);
	Mat rot90 = getRotationMatrix2D(Point2f(TEMPL_SIZE / 2.f, TEMPL_SIZE / 2.f), 90.f, 1.f);
	float max_ccoeff = 0.f;

	warpPerspective(src, dst, per_mat, Size(TEMPL_SIZE, TEMPL_SIZE));
	matchTemplate(templ, dst, res, TM_CCOEFF_NORMED);
	if (res.at<float>(0, 0) > max_ccoeff) {
		max_ccoeff = res.at<float>(0, 0);
		rot = ::ROTATE_0;
	}

	warpAffine(dst, dst, rot90, Size(TEMPL_SIZE, TEMPL_SIZE));
	matchTemplate(templ, dst, res, TM_CCOEFF_NORMED);
	if (res.at<float>(0, 0) > max_ccoeff) {
		max_ccoeff = res.at<float>(0, 0);
		rot = ::ROTATE_90;
	}

	warpAffine(dst, dst, rot90, Size(TEMPL_SIZE, TEMPL_SIZE));
	matchTemplate(templ, dst, res, TM_CCOEFF_NORMED);
	if (res.at<float>(0, 0) > max_ccoeff) {
		max_ccoeff = res.at<float>(0, 0);
		rot = ::ROTATE_180;
	}

	warpAffine(dst, dst, rot90, Size(TEMPL_SIZE, TEMPL_SIZE));
	matchTemplate(templ, dst, res, TM_CCOEFF_NORMED);
	if (res.at<float>(0, 0) > max_ccoeff) {
		max_ccoeff = res.at<float>(0, 0);
		rot = ::ROTATE_270;
	}

	return max_ccoeff;
}

void estimatePose(vector<Point> points, int rot)
{
	vector<Point2f> marker_points;
	marker_points.push_back(points[(0 + rot) % 4]);
	marker_points.push_back(points[(1 + rot) % 4]);
	marker_points.push_back(points[(2 + rot) % 4]);
	marker_points.push_back(points[(3 + rot) % 4]);

	Mat rvec, tvec;
	solvePnP(m_markerCorners3d, marker_points, m_intrinsic, m_distortion, rvec, tvec);

	Mat rmat;
	Rodrigues(rvec, rmat); // 3x1 -> 3x3

	rmat.copyTo(m_rotation);
	tvec.copyTo(m_translation);

	// Since solvePnP finds camera location, w.r.t to marker pose, to get marker pose w.r.t to the camera we invert it.
	m_rotation = m_rotation.t();
	m_translation = -m_translation;
}
