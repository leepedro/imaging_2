#if !defined(COORDINATES_INL_H)
#define COORDINATES_INL_H

namespace Imaging
{
	////////////////////////////////////////////////////////////////////////////////////////
	// Point2D<T>

	////////////////////////////////////////////////////////////////////////////////////////
	// Default constructors.
	template <typename T>
	Point2D<T>::Point2D(void) : x(this->at(0)), y(this->at(1)) {}

	template <typename T>
	Point2D<T>::Point2D(const Point2D<T> &src) :
#if defined(WIN32) && _MSC_VER <= 1700	// up to VS2012
		std::array<T, 2>(src), x(this->at(0)), y(this->at(1)) {}
#else	// C++11
		std::array<T, 2>(src), Point2D<T>() {}
#endif

	template <typename T>
	Point2D<T>::Point2D(Point2D<T> &&src) :
#if defined(WIN32) && _MSC_VER <= 1700	// up to VS2012
		x(this->at(0)), y(this->at(1))
#else	//C++11
		Point2D<T>()
#endif
	{
		this->swap(src);
	}

	/** Unifying assignment operator

	This operator replaces both copy and move assignment operators, and is called at the places
	of both copy and move assignment operators.
	If this operator is called at the place of copy assignment operator, the 'src' within the
	function block is already a product of copy constructor, so this syntax conforms the copy
	and swap idiom for copy assignment operators.
	If this operator is called at the place of move assignment operator, then the 'src' is an
	rvalue, so it will not invoke the copy constructor.

	Since this operator calls swap member fucntion of std::array<T, N> class, there should be
	NOT any member variable defined in this class.
	*/
	template <typename T>
	Point2D<T> &Point2D<T>::operator=(Point2D<T> src)
	{
		this->swap(src);	// std::array<T, N>::swap()
		return *this;
	}


	////////////////////////////////////////////////////////////////////////////////////////
	// Custom constructors.
	template <typename T>
	Point2D<T>::Point2D(T x, T y) :
#if defined(WIN32) && _MSC_VER <= 1700	// up to VS2012
		x(this->at(0)), y(this->at(1))
#else	// C++11
		Point2D<T>()
#endif
	{
		this->x = x;
		this->y = y;
	}

	/** This is a little risky implementation.
	If there was a member variable defined in this class and the source was a derived class
	from std::array<T, N> (other than this class) instead of std::array<T, N>, the member
	variable wouldn't be copied from the source.
	However, on the second thought, this is only as risky as using swap() in std::array<T, N>
	class.
	*/
	template <typename T>
	Point2D<T>::Point2D(const std::array<T, 2> &src) :
#if defined(WIN32) && _MSC_VER <= 1700	// up to VS2012
		std::array<T, 2>(src), x(this->at(0)), y(this->at(1)) {}
#else	// C++11
		std::array<T, 2>(src), Point2D<T>() {}
#endif

	////////////////////////////////////////////////////////////////////////////////////////
	// Point3D<T>

	////////////////////////////////////////////////////////////////////////////////////////
	// Default constructors.
	template <typename T>
	Point3D<T>::Point3D(void) : x(this->at(0)), y(this->at(1)), z(this->at(2)) {}

	template <typename T>
	Point3D<T>::Point3D(const Point3D<T> &src) :
#if defined(WIN32) && _MSC_VER <= 1700	// up to VS2012
		std::array<T, 3>(src), x(this->at(0)), y(this->at(1)), z(this->at(2)) {}
#else	// C++11
		std::array<T, 3>(src), Point3D<T>() {}
#endif

