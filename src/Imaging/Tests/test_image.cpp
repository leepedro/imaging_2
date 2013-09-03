/** This file contains the test functions to test classes and functions defined in
image.h */

#include "../Imaging/image.h"

#include <stdexcept>
#include <iostream>
#include <sstream>

template <typename T>
void TestImageFrame(::size_t width, ::size_t height, ::size_t depth = 1)
{
	using namespace Imaging;

	ImageFrame<T> img1(width, height, depth);	// custom constructor
	ImageFrame<T> img2;	// default constructor
	img2 = img1;	// copy constructor + unifying assignment
	ImageFrame<T> img3 = img1;	// copy constructor

	if (img2.size != img1.size)
		throw std::logic_error("ImageFrame<T>");
	if (img3.size != img1.size)
		throw std::logic_error("ImageFrame<T>");

	img3 = ImageFrame<T>(width, height, depth);	// unifying assignment for moving
	ImageFrame<T> img4 = std::move(img2);	// move constructor

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

	std::cout << std::endl << "Test for image.h has been completed." << std::endl;
}
