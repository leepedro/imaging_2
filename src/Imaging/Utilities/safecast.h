#if !defined(SAFECAST_H)
#define SAFECAST_H
////////////////////////////////////////////////////////////////////////////////////////
// Global functions and operators for safe casting and conversion.

#include <algorithm>

namespace Imaging
{
	////////////////////////////////////////////////////////////////////////////////////////
	/** Detecting integer overflow
	Two kinds of integer overflow risks are possible, negative and positive.

	A) Negative integer overflow risk
	Problem:
	A negative value can be misunderstood as a positive value by misinterpreting the sign 
	bit if
	 1) signed -> unsigned with any conversion
	 2) signed -> signed with narrowing conversion
	Solution:
	Check if source value is smaller than the minimum limit of destination type.

	B) Positive integer overflow risk
	Problem:
	An extremely high value can be rolled over as a negative value if
	 1) any narrowing conversion
	 2) unsigned -> signed with same data width conversion
	Solution:
	Check if the source value is larger than maximum limit of destination type.

	These two kinds of risks create four possible scenarios.
	1) Negative risk only. (A & ~B)
	2) Positive risk only. (~A & B)
	3) Both negative and positive risks. (A & B)
	4) No risk. (~A & ~B) -> No need, so not defined.
	If source and destination data types are identical, it belongs to the no risk case by
	the logic.

	These four scenarios are exclusively defined by enabling return values using type trait 
	classes. This approach minimizes run-time overhead by implementing only the required 
	check routines for given conditions. The conditions of type trait classes are computed
	during compile-time instead of run-time.
	The same benefit could have been achieved from function template specialization, but it
	requires to define functions for each expected data type instead of type group. */
	////////////////////////////////////////////////////////////////////////////////////////
		
	// 1. Negative risk only. (A & ~B)
	template <typename T, typename U>
	typename std::enable_if<(std::is_integral<T>::value && std::is_integral<U>::value) &&
		(((std::is_signed<U>::value && std::is_unsigned<T>::value) ||
		(std::is_signed<U>::value && std::is_signed<T>::value && sizeof(U) > sizeof(T))) &&
		!((sizeof(U) > sizeof(T)) ||
		(std::is_unsigned<U>::value && std::is_signed<T>::value && sizeof(U) == sizeof(T)))),
		T>::type SafeCast(U src)
	{
		if (src < static_cast<U>(std::numeric_limits<T>::min()))
			throw std::overflow_error("Source value is too low.");
		else
			return static_cast<T>(src);
	}

	// 2. Positive risk only. (~A & B)
	template <typename T, typename U>
	typename std::enable_if<(std::is_integral<T>::value && std::is_integral<U>::value) &&
		(!((std::is_signed<U>::value && std::is_unsigned<T>::value) ||
		(std::is_signed<U>::value && std::is_signed<T>::value && sizeof(U) > sizeof(T))) &&
		((sizeof(U) > sizeof(T)) ||
		(std::is_unsigned<U>::value && std::is_signed<T>::value && sizeof(U) == sizeof(T)))),
		T>::type SafeCast(U src)
	{
		if (src > static_cast<U>(std::numeric_limits<T>::max()))
			throw std::overflow_error("Source value is too high.");
		else
			return static_cast<T>(src);
	}

	// 3. Both negative and positive risks. (A & B)
	template <typename T, typename U>
	typename std::enable_if<(std::is_integral<T>::value && std::is_integral<U>::value) &&
		(((std::is_signed<U>::value && std::is_unsigned<T>::value) ||
		(std::is_signed<U>::value && std::is_signed<T>::value && sizeof(U) > sizeof(T))) &&
		((sizeof(U) > sizeof(T)) ||
		(std::is_unsigned<U>::value && std::is_signed<T>::value && sizeof(U) == sizeof(T)))),
		T>::type SafeCast(U src)
	{
		if (src < static_cast<U>(std::numeric_limits<T>::min()))
			throw std::overflow_error("Source value is too low.");
		else if (src > static_cast<U>(std::numeric_limits<T>::max()))
			throw std::overflow_error("Source value is too high.");
		else
			return static_cast<T>(src);
	}

