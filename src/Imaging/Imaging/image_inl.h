#if !defined(IMAGE_INL_H)
#define IMAGE_INL_H

namespace Imaging
{
	template <typename T>
	void CopyLines(typename std::vector<T>::const_iterator it_src,
		typename std::vector<T>::size_type nElemPerLineSrc,
		typename std::vector<T>::iterator it_dst,
		typename std::vector<T>::size_type nElemPerLineDst,
		typename std::vector<T>::size_type nElemWidth,
		typename std::vector<T>::size_type nLines)
	{
		for (auto H = 0; H != nLines; ++H)
		{
			std::copy(it_src, it_src + nElemWidth, it_dst);
			it_src += nElemPerLineSrc;
			it_dst += nElemPerLineDst;
		}
	}


	/** Implemented stdext::checked_array_iterator<> class for Visual Studio to bypass C4996
	warning. */
	template <typename T>
	void CopyLines(typename std::vector<T>::const_iterator it_src, ::size_t nElemPerLineSrc,
		T *dst, ::size_t nElemPerLineDst, ::size_t nElemWidth, ::size_t nLines)
	{
		for (auto H = 0; H != nLines; ++H)
		{
#if defined(WIN32)
			std::copy(it_src, it_src + nElemWidth,
				stdext::checked_array_iterator<T *>(dst, nElemPerLineDst));
#else
			std::copy(it_src, it_src + nElemWidth, dst);
#endif
			it_src += nElemPerLineSrc;
			dst += nElemPerLineDst;
		}
	}

