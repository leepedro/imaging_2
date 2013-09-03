#if !defined(IMAGE_H)
#define IMAGE_H

#include <vector>

#include "coordinates.h"

namespace Imaging
{
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
		void Clear(void);
		void Resize(const Size2D<SizeType> &sz, SizeType d = 1);
		void Resize(SizeType w, SizeType h, SizeType d = 1);

		void CheckRange(SizeType c) const;	// move to protedted?
		void CheckRange(SizeType x, SizeType y) const;	// move to protedted?

	protected:
		//////////////////////////////////////////////////
		// Methods.
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
