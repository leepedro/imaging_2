#if !defined(IMAGE_H)
#define IMAGE_H

#include <vector>

#include "coordinates.h"

namespace Imaging
{
	// TODO
	/** Copies image data from a void raw pointer to an std::vector<T> object
	after taking out dummy padding bytes. */
	template <typename T>
	void Copy(const void *src, ::size_t width, ::size_t height, ::size_t depth,
		::size_t bytesPerLine, std::vector<T> &dst);

	/** Copies lines of data repeatedly from an std::vector<T> to another.
	
	This function is usually used to copy an ROI of data where an image is stored in an
	std::vector<T>. */
	template <typename T>
	void CopyLines(typename std::vector<T>::const_iterator it_src,
		typename std::vector<T>::size_type nElemPerLineSrc,
		typename std::vector<T>::iterator it_dst,
		typename std::vector<T>::size_type nElemPerLineDst,
		typename std::vector<T>::size_type nElemWidth,
		typename std::vector<T>::size_type nLines);

	/** Copies lines of data repeatedly from an std::vector<T> to a raw pointer. */
	template <typename T>
	void CopyLines(typename std::vector<T>::const_iterator it_src, ::size_t nElemPerLineSrc,
		T *dst, ::size_t nElemPerLineDst, ::size_t nElemWidth, ::size_t nLines);

	/** Reorganizes data samples in std::vector<T> from BSQ to BIP format.
	
	Since data samples in source data is continuous through the whole band, the number of
	data samples/band (instead of number of data samples/line and lines/band) is sufficient
	information. */
	template <typename T>
	void BsqToBip(const std::vector<T> &src,
		typename std::vector<T>::size_type nBands,
		typename std::vector<T>::size_type nSamplesPerBand, std::vector<T> &dst);

	/** Reorganizes data samples in std::vector<T> from BIL to BIP format.
	
	Since data samples in source data is continuous through only one line, number of data
	samples/line and lines/band is required. */
	template <typename T>
	void BilToBip(const std::vector<T> &src,
		typename std::vector<T>::size_type nBands,
		typename std::vector<T>::size_type nSamplesPerLine,
		typename std::vector<T>::size_type nLinesPerBand, std::vector<T> &dst);

	// TODO: Convert BIP to BSQ

	// TODO: Convert BIL to BSQ

	// TODO: Convert BSQ to BIL (?)

	// TODO: Convert BIP to BIL (?)

	// TODO: Copy image data from raw pointer to an std::vector<T>.


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
		ImageFrame(const std::vector<T> &src, const Size2D<SizeType> &sz, SizeType d);
		ImageFrame(std::vector<T> &&src, const Size2D<SizeType> &sz, SizeType d);

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

		/** Copies an entire image from a raw data block WITH zero padding.
		
		@NOTE destination image will be reallocated based on the size of source
		image.
		
		TODO: different conversion scenarios */
		void CopyFrom(const T *src, const Size2D<typename ImageFrame<T>::SizeType> &sz,
			typename ImageFrame<T>::SizeType depth, ::size_t bytesPerLine,
			RawImageFormat fmt = RawImageFormat::BIP);

		/** Copies an entire image from a raw data block WITHOUT zero padding.
		
		@NOTE destination is reallocated based on the size of source image. */
		void CopyFrom(const T *src, const Size2D<typename ImageFrame<T>::SizeType> &sz,
			typename ImageFrame<T>::SizeType d,
			RawImageFormat fmt = RawImageFormat::BIP);

		/** Copies image data from an std::vector<T> object.
		
		The correct dimension must be given. */
		void CopyFrom(const std::vector<T> &src, const Size2D<SizeType> &sz, SizeType d);
		void CopyFrom(const std::vector<T> &src, SizeType w, SizeType h, SizeType d);

		/** Moves image data from an std::vector<T> object.
		
		The correct dimension must be given. */
		void MoveFrom(std::vector<T> &&src, const Size2D<SizeType> &sz, SizeType d);
		void MoveFrom(std::vector<T> &&src, SizeType w, SizeType h, SizeType d);

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
