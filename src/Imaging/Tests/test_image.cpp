/** This file contains the test functions to test classes and functions defined in
image.h */

#include "../Imaging/image.h"

#include <stdexcept>
#include <iostream>
#include <sstream>

void TestConvert(void)
{
	using namespace Imaging;

	// TODO: dummy padded bytes.

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
	Size2D<ImageFrame<T>::SizeType> sz(width, height);
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
	img1.Resize(3, 2, 1);
	std::cout << "After change: width = " << img1.size.width << ", height = "
		<< img1.size.height << ", depth = " << img1.depth << std::endl;

	std::cout << "Before change: width = " << img4.size.width << ", height = "
		<< img4.size.height << ", depth = " << img4.depth << std::endl;
	img4.Clear();
	std::cout << "After change: width = " << img4.size.width << ", height = "
		<< img4.size.height << ", depth = " << img4.depth << std::endl;

	ImageFrame<T>::Iterator it = img1.GetIterator(1, 1);
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
