#if !defined(IMAGE_H)
#define IMAGE_H

#include <vector>

#include "coordinates.h"

namespace Imaging
{
	/** Presents image format by how the data is stored.
	
	BIP: band-interleaved by pixel or pixel interleaved, e.g., most of RGB bitmap images
	Data is stored as channel -> column -> row.
	BSQ: band-sequential or band interleaved, e.g., planar RGB images, time-sequenced image
	frames
	Data is stored as column -> row -> channel.
	BIL: band-interleaved by line or row interleaved, e.g., hyper-spectral images with a
	line sensor
	Data is stored as column -> channel -> row.
	*/
	enum class RawImageFormat {UNKNOWN, BIP, BSQ, BIL};

	/** Pixel-based bitmap (raster) image.

	This class stores image data as a std::vector<T> object, so it does NOT need to release
	the memory at the destructor and it does NOT support padding bytes.
	The value of image data can be modifed externally by references and iterators of the
	std::vector<T> object.
	The dimension of image data can be changed externally by designated member functions.
	Of course, the member functions of a different object of same class still have full
	access.

	Image pixel values are always stored in the following order. 
	channel -> pixel -> line -> frame

	The terms used to describe the dimension of data are following.
	depth: number of channels per pixel
	width: number of pixels per line
	height: number of lines per frame
	length: number of frames per block (not used in this class)
	c: position of a channel at given pixel; [0 ~ depth)
	x: position of a pixel at given line; [0 ~ width)
	y: position of a line at given frame; [0 ~ height)
	*/
	template <typename T>
	class ImageFrame
	{
	public:
		//////////////////////////////////////////////////
		// Types and constants.
		typedef typename std::vector<T>::size_type SizeType;
		typedef typename std::vector<T>::iterator Iterator;
		typedef typename std::vector<T>::const_iterator ConstIterator;

		//////////////////////////////////////////////////
		// Default constructors.
		ImageFrame(void);
		ImageFrame(const ImageFrame<T> &src);
		ImageFrame(ImageFrame<T> &&src);
		ImageFrame<T> &operator=(ImageFrame<T> src);

		//////////////////////////////////////////////////
		// Custom constructors.
		ImageFrame(const Size2D<SizeType> &sz, SizeType d = 1);
		ImageFrame(SizeType w, SizeType h, SizeType d = 1);

		//////////////////////////////////////////////////
		// Accessors.

		/** Accesses image data for given coordinate (x, y, c) by an iterator. */
		Iterator GetIterator(SizeType x, SizeType y, SizeType c = 0);
		ConstIterator GetIterator(SizeType x, SizeType y, SizeType c = 0) const;

		/** Accesses image data for given coordinate (x, y, c) by a pointer. */
		T *GetPointer(SizeType x, SizeType y, SizeType c = 0);
		const T *GetPointer(SizeType x, SizeType y, SizeType c = 0) const;

		const std::vector<T> &data;
		const SizeType &depth;
		const Size2D<SizeType> &size;

		//////////////////////////////////////////////////
		// Methods.

		/** Copies the image data of an ROI of a source image to another ROI
		of this image.
		
		@NOTE destination image must already have been allocated. */
		void CopyFrom(const ImageFrame<T> &imgSrc,
			const Region<SizeType, SizeType> &roiSrc,
			const Point2D<SizeType> &orgnDst);

		/** Copies an entire image from a raw data block with zero padding.
		
		@NOTE destination image will be reallocated based on the size of source
		image.
		
		TODO: different conversion scenarios */
		void CopyFrom(const T *src, const Size2D<typename ImageFrame<T>::SizeType> &sz,
			typename ImageFrame<T>::SizeType depth, ::size_t bytesPerLine,
			RawImageFormat fmt = RawImageFormat::BIP);

		/** Copies the image data of this image to a destination image.
		
		@NOTE destination image will be resized based on the size of the source
		ROI. */
		void CopyTo(const Region<SizeType, SizeType> &roiSrc,
			ImageFrame<T> &imgDst) const;

		void Clear(void);
		void Resize(const Size2D<SizeType> &sz, SizeType d = 1);
		void Resize(SizeType w, SizeType h, SizeType d = 1);

		void CheckDepth(SizeType c) const;	// move to protected?
		void CheckRange(SizeType c) const;	// move to protected?
		void CheckRange(SizeType x, SizeType y) const;	// move to protected?

	protected:
		//////////////////////////////////////////////////
		// Methods.
		void CheckRange(const Point2D<SizeType> &orgn, const Size2D<SizeType> &sz) const;
		void CheckRange(const Region<SizeType, SizeType> &roi) const;
		SizeType GetOffset(SizeType x, SizeType y, SizeType c = 0) const;
		void Swap(ImageFrame<T> &src);

		//////////////////////////////////////////////////
		// Data.
		std::vector<T> data_;
		SizeType depth_;
		Size2D<SizeType> size_;

	};
}

#include "image_inl.h"


#endif
