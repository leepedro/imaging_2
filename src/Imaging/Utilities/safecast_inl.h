#if !defined(SAFECAST_INL_H)
#define SAFECAST_INL_H

namespace Imaging
{
	////////////////////////////////////////////////////////////////////////////////////////
	// safe casting

	/* <cmath> of C+11 supports std::round(), but VS2012 does not support it yet.
	The 'round-off from zero' algorithm, which is equivalent to the C+11 round(), is
	implemented using std::floor() and std::ceil(). */
	template <typename T, typename U>
	typename std::enable_if<std::is_floating_point<U>::value && std::is_integral<T>::value,
		T>::type RoundAs(U src)
	{
#if defined(WIN32) && _MSC_VER <= 1700	// up to VS2012
		if (src >= 0)
			return SafeCast_<T>(std::floor(src + 0.5));
		else
			return SafeCast_<T>(std::ceil(src - 0.5));
#else	// C++11
		return SafeCast_<T>(std::round(src));
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
