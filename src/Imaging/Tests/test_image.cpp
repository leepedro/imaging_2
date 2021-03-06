/** This file contains the test functions to test classes and functions defined in
image.h */

#include "../Imaging/image.h"

#include <stdexcept>
#include <iostream>
#include <sstream>

#include "opencv2/opencv.hpp"

void TestDummyBytes(void)
{
	// {int x 3 channel x 4 pixel} x 2 lines -> 48 bytes/line x 2 lines = 96 bytes
	// {32 bytes x 2} x 2 lines -> 64 bytes/line x 2 lines = 128 bytes

	// Make source data.
	std::array<int, 24> src;
	for (int I = 0; I != src.size(); ++I)
		src[I] = I;

	// Copy to source data with padding bytes.
	char *raw_1 = new char[128];	// 128 bytes

	// Copy 48 bytes (for 12 elements) to the first pointer.
	// Then, copy the next 48 bytes (for 12 elements) to a pointer after 32 x 2 = 64 bytes from
	// last pointer.
#if defined(WIN32)
	::memcpy_s(raw_1, 128, src.data(), 48);
	::memcpy_s(raw_1 + 64, 64, &*(src.cbegin() + 12), 48);
#else
	::memcpy(raw_1, src.data(), 48);
	::memcpy(raw_1 + 64, &*(src.cbegin() + 12), 48);
#endif

	// Copy data from a raw pointer with padded bytes to an std::vector<T>.
	std::vector<int> dst1;
	Imaging::Copy(raw_1, 4, 2, 3, 64, dst1);
	delete [] raw_1;
}

void TestConvert(void)
{
	using namespace Imaging;

	TestDummyBytes();

	std::vector<unsigned int> imgBsq1(24), imgBip1(24), imgBip2(24);
	for (unsigned int I = 0; I != imgBsq1.size(); ++I)
		imgBsq1[I] = I;
	std::vector<unsigned int> imgBil1 = imgBsq1;

	BsqToBip(imgBsq1, 3, 8, imgBip1);
	BilToBip(imgBil1, 3, 4, 2, imgBip2);
}

template <typename T>
void TestImageFrame(::size_t width, ::size_t height, ::size_t depth = 1)
{
	using namespace Imaging;

	ImageFrame<T> img1(width, height, depth);	// custom constructor
	ImageFrame<T> img2;	// default constructor
	img2 = img1;	// copy constructor + unifying assignment (for copy)
	ImageFrame<T> img3 = img1;	// copy constructor

	if (img2.size != img1.size)
		throw std::logic_error("ImageFrame<T>");
	if (img3.size != img1.size)
		throw std::logic_error("ImageFrame<T>");

	img3 = ImageFrame<T>(width, height, depth);	// custom constructor + unifying assignment (for moving)
	ImageFrame<T> img4 = std::move(img2);	// move constructor
	img4 = std::move(img3);	// move constructor + unifying assignment (for move)

	std::vector<T> src1(depth * width * height, 1), src2(depth * width * height, 2);
	Size2D<typename ImageFrame<T>::SizeType> sz(width, height);
	ImageFrame<T> img5(src1, sz, depth);
	ImageFrame<T> img6(std::move(src2), sz, depth);

	try
	{
		img1.CheckRange(0, 0);
		std::cout << "(0, 0) is within the range." << std::endl;
		img1.CheckRange(width, height);	// detected.
		// Following lines should NOT run.
		std::ostringstream msg;
		msg << "(" << width << ", " << height << ") is within the range." << std::endl;
		std::cout << msg.str() << std::endl;
		throw std::logic_error("Image<T>");
	}
	catch (const std::out_of_range &ex)
	{
		std::cout << ex.what() << std::endl;
	}
	catch (...)
	{
		throw;
	}

	//std::cout << "Before change: I(0, 0) = " << img1(0, 0) << std::endl;
	//img1(0, 0) = static_cast<T>(1);
	//std::cout << "After change: I(0, 0) = " << img1(0, 0) << std::endl;

	std::cout << "Before change: width = " << img1.size.width << ", height = "
		<< img1.size.height << ", depth = " << img1.depth << std::endl;
	img1.Reset(3, 2, 1);
	std::cout << "After change: width = " << img1.size.width << ", height = "
		<< img1.size.height << ", depth = " << img1.depth << std::endl;

	std::cout << "Before change: width = " << img4.size.width << ", height = "
		<< img4.size.height << ", depth = " << img4.depth << std::endl;
	img4.Clear();
	std::cout << "After change: width = " << img4.size.width << ", height = "
		<< img4.size.height << ", depth = " << img4.depth << std::endl;

	typename ImageFrame<T>::Iterator it = img1.GetIterator(1, 1);
}

