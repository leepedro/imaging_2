#if !defined(CONTAINERS_H)
#define CONTAINERS_H
////////////////////////////////////////////////////////////////////////////////////////
// Global functions and operators for std::array<T, N>, std::vector<T> classes.

#include <array>
#include <algorithm>
#include <vector>

#include "safecast.h"

namespace Imaging
{
	////////////////////////////////////////////////////////////////////////////////////////
	// Global functions and operators for std::array<T, N> class.

	////////////////////////////////////////////////////////////////////////////////////////
	/** Safe implicit conversion
	
	Problem: Only the following four widening conversion cases are truly safe during data
	type conversion.
	The rest of converion scenarios are vulnerable for overflow or data loss, so compiler
	throws a warning.
	1) integer -> floating && src < dst
	2) floating -> floating && src <= dst
	3) integer -> integer && {u -> u || s -> s} && src <= dst
	4) integer -> integer && {u -> s} && src < dst
	Generally [integer -> integer && {u -> s || s -> u} && src == dst] is allowed for
	implicit conversion by compilers, but there is a risk for overflow.

	Solution: Enable function or class for only these four cases with type traits.
	If other snecenarios, i.e., narrowing conversion, are really necessary, use
	static_cast<> and be aware of data loss and overflow. */
	////////////////////////////////////////////////////////////////////////////////////////

	/** Copies one array to another with the same or different data type.

	This function template is enabled for only the cases when implicit casting is possible
	without data loss or integer overflow risk.
	It can be used as an implicit casting method as well as a copying method. */
	/** Compiler gives a warning against int -> float, so it is disabled by enforcing
	src < dst in case 1.
	Since only implicitly allowed conversion is enabled, there is no need to use a safe
	casting method. */
	template <typename T, typename U, ::size_t N>
	typename std::enable_if<	
		(std::is_integral<T>::value && std::is_floating_point<U>::value && sizeof(T) < sizeof(U)) ||
		(std::is_floating_point<T>::value && std::is_floating_point<U>::value &&
		sizeof(T) <= sizeof(U)) ||
		(std::is_integral<T>::value && std::is_integral<U>::value &&
		((std::is_unsigned<T>::value && std::is_unsigned<U>::value) ||
		(std::is_signed<T>::value && std::is_signed<U>::value)) &&
		sizeof(T) <= sizeof(U)) ||
		(std::is_integral<T>::value && std::is_integral<U>::value &&
		std::is_unsigned<T>::value && std::is_signed<U>::value && sizeof(T) < sizeof(U)), void>::type
	Copy(const std::array<T, N> &src, std::array<U, N> &dst)
	{
		std::copy(src.cbegin(), src.cend(), dst.begin());
	}

	/** Adds two std::array<T, N> objects of the same data type and length into another
	std::array<U, N> object of the same data type
	
	@exception std::overflow_error	if the result of any element is below or beyond the
	range of the data type */
	template <typename T, ::size_t N>
	void Add(const std::array<T, N> &a, const std::array<T, N> &b, std::array<T, N> &c);

	/** Adds two std::array<T, N> objects of the same data type and length into another
	std::array<T, N> object of the same data type.

	@NOTE This operator can be practically used only within the namespace because we cannot
	use '+' without "using namespace Imaging". (Maybe it is better that way.)
	Another way is 'c = Imaging::operator+(a, b)', but function Add(a, b, c) is better in
	that way. */
	template <typename T, ::size_t N>
	std::array<T, N> operator+(const std::array<T, N> &a, const std::array<T, N> &b);

	/** Adds a scalar value into all elements of an std::array<T, N> object of the same data
	type.
	
	@exception std::overflow_error	if the result of any element is below or beyond the
	range of the data type */
	template <typename T, ::size_t N>
	void Add(const std::array<T, N> &a, const T &b, std::array<T, N> &c);

	template <typename T, ::size_t N>
	std::array<T, N> operator+(const std::array<T, N> &a, const T &b);

	/** Rounds off an std::array<T, N> from floating point data type to a given data type.
		
	This function template implements the 'round-off from zero' algorithm.
	It is also enabled only if source is a floating point data type. */
	template <typename T, typename U, ::size_t N>
	typename std::enable_if<std::is_floating_point<T>::value, void>::type
		RoundAs(const std::array<T, N> &src, std::array<U, N> &dst);

	/** Multiplies an std::array<T, N> with a (double) scalar, and saves the result into an
	std::array<double, N> object. */
	template <typename T, ::size_t N>
	void Multiply(const std::array<T, N> &a, double b, std::array<double, N> &c);

	template <typename T, ::size_t N>
	std::array<double, N> operator*(const std::array<T, N> &a, double b);

	/** Multiplies an std::array<T, N> with an std::array<double, N>, and saves the result into an
	std::array<double, N> object. */
	template <typename T, ::size_t N>
	void Multiply(const std::array<T, N> &a, const std::array<double, N> &b,
		std::array<double, N> &c);

	template <typename T, ::size_t N>
	std::array<double, N> operator*(const std::array<T, N> &a, const std::array<double, N> &b);

	template <typename T, ::size_t N>
	void Divide(const std::array<T, N> &a, double b, std::array<double, N> &c);

	template <typename T, ::size_t N>
	std::array<double, N> operator/(const std::array<T, N> &a, double b);

	/** Get p-norm of an std::<T, N> array.	

	@param [in] src
	@param [in] p	The order of the norm. Default = 2. Euclidean distance is a p-2 norm.
	@return The p-norm of a vector as a scalar.	*/
	template <typename T, ::size_t N>
	double GetNorm(const std::array<T, N> &src, double p = 2.0);

	/** Normalizes an std::array<T, N>, and returns the normalized vector as an
	std::array<double, N> object.

	@param [in] src
	@param [in] p	The order of the norm. Default = 2. Euclidean distance is a 2-norm.
	@return The normalized vector as an std::array<double, N> object. */
	template <typename T, ::size_t N>
	std::array<double, N> Normalize(const std::array<T, N> &src, double p = 2.0);

	////////////////////////////////////////////////////////////////////////////////////////
	// Global functions and operators for std::vector<T> class.

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
}

#include "containers_inl.h"

#endif