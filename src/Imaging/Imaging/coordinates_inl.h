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
	Point2D<T>::Point2D(const Point2D &src) :
#if _MSC_VER > 1700	// from VS2013	
		std::array<T, 2>(src), Point2D<T>() {}
#else				// up to VS2012
		std::array<T, 2>(src), x(this->at(0)), y(this->at(1)) {}
#endif

	template <typename T>
	Point2D<T> &Point2D<T>::operator=(const Point2D<T> &src)
	{
		this->std::array<T, 2>::operator=(src);
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Custom constructors.
	template <typename T>
	Point2D<T>::Point2D(T x, T y) : 
#if _MSC_VER > 1700	// from VS2013
		Point2D<T>()
#else				// up to VS2012
		x(this->at(0)), y(this->at(1))
#endif
	{
		this->x = x;
		this->y = y;
	}

	/** This is a risky implementation because if there is a member variable defined in this
	class they won't be copied from the source if the source is a derived class from
	std::array<T, N> instead of std::array<T, N>. */
	template <typename T>
	Point2D<T>::Point2D(const std::array<T, 2> &src) :
#if _MSC_VER > 1700	// from VS2013	
		std::array<T, 2>(src), Point2D<T>() {}
#else				// up to VS2012
		std::array<T, 2>(src), x(this->at(0)), y(this->at(1)) {}
#endif

	////////////////////////////////////////////////////////////////////////////////////////
	// Point3D<T>

	////////////////////////////////////////////////////////////////////////////////////////
	// Default constructors.
	template <typename T>
	Point3D<T>::Point3D(void) : x(this->at(0)), y(this->at(1)), z(this->at(2)) {}

	template <typename T>
	Point3D<T>::Point3D(const Point3D<T> &src) :
#if _MSC_VER > 1700	// from VS2013	
		std::array<T, 3>(src), Point3D<T>() {}
#else				// up to VS2012
		std::array<T, 3>(src), x(this->at(0)), y(this->at(1)), z(this->at(2)) {}
#endif

	template <typename T>
	Point3D<T> &Point3D<T>::operator=(const Point3D<T> &src)
	{
		this->std::array<T, 3>::operator=(src);
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Custom constructors.
	template <typename T>
	Point3D<T>::Point3D(T x, T y, T z) :
#if _MSC_VER > 1700	// from VS2013
		Point3D<T>()
#else				// up to VS2012
		x(this->at(0)), y(this->at(1)), z(this->at(2))
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
#if _MSC_VER > 1700	// from VS2013	
		std::array<T, 2>(src), Size2D<T>() {}
#else				// up to VS2012
		std::array<T, 2>(src), width(this->at(0)), height(this->at(1)) {}
#endif

	template <typename T>
	Size2D<T> &Size2D<T>::operator=(const Size2D<T> &src)
	{
		this->std::array<T, 2>::operator=(src);
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Custom constructors.
	template <typename T>
	Size2D<T>::Size2D(T width, T height) : 
#if _MSC_VER > 1700	// from VS2013
		Size2D<T>()
#else				// up to VS2012
		width(this->at(0)), height(this->at(1))
#endif
	{
		this->width = width;
		this->height = height;
	}

	/** This is a risky implementation because if there is a member variable defined in this
	class they won't be copied from the source if the source is a derived class from
	std::array<T, N> instead of std::array<T, N>. */
	template <typename T>
	Size2D<T>::Size2D(const std::array<T, 2> &src) :
#if _MSC_VER > 1700	// from VS2013	
		std::array<T, 2>(src), Size2D<T>() {}
#else				// up to VS2012
		std::array<T, 2>(src), width(this->at(0)), height(this->at(1)) {}
#endif

	////////////////////////////////////////////////////////////////////////////////////////
	// Size3D<T>

	////////////////////////////////////////////////////////////////////////////////////////
	// Default constructors.
	template <typename T>
	Size3D<T>::Size3D(void) : width(this->at(0)), height(this->at(1)), depth(this->at(2)) {}

	template <typename T>
	Size3D<T>::Size3D(const Size3D<T> &src) :
#if _MSC_VER > 1700	// from VS2013	
		std::array<T, 3>(src), Size3D<T>() {}
#else				// up to VS2012
		std::array<T, 3>(src), width(this->at(0)), height(this->at(1)), depth(this->at(2)) {}
#endif

	template <typename T>
	Size3D<T> &Size3D<T>::operator=(const Size3D<T> &src)
	{
		this->std::array<T, 3>::operator=(src);
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Custom constructors.
	template <typename T>
	Size3D<T>::Size3D(T width, T height, T depth) :
#if _MSC_VER > 1700	// from VS2013
		Size3D<T>()
#else				// up to VS2012
		width(this->at(0)), height(this->at(1)), depth(this->at(2))
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
	Region<T, U> &Region<T, U>::operator=(const Region<T, U> &src)
	{
		this->origin = src.origin;
		this->size = src.size;
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
#if _MSC_VER > 1700	// from VS2013
		Region<T, U>(Point2D<T>(x, y), Size2D<U>(width, height)) {}	// for VS2013
#else				// up to VS2012
		origin(Point2D<T>(x, y)), size(Size2D<U>(width, height)) {}	// for VS2012
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
		Add(this->origin, dist, this->origin);
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
