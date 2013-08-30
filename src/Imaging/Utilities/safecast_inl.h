#if !defined(SAFECAST_INL_H)
#define SAFECAST_INL_H
////////////////////////////////////////////////////////////////////////////////////////
// Global functions and operators for safe casting and conversion.

namespace Imaging
{
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

	// floating point to integral types.
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

	/* floating point to floating point

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

	/* <cmath> of C++11 provides std::round(), but VS2012 does not support it yet.
	The 'round-off from zero' algorithm, which is equivalent to the C++11 std::round(), is
	implemented using std::floor() and std::ceil(). */
	template <typename T, typename U>
	typename std::enable_if<std::is_floating_point<U>::value && std::is_integral<T>::value,
		T>::type RoundAs(U src)
	{
#if defined(WIN32) && _MSC_VER <= 1700	// up to VS2012
		if (src >= 0)
			return SafeCast<T>(std::floor(src + 0.5));
		else
			return SafeCast<T>(std::ceil(src - 0.5));
#else	// C++11
		return SafeCast<T>(std::round(src));
#endif
	}

	template <typename T>
	typename std::enable_if<std::is_arithmetic<T>::value, T>::type
		SafeAdd(T a, T b)
	{
		if (b > 0 && a > (std::numeric_limits<T>::max() - b))
			throw std::overflow_error("Result value is too high.");
		else if (b < 0 && a < (std::numeric_limits<T>::min() - b))
			throw std::overflow_error("Result value is too low.");
		return a + b;
	}
}

#endif
