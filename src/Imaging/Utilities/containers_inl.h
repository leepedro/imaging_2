#if !defined(CONTAINERS_INL_H)
#define CONTAINERS_INL_H
////////////////////////////////////////////////////////////////////////////////////////
// Global functions and operators for std::array<T, N>, std::vector<T> classes

namespace Imaging
{
	////////////////////////////////////////////////////////////////////////////////////////
	// Global functions and operators for std::array<T, N> class.

	// Enables this function for only signed integral data types to use safe negating function.
	template <typename T, ::size_t N>
	typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value, void>::type
		Negate(const std::array<T, N> &a, std::array<T, N> &b)
	{
		auto it_b = b.begin();
		for (auto it_a = a.cbegin(), it_a_end = a.cend(); it_a != it_a_end; ++it_a, ++it_b)
			*it_b = SafeNegate(*it_a);
	}

	/** Enables this function for only floating point data types assuming it does not need
	safe negating function. */
	template <typename T, ::size_t N>
	typename std::enable_if<std::is_floating_point<T>::value, void>::type
		Negate(const std::array<T, N> &a, std::array<T, N> &b)
	{
		auto it_b = b.begin();
		for (auto it_a = a.cbegin(), it_a_end = a.cend(); it_a != it_a_end; ++it_a, ++it_b)
			*it_b = -*it_a;
	}

	/** This function is enabled for both signed integral types and floating point types, but
	not enabled for other data types. */
	template <typename T, ::size_t N>
	std::array<T, N> operator-(const std::array<T, N> &a)
	{
		std::array<T, N> b;
		Negate(a, b);
		return b;
	}

	/** If any operation produces an integer overflow, SafeAdd() function will throw an
	exception. */
	template <typename T, ::size_t N>
	void Add(const std::array<T, N> &a, const std::array<T, N> &b, std::array<T, N> &c)
	{
		auto it_c = c.begin(), it_c_end = c.end();
		for (auto it_a = a.cbegin(), it_b = b.cbegin();
			it_c != it_c_end; ++it_a, ++it_b, ++it_c)
			*it_c = SafeAdd(*it_a, *it_b);
	}

	template <typename T, ::size_t N>
	std::array<T, N> operator+(const std::array<T, N> &a, const std::array<T, N> &b)
	{
		std::array<T, N> c;
		Add(a, b, c);
		return c;
	}

	template <typename T, ::size_t N>
	void Add(std::array<T, N> &a, const std::array<T, N> &b)
	{
		auto it_b = b.cbegin();
		for (auto it_a = a.begin(), it_a_end = a.end(); it_a != it_a_end; ++it_a, it_b)
			*it_a = SafeAdd(*it_a, *it_b);
	}

	template <typename T, ::size_t N>
	std::array<T, N> &operator+=(std::array<T, N> &a, const std::array<T, N> &b)
	{
		Add(a, b);
		return a;
	}

	template <typename T, ::size_t N>
	void Add(const std::array<T, N> &a, const T &b, std::array<T, N> &c)
	{
		auto it_c = c.begin(), it_c_end = c.end();
		for (auto it_a = a.cbegin(); it_c != it_c_end; ++it_a, ++it_c)
			*it_c = SafeAdd(*it_a, b);
	}

	template <typename T, ::size_t N>
	std::array<T, N> operator+(const std::array<T, N> &a, const T &b)
	{
		std::array<T, N> c;
		Add(a, b, c);
		return c;
	}

	template <typename T, ::size_t N>
	void Add(std::array<T, N> &a, const T &b)
	{
		for (auto &elem: a)
			elem = SafeAdd(elem, b);
	}

	template <typename T, ::size_t N>
	std::array<T, N> &operator+=(std::array<T, N> &a, const T &b)
	{
		Add(a, b);
		return a;
	}

	template <typename T, typename U, ::size_t N>
	typename std::enable_if<std::is_floating_point<T>::value, void>::type
		RoundAs(const std::array<T, N> &src, std::array<U, N> &dst)
	{
		auto it_dst = dst.begin(), it_dst_end = dst.end();
		for (auto it_src = src.cbegin(); it_dst != it_dst_end; ++it_src, ++it_dst)
			*it_dst = RoundAs<U>(*it_src);
	}