void TestImageFrames(void)
{
	using namespace Imaging;

	std::cout << std::endl << "Test for image.h has started." << std::endl;

	TestImageFrame<unsigned char>(32, 16, 3);
	TestImageFrame<int>(32, 16);

	try
	{
		ImageFrame<unsigned char> img1(32, 28, 1), img2(16, 16, 1);
		Region<::size_t, ::size_t> roi1(1, 1, 3, 2);
		img1.CopyFrom(img2, roi1, Point2D<::size_t>(1, 1));
	}
	catch (const std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Unknown error" << std::endl;
	}

	std::cout << std::endl << "Test for image.h has been completed." << std::endl;
}

//void TestImage(void)
//{
//	try
//	{
//		using namespace Imaging;
//
//		cv::Mat cvSrc1 = cv::imread(std::string("Lenna.png"), CV_LOAD_IMAGE_COLOR);
//		cv::namedWindow(std::string("Source 1"), CV_WINDOW_AUTOSIZE);
//		cv::imshow(std::string("Source 1"), cvSrc1);
//		cv::waitKey(0);		
//
//		// Copy from cv::Mat object to ImageFrame<T>.
//		ImageFrame<unsigned char> img1;
//		img1.CopyFrom(cvSrc1.ptr(), cvSrc1.cols, cvSrc1.rows, cvSrc1.channels(),
//			cvSrc1.channels() * cvSrc1.cols);
//
//		// Copy from ImageFrame<T> to cv::Mat.
//		cv::Mat cvDst1(SafeCast<int>(img1.size.height),
//			SafeCast<int>(img1.size.width), CV_8UC3, cv::Scalar(0, 0, 0));
//		Region<ImageFrame<unsigned char>::SizeType, ImageFrame<unsigned char>::SizeType> roiSrc1(0, 0, img1.size.width, img1.size.height);
//#if defined(WIN32)
//		std::copy(img1.data.cbegin(), img1.data.cend(),
//			stdext::checked_array_iterator<unsigned char *>(cvDst1.ptr(), img1.data.size()));
//#else
//		std::copy(img1.data.cbegin(), img1.data.cend(), cvDst1.ptr());
//#endif
//		cv::namedWindow(std::string("Destination 1"), CV_WINDOW_AUTOSIZE);
//		cv::imshow(std::string("Destination 1"), cvDst1);
//		cv::waitKey(0);
//
//		// Shared allocation of cv::Mat object from an ImageFrame<T>.
//		auto it_img1 = img1.GetIterator(0, 0);
//		cv::Mat cvDst2(SafeCast<int>(img1.size.height),
//			SafeCast<int>(img1.size.width), CV_8UC3, &(*it_img1));
//		cv::namedWindow(std::string("Destination 2"), CV_WINDOW_AUTOSIZE);
//		cv::imshow(std::string("Destination 2"), cvDst2);
//		cv::waitKey(0);
//	}
//	catch (const std::exception &ex)
//	{
//		std::cout << ex.what() << std::endl;
//	}
//	catch (...)
//	{
//		std::cout << "Unknown exception" << std::endl;
//	}
//}
