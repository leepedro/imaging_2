#if !defined(COORDINATES_H)
#define COORDINATES_H

#include <array>

#include "utilities.h"

namespace Imaging
{
	/** The classes defined in this file are aliases of std::array<T, N>.
	They have references indicating each element of the array such as x, y, width, height,
	but they should not have any member variables.
	If it does, the member variable will be discarded by the operators or methods defined
	for std::array<T, N>. */
	/* Since std::array<T, N> class does not have move constructor, the inherited classes
	shall not have a move constructor. */

	/** Presents a 2-D Cartesian coordinate as (x, y). */
	template <typename T>
	class Point2D : public std::array<T, 2>
	{
	public:
		////////////////////////////////////////////////////////////////////////////////////
		// Default constructors.
		Point2D(void);
		Point2D(const Point2D<T> &src);
		Point2D<T> &operator=(const Point2D<T> &src);

		////////////////////////////////////////////////////////////////////////////////////
		// Custom constructors.
		Point2D(T x, T y);

		/** Instantiates the object from an std::array<T, N> object.
		
		The purpose of this constructor is to use the operators and methods, which were
		defined for the base class, for this class.

		@Example
		Point2D<int> ptA, ptB, ptC;
		ptA = ptB + ptC;	// where operator+(...) is defined for only std::array<T, N>. */
		Point2D(const std::array<T, 2> &src);
	
		////////////////////////////////////////////////////////////////////////////////////
		// Accessor.
		T &x, &y;
	};

	/** Presents a 3-D Cartesian coordinate as (x, y, z). */
	template <typename T>
	class Point3D : public std::array<T, 3>
	{
	public:
		////////////////////////////////////////////////////////////////////////////////////
		// Default constructors.
		Point3D(void);
		Point3D(const Point3D<T> &src);
		Point3D<T> &operator=(const Point3D<T> &src);

		////////////////////////////////////////////////////////////////////////////////////
		// Custom constructors.
		Point3D(T x, T y, T z);
		//Point3D(const std::array<T, 3> &src);
	
		////////////////////////////////////////////////////////////////////////////////////
		// Accessor.
		T &x, &y, &z;
	};

	/** Presents the size of a 2-D Cartesian space as (width, height).

	@TODO: It makes sense to enable this class for only unsigned integer and floating point
	data types. */
	template <typename T>
	class Size2D : public std::array<T, 2>
	{
	public:
		////////////////////////////////////////////////////////////////////////////////////
		// Default constructors.
		Size2D(void);
		Size2D(const Size2D<T> &src);
		Size2D<T> &operator=(const Size2D<T> &src);

		////////////////////////////////////////////////////////////////////////////////////
		// Custom constructors.
		Size2D(T width, T height);
		Size2D(const std::array<T, 2> &src);

		////////////////////////////////////////////////////////////////////////////////////
		// Accessor.
		T &width, &height;
	};

	/** Presents the size of a 3-D Cartesian space as (width, height, depth).

	@TODO: It makes sense to enable this class for only unsigned integer and floating point
	data types. */
	template <typename T>
	class Size3D : public std::array<T, 3>
	{
	public:
		////////////////////////////////////////////////////////////////////////////////////
		// Default constructors.
		Size3D(void);
		Size3D(const Size3D<T> &src);
		Size3D<T> &operator=(const Size3D<T> &src);

		////////////////////////////////////////////////////////////////////////////////////
		// Custom constructors.
		Size3D(T width, T height, T depth);
		//Size3D(const std::array<T, 3> &src);

		////////////////////////////////////////////////////////////////////////////////////
		// Accessor.
		T &width, &height, &depth;
	};

	/** Represents a region of interest or an area of interest.

	The dimension is defined as the number of pixels as [x, y] ~ (x + width, y + height). */
	template <typename T, typename U>
	class Region
	{
	public:
		////////////////////////////////////////////////////////////////////////////////////
		// Default constructors.
		Region(void);
		Region(const Region<T, U> &src);
		Region &operator=(const Region<T, U> &src);

		////////////////////////////////////////////////////////////////////////////////////
		// Custom constructors.
		Region(const Point2D<T> &origin, const Size2D<U> &size);
		Region(T x, T y, U width, U height);

		////////////////////////////////////////////////////////////////////////////////////
		// Operators.
		bool operator==(const Region<T, U> &rhs) const;
		bool operator!=(const Region<T, U> &rhs) const;

		/** Moves the origin by the given distance, and returns the result as a new
		Region<T, U> without changing this object. */
		Region<T, U> operator+(const Point2D<T> &dist) const;

		/** Zooms the size by the given zoom rate without moving the origin, and returns
		the result as a new Region<T, U> without changing this object. */
		Region<T, U> operator*(const Point2D<double> &zm) const;
		Region<T, U> operator*(double zm) const;

		////////////////////////////////////////////////////////////////////////////////////
		// Methods.

		//bool Fit(U width, U height) const;

		/** Moves the origin by the given distance. */
		void Move(const Point2D<T> &dist);

		/** Zooms the size by the given zoom rate without moving the origin. */
		void Zoom(const Point2D<double> &zm);
		void Zoom(double zm);

		////////////////////////////////////////////////////////////////////////////////////
		// Data.
		Point2D<T> origin;
		Size2D<U> size;
	};
}

#include "coordinates_inl.h"

#endif