	template <typename T, ::size_t N>
	void Multiply(const std::array<T, N> &a, double b, std::array<double, N> &c)
	{
		auto it_c = c.begin(), it_c_end = c.end();
		for (auto it_a = a.cbegin(); it_c != it_c_end; ++it_a, ++it_c)
			*it_c = *it_a * b;
	}

	template <typename T, ::size_t N>
	std::array<double, N> operator*(const std::array<T, N> &a, double b)
	{
		std::array<double, N> c;
		Multiply(a, b, c);
		return c;
	}

	template <typename T, ::size_t N>
	void Multiply(const std::array<T, N> &a, const std::array<double, N> &b,
		std::array<double, N> &c)
	{
		auto it_b = b.cbegin();
		auto it_c = c.begin(), it_c_end = c.end();
		for (auto it_a = a.cbegin(); it_c != it_c_end; ++it_a, ++it_b, ++it_c)
			*it_c = *it_a * *it_b;
	}

	template <typename T, ::size_t N>
	std::array<double, N> operator*(const std::array<T, N> &a, const std::array<double, N> &b)
	{
		std::array<double, N> c;
		Multiply(a, b, c);
		return c;
	}

	template <::size_t N>
	void Multiply(std::array<double, N> &a, double b)
	{
		for (auto &elem: a)
			elem *= b;
	}

	template <::size_t N>
	std::array<double, N> &operator*=(std::array<double, N> &a, double b)
	{
		Multiply(a, b);
		return a;
	}

	template <::size_t N>
	void Multiply(std::array<double, N> &a, const std::array<double, N> &b)
	{
		auto it_b = b.cbegin();
		for (auto it = a.begin(), it_end = a.end(); it != it_end; ++it, ++it_b)
			*it *= *it_b;
	}

	template <::size_t N>
	std::array<double, N> &operator*=(std::array<double, N> &a,
		const std::array<double, N> &b)
	{
		Multiply(a, b);
		return a;
	}

	template <typename T, ::size_t N>
	void Divide(const std::array<T, N> &a, double b, std::array<double, N> &c)
	{
		Multiply(a, 1.0 / b, c);
	}

	template <typename T, ::size_t N>
	std::array<double, N> operator/(const std::array<T, N> &a, double b)
	{
		std::array<double, N> c;
		Divide(a, b, c);
		return c;
	}

	template <::size_t N>
	void Divide(std::array<double, N> &a, double b)
	{
		Multiply(a, 1.0 / b);
	}

	template <::size_t N>
	std::array<double, N> &operator/=(std::array<double, N> &a, double b)
	{
		Divide(a, b);
		return a;
	}

	template <::size_t N>
	void Divide(std::array<double, N> &a, const std::array<double, N> &b)
	{
		auto it_b = b.cbegin();
		for (auto it = a.begin(), it_end = a.end(); it != it_end; ++it, ++it_b)
			*it /= *it_b;
	}

	template <::size_t N>
	std::array<double, N> &operator/=(std::array<double, N> &a,
		const std::array<double, N> &b)
	{
		Divide(a, b);
		return a;
	}

	template <typename T, ::size_t N>
	double GetNorm(const std::array<T, N> &src, double p)
	{
		double sum(0.0);
		for (auto it = src.cbegin(), it_end = src.cend(); it != it_end; ++it)
			sum += ::pow(*it, p);
		return ::pow(sum, 1.0 / p);
	}

	template <typename T, ::size_t N>
	std::array<double, N> GetNormedVector(const std::array<T, N> &src, double p)
	{
		double norm = GetNorm(src, p);
		return src / norm;
	}

	template <::size_t N>
	void Normalize(std::array<double, N> &src, double p)
	{
		double norm = GetNorm(src, p);
		src /= norm;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Global functions and operators for std::vector<T> class.

	template <typename T>
	void Copy(const T *src, ::size_t length, std::vector<T> &dst)
	{
		// Re-allocate destination.
		if (dst.size() != length)
			dst.resize(length);

		std::copy(src, src + length, dst.begin());
	}
}

#endif