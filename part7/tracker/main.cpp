#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include "opencv2/tracking.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	// Set up tracker. Instead of TrackerKCF, 
	// you can also use TrackerBoosting, TrackerGOTURN, 
	// TrackerMedianFlow, TrackerMIL, and TrackerTLD.
	Ptr<Tracker> tracker = TrackerKCF::create();

	// Read video
	VideoCapture video("chaplin.mp4");

	// Check video is open
	if (!video.isOpened()) {
		cout << "Could not read video file" << endl;
		return 1;
	}

	// Read first frame. 
	Mat frame;
	video.read(frame);

	//imwrite("first.bmp", frame);

	// Define an initial bounding box
	Rect2d bbox(287, 23, 86, 320);

	// Uncomment the line below if you 
	// want to choose the bounding box
	// bbox = selectROI(frame, false);

	// Initialize tracker with first frame and bounding box
	tracker->init(frame, bbox);

	while (video.read(frame))
	{
		// Update tracking results
		tracker->update(frame, bbox);

		// Draw bounding box
		rectangle(frame, bbox, Scalar(255, 0, 0), 2, 1);

		// Display result
		imshow("Tracking", frame);
		
		if (waitKey(1) == 27) 
			break;
	}

	return 0;
}