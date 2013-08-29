#include "../Imaging/image.h"

#include "tests.h"

#include <stdexcept>
#include <iostream>

int main(void)
{
	try
	{
		//TestUtilities();
		//TestCoordinates();
		//TestImage();
		TestImageFrame();
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