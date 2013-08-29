/** This file contains the test functions to test classes and functions defined in
coordinates.h */
#include "../Imaging/coordinates.h"

#include <stdexcept>
#include <iostream>

void TestPoint2D(void)
{
	// Point2D<T>
	using namespace Imaging;	
	Point2D<int> pt1, pt2(1, 2);
	pt1 = pt2;
	Point2D<int> pt3 = pt2;

	if (pt3 != pt2)
		throw std::logic_error("Point2D<T>");
}

void TestPoint3D(void)
{
	// Point3D<T>
	using namespace Imaging;
	Point3D<int> pt1, pt2(1, 2, 3);
	pt1 = pt2;
	Point3D<int> pt3 = pt2;

	if (pt3 != pt2)
		throw std::logic_error("Point3D<T>");
}

void TestSize2D(void)
{
	// Size2D<T>
	using namespace Imaging;	
	Size2D<int> sz1, sz2(1, 2);
	sz1 = sz2;
	Size2D<int> sz3 = sz2;

	if (sz3 != sz2)
		throw std::logic_error("Size2D<T>");
}

void TestSize3D(void)
{
	// Size3D<T>
	using namespace Imaging;	
	Size3D<int> sz1, sz2(1, 2, 3);
	sz1 = sz2;
	Size3D<int> sz3 = sz2;

	if (sz3 != sz2)
		throw std::logic_error("Size3D<T>");
}

template <typename T, typename U>
void TestRegion(T x, T y, U width, U height)
{
	using namespace Imaging;

	Region<T, U> roi1(x, y, width, height), roi2;
	roi2 = roi1;
	Region<T, U> roi3 = roi1;

	if (roi2 != roi1)
		throw std::logic_error("Region<T, U>");
	if (roi3 != roi1)
		throw std::logic_error("Region<T, U>");
	std::cout << "{(" << roi1.origin.x << ", " << roi1.origin.y << "), [" << roi1.size.width
		<< ", " << roi1.size.height << "]}" << std::endl;

	roi2 = roi1 + Point2D<T>(1, 1);
	std::cout << "Before moving: {(" << roi1.origin.x << ", " << roi1.origin.y << "), [" 
		<< roi1.size.width << ", " << roi1.size.height << "]}" << std::endl;
	std::cout << "After moving: {(" << roi2.origin.x << ", " << roi2.origin.y << "), ["
		<< roi2.size.width << ", " << roi2.size.height << "]}" << std::endl;

	roi3 = roi1 * 2.0;
	std::cout << "Before zooming: {(" << roi1.origin.x << ", " << roi1.origin.y << "), ["
		<< roi1.size.width << ", " << roi1.size.height << "]}" << std::endl;
	std::cout << "After zooming: {(" << roi3.origin.x << ", " << roi3.origin.y << "), ["
		<< roi3.size.width << ", " << roi3.size.height << "]}" << std::endl;

	roi3 = roi1 * Point2D<double>(2.0, 1.0);
	std::cout << "Before zooming: {(" << roi1.origin.x << ", " << roi1.origin.y << "), ["
		<< roi1.size.width << ", " << roi1.size.height << "]}" << std::endl;
	std::cout << "After zooming: {(" << roi3.origin.x << ", " << roi3.origin.y << "), ["
		<< roi3.size.width << ", " << roi3.size.height << "]}" << std::endl;
}

void TestCoordinates(void)
{
	std::cout << std::endl << "Test for coordinates.h has started." << std::endl;
	try
	{
		TestPoint2D();
		TestPoint3D();
		TestSize2D();
		TestSize3D();
		TestRegion<int, unsigned int>(0, 0, 4, 8);
		TestRegion<int, int>(-1, -1, 4, 8);
	}
	catch (const std::logic_error &ex)
	{
		std::cout << ex.what() << std::endl;
	}
	catch (...)
	{
		throw;
	}
	std::cout << "Test for coordinates.h has been completed." << std::endl;
}
