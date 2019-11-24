#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <wiringPi.h>

using namespace cv;
using namespace std;

int main(void) {
	Mat src, src_gray, src_blur, src_hist, src_erode, src_dilate, src_canny;
	int lowThreshold = 90;
	int highThreshold = 255;
	int erosion_elem = 0;
	int erosion_size = 0;
	int erosion_type;
	int dilation_elem = 0;
	int dilation_size = 10;
	int dilation_type;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	int k = 0;
	int hitungan = 0;

	wiringPiSetup();
	pinMode(0, OUTPUT);
	digitalWrite(0, LOW);


	VideoCapture cap;
	cap.open(0);
	cap.set(CAP_PROP_FRAME_WIDTH, 256);
	cap.set(CAP_PROP_FRAME_HEIGHT, 208);

	if (!cap.isOpened())
		return 1;

	for (;;) {
		//Get original image
		cap >> src;
		imshow("Source", src);

		//Convert image to grayscale
		cvtColor(src, src_gray, COLOR_BGR2GRAY);
		imshow("Gray", src_gray);

		//Smooth the image
		GaussianBlur(src_gray, src_blur, Size(9, 9), 0, 0);
		imshow("Blurred Image", src_blur);

		//Equalize Histogram
		equalizeHist(src_blur, src_hist);
		imshow("Histogram Equalized", src_hist);

		//Apply Canny Edge Detection
		Canny(src_hist, src_canny, lowThreshold, highThreshold, 3, true);
		imshow("Canny Edge", src_canny);

		//Erode
		if (erosion_elem == 0) {
			erosion_type = MORPH_RECT;
		}
		else if (erosion_elem == 1) {
			erosion_type = MORPH_CROSS;
		}
		else if (erosion_elem == 2) {
			erosion_type = MORPH_ELLIPSE;
		}

		Mat element1 = getStructuringElement(erosion_type, Size(2 * erosion_size + 1, 2 * erosion_size + 1), Point(erosion_size, erosion_size));
		erode(src_canny, src_erode, element1);
		imshow("Eroded", src_erode);

		//Dilate
		if (dilation_elem == 0) {
			dilation_type = MORPH_RECT;
		}
		else if (dilation_elem == 1) {
			dilation_type = MORPH_CROSS;
		}
		else if (dilation_elem == 2) {
			dilation_type = MORPH_ELLIPSE;
		}

		Mat element2 = getStructuringElement(dilation_type, Size(2 * dilation_size + 1, 2 * dilation_size + 1), Point(dilation_size, dilation_size));
		dilate(src_erode, src_dilate, element2);
		imshow("Dilated", src_dilate);


		//Find Contour
		findContours(src_dilate, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));


		int count = 0;
		if (contours.size() > 0) {
			//minEllipse[i] = fitEllipse(Mat(contours[i]));
			cout << "blob = " << contours.size() << endl;
			cout << hitungan << "\tGerakan Terdeteksi!" << endl;
			//hitungan ++;
			digitalWrite(0, HIGH);
		}
		if (contours.size() <= 0) {
			digitalWrite(0, LOW);
		}


		if (cv::waitKey(10) > 0) break;

	}
	destroyAllWindows();
	return 0;

}