	template <typename T>
	Point3D<T> &Point3D<T>::operator=(const Point3D<T> &src)
	{
		//this->std::array<T, 3>::operator=(src);
		Point3D<T> temp(src);
		this->swap(temp);
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Custom constructors.
	template <typename T>
	Point3D<T>::Point3D(T x, T y, T z) :
#if defined(WIN32) && _MSC_VER <= 1700	// up to VS2012
		x(this->at(0)), y(this->at(1)), z(this->at(2))
#else	// C++11
		Point3D<T>()
#endif
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Size2D<T>

	////////////////////////////////////////////////////////////////////////////////////////
	// Default constructors.
	template <typename T>
	Size2D<T>::Size2D(void) : width(this->at(0)), height(this->at(1)) {}

	template <typename T>
	Size2D<T>::Size2D(const Size2D &src) :
#if defined(WIN32) && _MSC_VER <= 1700	// up to VS2012
		std::array<T, 2>(src), width(this->at(0)), height(this->at(1)) {}
#else	// C++11
		std::array<T, 2>(src), Size2D<T>() {}
#endif

	template <typename T>
	Size2D<T> &Size2D<T>::operator=(const Size2D<T> &src)
	{
		//this->std::array<T, 2>::operator=(src);
		Size2D<T> temp(src);
		this->swap(temp);
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Custom constructors.
	template <typename T>
	Size2D<T>::Size2D(T width, T height) :
#if defined(WIN32) && _MSC_VER <= 1700	// up to VS2012
		width(this->at(0)), height(this->at(1))
#else	// C++11
		Size2D<T>()
#endif
	{
		this->width = width;
		this->height = height;
	}

	/** This is a risky implementation.
	If there was a member variable defined in this class and the source was a derived class
	from std::array<T, N> (other than this class) instead of std::array<T, N>, the member
	variable wouldn't be copied from the source. */
	template <typename T>
	Size2D<T>::Size2D(const std::array<T, 2> &src) :
#if defined(WIN32) && _MSC_VER <= 1700	// up to VS2012
		std::array<T, 2>(src), width(this->at(0)), height(this->at(1)) {}
#else	// C++11
		std::array<T, 2>(src), Size2D<T>() {}
#endif

	////////////////////////////////////////////////////////////////////////////////////////
	// Size3D<T>

	////////////////////////////////////////////////////////////////////////////////////////
	// Default constructors.
	template <typename T>
	Size3D<T>::Size3D(void) : width(this->at(0)), height(this->at(1)), depth(this->at(2)) {}

	template <typename T>
	Size3D<T>::Size3D(const Size3D<T> &src) :
#if defined(WIN32) && _MSC_VER <= 1700	// up to VS2012
		std::array<T, 3>(src), width(this->at(0)), height(this->at(1)), depth(this->at(2)) {}
#else	// C++11
		std::array<T, 3>(src), Size3D<T>() {}
#endif

	template <typename T>
	Size3D<T> &Size3D<T>::operator=(const Size3D<T> &src)
	{
		//this->std::array<T, 3>::operator=(src);
		Size3D<T> temp(src);
		this->swap(temp);
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Custom constructors.
	template <typename T>
	Size3D<T>::Size3D(T width, T height, T depth) :
#if defined(WIN32) && _MSC_VER <= 1700	// up to VS2012
		width(this->at(0)), height(this->at(1)), depth(this->at(2))
#else	// C++11
		Size3D<T>()
#endif
	{
		this->width = width;
		this->height = height;
		this->depth = depth;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Region<T, U> class

	////////////////////////////////////////////////////////////////////////////////////////
	// Default constructors.

	template <typename T, typename U>
	Region<T, U>::Region(void) {}

	template <typename T, typename U>
	Region<T, U>::Region(const Region<T, U> &src) :
		origin(src.origin), size(src.size) {}

	template <typename T, typename U>
	Region<T, U>::Region(Region<T, U> &&src)
	{
		this->Swap(src);
	}

	template <typename T, typename U>
	Region<T, U> &Region<T, U>::operator=(Region<T, U> src)
	{
		this->Swap(src);
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Custom constructors.

	template <typename T, typename U>
	Region<T, U>::Region(const Point2D<T> &origin, const Size2D<U> &size) :
		origin(origin), size(size) {}

	// Delegation constructors are possible only from VS2013, so this won't work for now.
	template <typename T, typename U>
	Region<T, U>::Region(T x, T y, U width, U height) :
#if defined(WIN32) && _MSC_VER <= 1700	// up to VS2012
		origin(Point2D<T>(x, y)), size(Size2D<U>(width, height)) {}	// for VS2012
#else	// C++11
		Region<T, U>(Point2D<T>(x, y), Size2D<U>(width, height)) {}	// for VS2013
#endif

	////////////////////////////////////////////////////////////////////////////////////////
	// Operators.

	template <typename T, typename U>
	bool Region<T, U>::operator==(const Region<T, U> &rhs) const
	{
		if (this->origin == rhs.origin && this->size == rhs.size)
			return true;
		else
			return false;
	}

	template <typename T, typename U>
	bool Region<T, U>::operator!=(const Region<T, U> &rhs) const
	{
		return !this->operator==(rhs);
	}

	template <typename T, typename U>
	Region<T, U> Region<T, U>::operator+(const Point2D<T> &dist) const
	{
		Region<T, U> dst(*this);
		dst.Move(dist);
		return dst;
	}

	template <typename T, typename U>
	Region<T, U> Region<T, U>::operator*(const Point2D<double> &zm) const
	{
		Region<T, U> dst(*this);
		dst.Zoom(zm);
		return dst;
	}

	template <typename T, typename U>
	Region<T, U> Region<T, U>::operator*(double zm) const
	{
		Region<T, U> dst(*this);
		dst.Zoom(zm);
		return dst;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Methods.

	//template <typename T, typename U>
	//bool Region<T, U>::Fit(U width, U height) const
	//{
	//	return this->origin.x == 0 && this->origin.y == 0 &&
	//		this->size.width == width && this->size.height == height;
	//}


	template <typename T, typename U>
	void Region<T, U>::Move(const Point2D<T> &dist)
	{
		this->origin += dist;
	}

	template <typename T, typename U>
	void Region<T, U>::Swap(Region<T, U> &region)
	{
		this->origin.swap(region.origin);
		this->size.swap(region.size);
	}

	template <typename T, typename U>
	void Region<T, U>::Zoom(const Point2D<double> &zm)
	{
		RoundAs(this->size * zm, this->size);
	}

	template <typename T, typename U>
	void Region<T, U>::Zoom(double zm)
	{
		RoundAs(this->size * zm, this->size);
	}
}
#endif
