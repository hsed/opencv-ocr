/*
* textdetection.cpp
*
* A demo program of End-to-end Scene Text Detection and Recognition:
* Shows the use of the Tesseract OCR API with the Extremal Region Filter algorithm described in:
* Neumann L., Matas J.: Real-Time Scene Text Localization and Recognition, CVPR 2012
*
* Created on: Jul 31, 2014
*     Author: Lluis Gomez i Bigorda <lgomez AT cvc.uab.es>
*/

#include "opencv2/text.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <Windows.h>

#include <iostream>

using namespace std;
using namespace cv;
using namespace cv::text;

const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;


Mat hwnd2mat(HWND hwnd)
{
	HDC hwindowDC, hwindowCompatibleDC;

	int height, width, srcheight, srcwidth;
	HBITMAP hbwindow;
	Mat src;
	BITMAPINFOHEADER  bi;

	hwindowDC = GetDC(hwnd);
	hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
	SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

	RECT windowsize;    // get the height and width of the screen
	GetClientRect(hwnd, &windowsize);

	srcheight = windowsize.bottom;
	srcwidth = windowsize.right;
	height = windowsize.bottom / 1;  //change this to whatever size you want to resize to
	width = windowsize.right / 1;

	src.create(height, width, CV_8UC4);

	// create a bitmap
	hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
	bi.biSize = sizeof(BITMAPINFOHEADER);    //http://msdn.microsoft.com/en-us/library/windows/window/dd183402%28v=vs.85%29.aspx
	bi.biWidth = width;
	bi.biHeight = -height;  //this is the line that makes it draw upside down or not
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	// use the previously created device context with the bitmap
	SelectObject(hwindowCompatibleDC, hbwindow);
	// copy from the window device context to the bitmap device context
	StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, 0, 0, srcwidth, srcheight, SRCCOPY); //change SRCCOPY to NOTSRCCOPY for wacky colors !
	GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO *)&bi, DIB_RGB_COLORS);  //copy from hwindowCompatibleDC to hbwindow

																									   // avoid memory leak
	DeleteObject(hbwindow);
	DeleteDC(hwindowCompatibleDC);
	ReleaseDC(hwnd, hwindowDC);

	return src;
}

//Perform text detection and recognition and evaluate results using edit distance
int main(int argc, char* argv[])
{
	cout << endl << argv[0] << endl << endl;
	cout << "A demo program of End-to-end Scene Text Detection and Recognition: " << endl;
	cout << "Shows the use of the Tesseract OCR API with the Extremal Region Filter algorithm described in:" << endl;
	cout << "Neumann L., Matas J.: Real-Time Scene Text Localization and Recognition, CVPR 2012" << endl << endl;

	const LPCWSTR windowString = L"aaa.TXT - Notepad"; // "Long Pointer to Const Wide String, stupid MS notation.

	HWND hwndDesktop = FindWindow(0, windowString);//FindWindowEx(NULL, NULL, LPCWSTR("OpusApp"), NULL); //GetActiveWindow(); // GetDesktopWindow();
	while (!hwndDesktop)
	{
		printf("Failed FindWindow\n");
		hwndDesktop = FindWindow(0, windowString);
	}

	//cout << "IMG_W=" << image.cols << endl;
	//cout << "IMG_H=" << image.rows << endl;

	/*Text Recognition (OCR)*/
	double t_r = (double)getTickCount();
	Ptr<OCRTesseract> ocr = OCRTesseract::create();
	cout << "TIME_OCR_INITIALIZATION = " << ((double)getTickCount() - t_r) * 1000 / getTickFrequency() << endl;

	Mat image;
	Mat src;
	//video capture object to acquire webcam feed
	VideoCapture capture;
	//open capture object at location zero (default location for webcam)
	capture.open(0);
	//set height and width of capture frame
	capture.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);

	//*******************************loop start****************************************************
	bool quit = false;
	while (!quit) {
		//src = hwnd2mat(hwndDesktop);
		hwnd2mat(hwndDesktop).copyTo(src);
		if (src.rows > 0 && src.cols > 0) {


			capture.read(image);
			Mat grey;
			//cvtColor(image, grey, COLOR_RGB2GRAY);
			cvtColor(src, grey, COLOR_RGBA2GRAY);

			float scale_img = 600.f / image.rows;
			float scale_font = (float)(2 - scale_img) / 1.4f;

			Mat g_img = Mat::zeros(grey.rows + 2, grey.cols + 2, CV_8UC1);
			convertScaleAbs(grey, g_img);
			//convertScaleAbs(image, g_img);
			string output;
			vector<Rect>   boxes;
			vector<string> words;
			vector<float>  confidences;

			t_r = (double)getTickCount();
			ocr->run(g_img, output, &boxes, &words, &confidences, OCR_LEVEL_WORD);

			output.erase(remove(output.begin(), output.end(), '\n\n'), output.end());
			cout << "OCR output:\n" << output << "\nOCR Length: " << output.size() << endl;
			cout << "OCR No of Words: " << words.size() << endl;

			cout << "TIME_OCR = " << ((double)getTickCount() - t_r) * 1000 / getTickFrequency() << endl;

			for (int i = 0; i < boxes.size(); i++) {
				if ((words.size() < 2) || (confidences[i] < 51) || ((words.size() == 2) && (words[0] == words[1])) || ((words.size()< 4) && (confidences[i] < 60)))
					continue;
				rectangle(src, boxes[i].tl(), boxes[i].br(), Scalar(255, 0, 255), 2);
				Size word_size = getTextSize(words[i], FONT_HERSHEY_SIMPLEX, (double)scale_font, (int)(2 * scale_font), NULL);
				rectangle(src, boxes[i].tl() - Point(2, word_size.height + 2), boxes[i].tl() + Point(word_size.width, 0), Scalar(255, 0, 255), -1);
				putText(src, words[i], boxes[i].tl() - Point(1, 1), FONT_HERSHEY_SIMPLEX, scale_font, Scalar(255, 255, 255), (int)(2 * scale_font));
			}
			namedWindow("recognition", WINDOW_NORMAL);
			imshow("recognition", src);
		}
		else {
			cout << "Window minimized, no text detection will take place." << endl;
		}
		int k = waitKey(30); //wait for stability, nevertheless the function will take atleast 1 sec to recognize text
		if (k == 113) { cout << "quitting..."; quit = true; };
		//if (k > -1) { cout << "Key Pressed: " << k << endl; }

	}
	ReleaseCapture();
	destroyAllWindows();
	return 0;
}