	// 4. No risk. (~A & ~B)
	// Since there is no risk of integer overflow, implicit conversion is sufficient.
	//template <typename T, typename U>
	//typename std::enable_if<(std::is_integral<T>::value && std::is_integral<U>::value) &&
	//	(!((std::is_signed<U>::value && std::is_unsigned<T>::value) ||
	//	(std::is_signed<U>::value && std::is_signed<T>::value && sizeof(U) > sizeof(T))) &&
	//	!((sizeof(U) > sizeof(T)) ||
	//	(std::is_unsigned<U>::value && std::is_signed<T>::value && sizeof(U) == sizeof(T)))),
	//	T>::type SafeCast(U src)
	//{
	//	return src;
	//}

	/** Casting floating point types to integral types with real-time integer overflow
	checking
	
	@exception std::overflow_error	if source value is beyond the range of destination data
	type

	The compiler warning messages for data precision loss are silenced by explicit
	conversion, i.e., static_cast<T>. */
	template <typename T, typename U>
	typename std::enable_if<std::is_floating_point<U>::value && std::is_integral<T>::value,
		T>::type SafeCast(U src)
	{
		if (static_cast<U>(std::numeric_limits<T>::max()) < src)
			throw std::overflow_error("Source value is too high.");
		else if (static_cast<U>(std::numeric_limits<T>::min()) > src)
			throw std::overflow_error("Source value is too low.");
		else
			return static_cast<T>(src);
	}

	/** narrowing conversion from floating point to floating point with integer (?) overflow
	check

	std::numeric_limits<T>::min() for floating point data types return the minimum precision
	value instead of the negative minimum value. */
	template <typename T, typename U>
	typename std::enable_if<std::is_floating_point<U>::value &&
		std::is_floating_point<T>::value && (sizeof(U) > sizeof(T)),
		T>::type SafeCast(U src)
	{
		if (static_cast<U>(std::numeric_limits<T>::max()) < src)
			throw std::overflow_error("Source value is too high.");
		else if (static_cast<U>(-std::numeric_limits<T>::max()) > src)
			throw std::overflow_error("Source value is too low.");
		else
			return static_cast<T>(src);
	}


	/** Safe round off operation from floating point to integral types.

	@exception std::overflow_error	if source value is beyond the range of destination data
	type */
	template <typename T, typename U>
	typename std::enable_if<std::is_floating_point<U>::value && std::is_integral<T>::value,
		T>::type RoundAs(U src)
	{
#if _MSC_VER > 1700	// from C+11
		return SafeCast<T>(std::round(src));
#else				// up to VS2012
		if (src >= 0)
			return SafeCast<T>(std::floor(src + 0.5));
		else
			return SafeCast<T>(std::ceil(src - 0.5));
#endif
	}

	////////////////////////////////////////////////////////////////////////////////////////
	/** Detecting integer overflow from arithmetic (add) operations

	Problem: Data type is implicitly assumed as the widest one between the two sources.
	If the result of an operation exceeds the data range, overflow will happen.

	@Example
	int i_max = std::numeric_limits<int>::max();
	int i_1 = i_max + 1;		// overflowed
	long long l_1 = i_max + 1;	// overflowed because i_max + 1 is assumed as int
	long long l_temp = i_max;
	long long l_1 = l_temp + 1;	// not overflowed

	Solution 1:
	1) data_type(dst) < max_data_type(src_a, src_b)
	max_src_type temp = a + b (with checking)
	dst = temp (with checking)
	2) data_type(dst) == max_data_type(src_a, src_b)
	dst = a + b (with checking)	Q. Which one between a and b is the maximum data type?
	3) data_type(dst) > max_data_type(src_a, src_b)
	dst = dst_type(a) + dst_type(b)
	-> for 2) and 3),
	if dst >= a && dst >= b,
		dst = dst_type(a) + dst_type(b) (with checking)
	-> for 1)
	if ~(dst >= a && dst >= b),
		auto temp = a + b (with checking? how?)
		dst = static_cast<>(temp) (with checking)

	Solution 2: (selected!)
	Define arithmetic functions for only the same data types and check source value in
	real-time.
	Explicitly convert the variables before/after the operation as necessary. */
	////////////////////////////////////////////////////////////////////////////////////////

	/** Adds two values into one while checking integer overflow.

	@exception std::overflow_error	if the result is below or beyond the range of
	destination data type */
	template <typename T>
	typename std::enable_if<std::is_arithmetic<T>::value, T>::type
		SafeAdd(T a, T b);
}

#include "safecast_inl.h"

#endif
