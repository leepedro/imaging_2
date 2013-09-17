/** This file contains the test functions to test classes and functions defined in
image_processing.h */

#include "../Imaging/image_processing.h"

#include <stdexcept>
#include <iostream>
#include <sstream>

#include "opencv2/opencv.hpp"

void TestImageProcessing(void)
{
	try
	{
		using namespace Imaging;

		cv::Mat cvSrc1 = cv::imread(std::string("Lenna.png"), CV_LOAD_IMAGE_COLOR);
		cv::namedWindow(std::string("Source 1"), CV_WINDOW_AUTOSIZE);
		cv::imshow(std::string("Source 1"), cvSrc1);
		cv::waitKey(0);		

		// Copy from cv::Mat object to ImageFrame<T>.
		ImageFrame<unsigned char> img1;
		img1.CopyFrom(cvSrc1.ptr(), cvSrc1.cols, cvSrc1.rows, cvSrc1.channels(),
			cvSrc1.channels() * cvSrc1.cols);

		// Copy from ImageFrame<T> to cv::Mat.
		cv::Mat cvDst1(SafeCast<int>(img1.size.height),
			SafeCast<int>(img1.size.width), CV_8UC3, cv::Scalar(0, 0, 0));
		Region<ImageFrame<unsigned char>::SizeType, ImageFrame<unsigned char>::SizeType> roiSrc1(0, 0, img1.size.width, img1.size.height);
#if defined(WIN32)
		std::copy(img1.data.cbegin(), img1.data.cend(),
			stdext::checked_array_iterator<unsigned char *>(cvDst1.ptr(), img1.data.size()));
#else
		std::copy(img1.data.cbegin(), img1.data.cend(), cvDst1.ptr());
#endif
		cv::namedWindow(std::string("Destination 1"), CV_WINDOW_AUTOSIZE);
		cv::imshow(std::string("Destination 1"), cvDst1);
		cv::waitKey(0);

		// Shared allocation of cv::Mat object from an ImageFrame<T>.
		auto it_img1 = img1.GetIterator(0, 0);
		cv::Mat cvDst2(SafeCast<int>(img1.size.height),
			SafeCast<int>(img1.size.width), CV_8UC3, &(*it_img1));
		cv::namedWindow(std::string("Destination 2"), CV_WINDOW_AUTOSIZE);
		cv::imshow(std::string("Destination 2"), cvDst2);
		cv::waitKey(0);

		// Resize the image.
		ImageFrame<unsigned char> img2;
		Resize(img1, roiSrc1, Point2D<double>(2.0, 2.0), img2);
		auto it_img2 = img2.GetIterator(0, 0);
		cv::Mat cvDst3(SafeCast<int>(img2.size.height), SafeCast<int>(img2.size.width), CV_8UC3, &(*it_img2));
		cv::namedWindow(std::string("Resized"), CV_WINDOW_AUTOSIZE);
		cv::imshow(std::string("Resized"), cvDst3);
		cv::waitKey(0);
	}
	catch (const std::exception &ex)
	{
		std::cout << ex.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Unknown exception" << std::endl;
	}
}