	/** Check the dimension of source and desitination data, and skip the
	boundary check by using [] instead of at(). */
	template <typename T>
	void BsqToBip(const std::vector<T> &src,
		typename std::vector<T>::size_type nBands,
		typename std::vector<T>::size_type nSamplesPerBand, std::vector<T> &dst)
	{
		// Check the size of source/destination.
		auto totalCount = nBands * nSamplesPerBand;
		if (src.size() != totalCount || dst.size() != totalCount)
			throw std::runtime_error(
			"The size of source or destination block is unmatched for given dimension.");

		for (auto B = 0; B != nBands; ++B)
		{
			auto it_src = src.cbegin() + nSamplesPerBand * B;			
			for (auto inc = 0; inc != nSamplesPerBand; ++inc, ++it_src)
				dst[nBands * inc + B] = *it_src;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// ImageFrame<T> class

	////////////////////////////////////////////////////////////////////////////////////////
	// Default constructors.
	template <typename T>
	ImageFrame<T>::ImageFrame(void) : data(data_), depth(depth_), size(size_),
		depth_(0), size_(Size2D<SizeType>(0, 0)) {}

	template <typename T>
	ImageFrame<T>::ImageFrame(const ImageFrame<T> &src) :
#if defined(WIN32) && _MSC_VER <= 1700	// up to VS2012
		data(data_), depth(depth_), size(size_),
#else	// C++11
		ImageFrame<T>(),
#endif
		data_(src.data)
	{
		this->depth_= src.depth;
		this->size_ = src.size;
	}

	template <typename T>
	ImageFrame<T>::ImageFrame(ImageFrame<T> &&src) :
#if defined(WIN32) && _MSC_VER <= 1700	// up to VS2012
		data(data_), depth(depth_), size(size_)
#else	// C++11
		ImageFrame<T>()
#endif
	{
		this->Clear();
		this->Swap(src);
	}

	template <typename T>
	ImageFrame<T> &ImageFrame<T>::operator=(ImageFrame<T> src)
	{
		this->Swap(src);
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Custom constructors.
	template <typename T>
	ImageFrame<T>::ImageFrame(const Size2D<SizeType> &sz, SizeType d) :
#if defined(WIN32) && _MSC_VER <= 1700	// up to VS2012
		data(data_), depth(depth_), size(size_)
#else	// C+11
		ImageFrame<T>()
#endif
	{
		this->Resize(sz, d);
	}

	template <typename T>
	ImageFrame<T>::ImageFrame(SizeType w, SizeType h, SizeType d) :
#if defined(WIN32) && _MSC_VER <= 1700	// up to VS2012
		data(data_), depth(depth_), size(size_)
#else	// C+11
		ImageFrame<T>()
#endif
	{
		this->Resize(w, h, d);
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Accessors.

	template <typename T>
	typename ImageFrame<T>::Iterator ImageFrame<T>::GetIterator(SizeType x, SizeType y,
		SizeType c)
	{
		this->CheckRange(c);
		this->CheckRange(x, y);
		return this->data_.begin() + this->GetOffset(x, y, c);
	}

	template <typename T>
	typename ImageFrame<T>::ConstIterator ImageFrame<T>::GetIterator(SizeType x, SizeType y,
		SizeType c) const
	{
		this->CheckRange(c);
		this->CheckRange(x, y);
		return this->data.cbegin() + this->GetOffset(x, y, c);
	}

	template <typename T>
	T *ImageFrame<T>::GetPointer(SizeType x, SizeType y, SizeType c)
	{
		auto it = this->GetIterator(x, y, c);
		return &(*it);
	}

	template <typename T>
	const T *ImageFrame<T>::GetPointer(SizeType x, SizeType y, SizeType c) const
	{
		auto it = this->GetIterator(x, y, c);
		return &(*it);
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Methods.

	template <typename T>
	void ImageFrame<T>::CopyFrom(const ImageFrame<T> &imgSrc,
		const Region<typename ImageFrame<T>::SizeType, typename ImageFrame<T>::SizeType> &roiSrc,
		const Point2D<typename ImageFrame<T>::SizeType> &orgnDst)
	{
		// Check the depth of both images.
		this->CheckDepth(imgSrc.depth);
		
		// Check source/destination ROI.
		imgSrc.CheckRange(roiSrc);	// TODO: Change CheckRange() into protected, and try again.
		this->CheckRange(orgnDst, roiSrc.size);

		// Copy line by line.
		auto it_src = imgSrc.GetIterator(roiSrc.origin.x, roiSrc.origin.y);
		auto it_dst = this->GetIterator(orgnDst.x, orgnDst.y);
		CopyLines<T>(it_src, imgSrc.depth * imgSrc.size.width, it_dst,
			this->depth * this->size.width, this->depth * roiSrc.size.width,
			roiSrc.size.height);
	}

	template <typename T>
	void ImageFrame<T>::CopyFrom(const T *src,
		const Size2D<typename ImageFrame<T>::SizeType> &sz,
		typename ImageFrame<T>::SizeType depth, ::size_t bytesPerLine,
		RawImageFormat fmt)
	{
		if (bytesPerLine == sz.width * sizeof(T))	// no zero padding
			;
		else// with zero padding
		{
			// Reset destination image for given dimension.
			this->resize(sz, depth);

			::size_t nElemPerLine = bytesPerLine / sizeof(T);	// ?
			switch (fmt)
			{
			case Imaging::RawImageFormat::BIP:
				{
					auto it_dst = this->GetIterator(0, 0);
					CopyLines<T>(src, nElemPerLine, it_dst,
						this->depth * this->size.width, depth * sz.width,
						sz.height);
				}
				break;
			case Imaging::RawImageFormat::BSQ:
				//for (auto C = 0; C != depth; ++C)
				//{
				//	auto it_dst = this->GetIterator(0, 0, C);
				//	CopyLines<T>(src + nElemPerLine * sz.height * C, nElemPerLine, it_dst,
				//		this->size.width, sz.width, sz.height);
				//}
				break;
			//case Imaging::RawImageFormat::BIL:
			//	{
			//		auto it_dst = imgDst.GetIterator(0, 0);
			//		CopyLines<T>(src, nElemWidthSrc, it_dst, imgDst.size.width,
			//			sz.width, depth * sz.height);
			//	}
			//	break;
			default:
				std::ostringstream errMsg;
				errMsg << "Raw image format " << static_cast<int>(fmt) <<
					" is not supported.";
				throw std::logic_error(errMsg.str());
			}

		}
	}

	template <typename T>
	void ImageFrame<T>::CopyFrom(const T *src, const Size2D<typename ImageFrame<T>::SizeType> &sz,
		typename ImageFrame<T>::SizeType depth,	RawImageFormat fmt)
	{
		// Reset destination image for given dimension.
		this->resize(sz, depth);	
		
		// Copy image data pixel by pixel.
		switch (fmt)
		{
		case Imaging::RawImageFormat::BIP:
			auto it_dst = this->GetIterator(0, 0);
			auto nElem = depth * sz.width * sz.height;
			std::copy(src, src + nElem, it_dst);
			break;
		case Imaging::RawImageFormat::BSQ:
			for (auto C = 0; C != depth; ++C)
			{
				auto it_dst = this->GetIterator(0, 0, C);
				for (auto endSrc = src + sz.width * sz.height; src == endSrc; ++src)
				{
					*it_dst = *src;
					it_dst += depth;
				}
			}
			break;
		case Imaging::RawImageFormat::BIL:
			for (auto R = 0; R != sz.height; ++R)
				for (auto C = 0; C != depth; ++C)
				{
					auto it_dst = this->GetIterator(0, R, C);
					for (auto endSrc = src + sz.width; src == endSrc; ++src)
					{
						*it_dst = *src;
						it_dst += depth;
					}
				}
			break;
		case Imaging::RawImageFormat::UNKNOWN:
		default:
			std::ostringstream errMsg;
			errMsg << "Raw image format " << static_cast<int>(fmt) <<
				" is not supported.";
			throw std::logic_error(errMsg.str());
			break;
		}		
	}

	template <typename T>
	void ImageFrame<T>::CopyTo(const Region<SizeType, SizeType> &roiSrc,
		ImageFrame<T> &imgDst) const
	{
		// Reset destination image for given dimension.
		imgDst.resize(roiSrc.size.width, roiSrc.size.height, this->depth);

		// Check source/destination ROI.
		this->CheckRange(roiSrc);

		// Copy line by line.
		auto it_src = this->GetIterator(roiSrc.origin.x, roiSrc.origin.y);
		auto it_dst = imgDst.GetIterator(0, 0);
		CopyLines<T>(it_src, this->depth * this->size.width, it_dst,
			imgDst.depth * imgDst.size.width, this->depth * roiSrc.size.width,
			roiSrc.size.height);
	}
	
	template <typename T>
	void ImageFrame<T>::CheckDepth(SizeType c) const
	{
		if (this->depth != c)
			throw std::runtime_error("Depth is not matched.");
	}

	/** Throws an exception instead of returning false because you have to throw an
	exception at a higher level any way. */
	template <typename T>
	void ImageFrame<T>::CheckRange(SizeType c) const
	{
		if (c < 0 || c >= this->depth)
		{
			std::ostringstream errMsg;
			errMsg << "Channel c = " << c << " is out of range.";
			throw std::out_of_range(errMsg.str());
		}
	}

	template <typename T>
	void ImageFrame<T>::CheckRange(SizeType x, SizeType y) const
	{
		if (x < 0 || x >= this->size.width || y < 0 || y >= this->size.height)
		{
			std::ostringstream errMsg;
			errMsg << "Position (" << x << ", " << y << ") is out of range.";
			throw std::out_of_range(errMsg.str());
		}
	}

	// The end points are the excluding end of an ROI, so it could be up to (width, height).
	template <typename T>
	void ImageFrame<T>::CheckRange(const Point2D<SizeType> &orgn, const Size2D<SizeType> &sz) const
	{
		Point2D<SizeType> ptEnd = orgn + sz;
		if (orgn.x < 0 || ptEnd.x > this->size.width || orgn.y < 0 ||
			ptEnd.y > this->size.height)
		{
			std::ostringstream errMsg;
			errMsg << "[" << orgn.x << ", " << orgn.y << "] ~ (" << ptEnd.x << ", " <<
				ptEnd.y << ") is out of range.";
			throw std::out_of_range(errMsg.str());
		}
	}

	template <typename T>
	void ImageFrame<T>::CheckRange(const Region<SizeType, SizeType> &roi) const
	{
		this->CheckRange(roi.origin, roi.size);
	}

	template <typename T>
	void ImageFrame<T>::Clear(void)
	{
		this->data_.clear();
		this->depth_ = 0;
		this->size_ = Size2D<SizeType>(0, 0);
	}

	template <typename T>
	typename ImageFrame<T>::SizeType ImageFrame<T>::GetOffset(SizeType x, SizeType y,
		SizeType c) const
	{
		return c + this->depth * x + this->depth * this->size.width * y;
	}

	/** Resizes the std::vector<T> object only if necessary.
	If size is changed while the total number of elements are the same (reshaping),
	it does NOT run resize() function of the std::vector<T>. */
	template <typename T>
	void ImageFrame<T>::Resize(const Size2D<SizeType> &sz, SizeType d)
	{
		SizeType nElem = sz.width * sz.height * d;
		if (this->data.size() != nElem)
			this->data_.resize(nElem);
		this->depth_ = d;
		this->size_ = sz;
	}

	template <typename T>
	void ImageFrame<T>::Resize(SizeType w, SizeType h, SizeType d)
	{
		this->Resize(Size2D<SizeType>(w, h), d);
	}

	template <typename T>
	void ImageFrame<T>::Swap(ImageFrame<T> &src)
	{
		this->data_.swap(src.data_);
		std::swap(this->depth_, src.depth_);
		std::swap(this->size_, src.size_);
	}
}

#endif
