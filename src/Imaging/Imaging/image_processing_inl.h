#if !defined(IMAGE_PROCESSING_INL_H)
#define IMAGE_PROCESSING_INL_H

namespace Imaging
{
	template <typename T>
	int GetOpenCvType(::size_t depth)
	{
		if (typeid(T) == typeid(unsigned char))
			return SafeCast<int>(CV_MAKETYPE(CV_8U, depth));
		else if (typeid(T) == typeid(signed char))
			return SafeCast<int>(CV_MAKETYPE(CV_8S, depth));
		else if (typeid(T) == typeid(unsigned short))
			return SafeCast<int>(CV_MAKETYPE(CV_16U, depth));
		else if (typeid(T) == typeid(short))
			return SafeCast<int>(CV_MAKETYPE(CV_16S, depth));
		else if (typeid(T) == typeid(int))
			return SafeCast<int>(CV_MAKETYPE(CV_32S, depth));
		else if (typeid(T) == typeid(float))
			return SafeCast<int>(CV_MAKETYPE(CV_32F, depth));
		else if (typeid(T) == typeid(double))
			return SafeCast<int>(CV_MAKETYPE(CV_64F, depth));
		else
			throw std::invalid_argument("Unsupported type.");
	}

	/** The internal interpolation logic calls an OpenCV function, and this function creates
	a temporary cv::Mat objects from existing images without memory allocation. */
	template <typename T>
	void Resize(ImageFrame<T> &imgSrc,
		const Region<typename ImageFrame<T>::SizeType, typename ImageFrame<T>::SizeType> &roiSrc,
		const Point2D<double> &zm, ImageFrame<T> &imgDst,
		Interpolation interp)
	{
		// Reset destination image.
		Size2D<typename ImageFrame<T>::SizeType> szDst;
		RoundAs(roiSrc.size * zm, szDst);
		imgDst.Reset(szDst.width, szDst.height, imgSrc.depth);

		/* Create a temporary image for given source ROI to create cv::Mat objects.
		This stemp is necessary even if the given ROI is the entire image because we cannot
		create the temporary cv::Mat object from const &. */
		ImageFrame<T> imgTemp;
		imgSrc.CopyTo(roiSrc, imgTemp);

		// Prepare temporary cv::Mat objects without memory allocation.
		cv::Mat cvSrc(SafeCast<int>(imgTemp.size.height), SafeCast<int>(imgTemp.size.width),
			GetOpenCvType<T>(imgTemp.depth), imgTemp.GetPointer(0, 0));
		cv::Mat cvDst(SafeCast<int>(imgDst.size.height), SafeCast<int>(imgDst.size.width),
			GetOpenCvType<T>(imgDst.depth), imgDst.GetPointer(0, 0));

		cv::resize(cvSrc, cvDst, cvDst.size(), zm.x, zm.y, static_cast<int>(interp));
	}

	template <typename T>
	void Resize(const ImageFrame<T> &imgSrc, const Point2D<double> &zm, ImageFrame<T> &imgDst,
		Interpolation interp)
	{
		Region<typename ImageFrame<T>::SizeType, typename ImageFrame<T>::SizeType>
			roi(0, 0, imgSrc.size.width, imgSrc.size.height);
		Resize(imgSrc, roi, zm, imgDst, interp);
	}
}

#endif
