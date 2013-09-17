#if !defined(IMAGE_PROCESSING_H)
#define IMAGE_PROCESSING_H

#include "image.h"

#include "opencv2/opencv.hpp"

namespace Imaging
{
	template <typename T>
	int GetOpenCvType(void);

	enum class Interpolation {NEAREST, LINEAR, AREA, CUBIC, LANCZO};

	/** Resizes image data from a source ROI, and copies the resized image data to
	destination image.
	
	The destination image is resized to exactly fit the result. */
	template <typename T>
	void Resize(ImageFrame<T> &imgSrc,
		const Region<typename ImageFrame<T>::SizeType, typename ImageFrame<T>::SizeType> &roiSrc,
		const Point2D<double> &zm, ImageFrame<T> &imgDst,
		Interpolation interp = Interpolation::LINEAR);

	/** Resizes the entire source image, and copies the resized image data to destination
	image. */
	template <typename T>
	void Resize(const ImageFrame<T> &imgSrc, const Point2D<double> &zm, ImageFrame<T> &imgDst,
		Interpolation interp = Interpolation::LINEAR);
}

#include "image_processing_inl.h"

#endif
