#include "opencv2/highgui/highgui.hpp"
#include "opencv\cv.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

using namespace cv;
using namespace std;

IplImage* src = 0;
IplImage* dst = 0;

int findCells(IplImage* _image)
{
	assert(_image != 0);

	IplImage* bin = cvCreateImage(cvGetSize(_image), IPL_DEPTH_8U, 1);

	// конвертируем в градации серого
	cvConvertImage(_image, bin, CV_BGR2GRAY);
	// находим границы
	cvCanny(bin, bin, 50, 200);

	cvNamedWindow("bin", 1);
	cvShowImage("bin", bin);

	// хранилище памяти для контуров
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contours = 0;

	// находим контуры
	int contoursCont = cvFindContours(bin, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));

	assert(contours != 0);
	int num = 0;
	// обходим все контуры
	for (CvSeq* current = contours; current != NULL; current = current->h_next){
		// вычисляем площадь и периметр контура
		double area = fabs(cvContourArea(current));
		double perim = cvContourPerimeter(current);

		// 1/4*CV_PI = 0,079577
		//if (area / (perim * perim) > 0.07 && area / (perim * perim)< 0.087){ // в 10% интервале
			// нарисуем контур
			cvDrawContours(_image, current, cvScalar(0, 0, 255), cvScalar(0, 255, 0), -1, 1, 8);
		//}
			++num;
	}

	// освобождаем ресурсы
	cvReleaseMemStorage(&storage);
	cvReleaseImage(&bin);
	return num;
}


int main(int argc, const char** argv)
{
	char* filename = "30.jpg";
	src = cvLoadImage(filename, 1);
	printf("[i] image: %s\n", filename);
	assert(src != 0);

	//char* filename2 = "10ex.jpg";
	
	dst = cvCloneImage(src);

	// покажем изображение
	cvNamedWindow("original", 1);
	cvShowImage("original", src);

	// находим круги на изображении
	int num = findCells(dst);
	std::cout << "Num of cells: " << num << "\n";

	cvNamedWindow("circles", 1);
	cvShowImage("circles", dst);

	// ждём нажатия клавиши
	cvWaitKey(0);

	// освобождаем ресурсы
	cvReleaseImage(&src);
	cvReleaseImage(&dst);
	// удаляем окна
	cvDestroyAllWindows();

	return 0;
}