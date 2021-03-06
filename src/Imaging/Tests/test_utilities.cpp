/** This file contains the test functions to test classes and functions defined utilities.h */
//#include "../Utilities/safecast.h"
#include "../Utilities/containers.h"

#include <stdexcept>
#include <iostream>

template <typename T, typename U>
void TestSafeCast(const T src, U &dst)
{	
	try
	{
		dst = Imaging::SafeCast<U>(src);
		std::cout << "Casted " << typeid(T).name() << " " << src << " to " <<
		typeid(U).name() << " " << dst << std::endl;
	}
	catch (const std::overflow_error &)
	{
		std::cout << "Overflow detected while casting " << typeid(T).name() << " " << src <<
		" to " << typeid(U).name() << std::endl;
	}
	catch (...)
	{
		throw;
	}
}

void TestsSafeCast(void)
{
	std::cout << "Test for safe casting started." << std::endl;

	int i_max(std::numeric_limits<int>::max()), i_neg(-1), i_small(1), i_dst;
	unsigned int ui_max(std::numeric_limits<unsigned int>::max()), ui_small(1), ui_dst;
	long long ll_max(std::numeric_limits<long long>::max()), ll_neg(-1), ll_small(1),
		ll_min(std::numeric_limits<long long>::min());
	unsigned long long ull_dst;
	float f_small(1.0), f_neg(-1.0f), f_max(std::numeric_limits<float>::max()), f_dst;
	double d_neg(-1.0), d_small(1.0), d_max(std::numeric_limits<double>::max()), d_dst;
	double d_min(-std::numeric_limits<double>::max());

	// negative integer overflow risk.
	std::cout << std::endl;
	std::cout << "negative integer overflow risk" << std::endl;
	TestSafeCast(f_neg, ui_dst);		// Warning C4244, overflow detected.
	TestSafeCast(d_small, ui_dst);		// Warning C4244
	TestSafeCast(i_small, ui_dst);
	TestSafeCast(i_neg, ui_dst);		// overflow detected.
	TestSafeCast(ll_neg, i_dst);
	TestSafeCast(ll_min, i_dst);		// overflow detected.

	// positive integer overflow risk.
	std::cout << std::endl;
	std::cout << "positive integer overflow risk" << std::endl;
	TestSafeCast(ll_small, i_dst);
	TestSafeCast(ll_max, i_dst);		// overflow detected.
	TestSafeCast(ui_small, i_dst);
	TestSafeCast(ui_max, i_dst);		// overflow detected.

	// both negative and positive
	std::cout << std::endl;
	std::cout << "both negative and positive integer overflow risk" << std::endl;
	TestSafeCast(ll_small, ui_dst);
	TestSafeCast(ll_max, ui_dst);		// overflow detected.
	TestSafeCast(ll_neg, ui_dst);		// overflow detected.
	f_dst = d_small;					// C4244
	TestSafeCast(d_small, f_dst);
	TestSafeCast(d_min, f_dst);			// overflow detected.
	TestSafeCast(d_max, f_dst);			// overflow detected.
	TestSafeCast(f_small, i_dst);
	TestSafeCast(f_max, i_dst);			// overflow detected.

	// no risk
	std::cout << std::endl;
	std::cout << "no risk" << std::endl;
	//TestSafeCast(ui_max, ull_dst);
	ull_dst = ui_max;
	d_dst = f_small;

	std::cout << "Test for safe casting completed." << std::endl;
}

template <typename T>
void TestSafeAdd(T a, T b, T &c)
{
	try
	{
		c = Imaging::SafeAdd(a, b);
		std::cout << typeid(T).name() << " " << a << " + " <<
		typeid(T).name() << " " << b << " = " << typeid(T).name() << " " << c << std::endl;
	}
	catch (const std::overflow_error &)
	{
		std::cout << "Overflow detected while adding " << typeid(T).name() << " " << a <<
		" and " << typeid(T).name() << " " << b << std::endl;
	}
	catch (...)
	{
		throw;
	}
}

void TestSafeArithmetic(void)
{
	std::cout << "Test for safe arithmetic operation started." << std::endl;

	int i_max(std::numeric_limits<int>::max()), i_neg(-1), i_small(1), i_dst;
	int i_min(std::numeric_limits<int>::min());
	unsigned int ui_max(std::numeric_limits<unsigned int>::max()), ui_small(1), ui_dst;
	long long ll_max(std::numeric_limits<long long>::max()), ll_neg(-1), ll_small(1);
	long long ll_min(std::numeric_limits<long long>::min()), ll_dst;

	// negative integer overflow risk.
	std::cout << std::endl;
	std::cout << "negative integer overflow risk" << std::endl;
	TestSafeAdd(i_min, i_neg, i_dst);		// overflow detected.
	TestSafeAdd(i_neg, i_small, i_dst);
	TestSafeAdd(ll_min, ll_neg, ll_dst);	// overflow detected.
	TestSafeAdd(ll_neg, ll_small, ll_dst);

	// positive integer overflow risk.
	std::cout << std::endl;
	std::cout << "positive integer overflow risk" << std::endl;
	TestSafeAdd(i_max, i_small, i_dst);	// overflow detected.
	TestSafeAdd(i_max, i_neg, i_dst);
	TestSafeAdd(ui_max, ui_small, ui_dst);	// overflow detected.
	TestSafeAdd(ll_max, ll_small, ll_dst);	// overflow detected.
	TestSafeAdd(ll_max, ll_neg, ll_dst);

	std::cout << "Test for safe arithmetic operation completed." << std::endl;
}

template <typename T, ::size_t N>
void TestStdArrayAdd(const std::array<T, N> &a, const std::array<T, N> &b,
					 std::array<T, N> &c)
{
	try
	{
		// Following line does not work without "using namespace Imaging".
		//c = a + b;

		// Following line works.
		c = Imaging::operator+(a, b);
		Imaging::Add(a, b, c);
		std::cout << "Added two std::array<" << typeid(T).name() << ">." << std::endl;

		Imaging::Add(c, b);
		{
			using namespace Imaging;
			c += b;
		}
	}
	catch (const std::overflow_error &)
	{
		std::cout << "Overflow detected while adding two std::array<" << typeid(T).name()
			<< ">." << std::endl;
	}
	catch (...)
	{
		throw;
	}
}

void TestStdArray(void)
{
	using namespace Imaging;
	std::cout << "Test for safe arithmetic operation for std::array<T, N> started."
		<< std::endl;

	int i_max = std::numeric_limits<int>::max();
	int i_min = std::numeric_limits<int>::min();
	std::array<int, 3> i1 = {0, 1, 2}, i2 = {i_max, i_max, i_max}, i3, i0;
	std::array<int, 3> i4 = {0, -1, -2}, i5 = {i_min, i_min, i_min}, i6;	
	
	try
	{
		i6 = -i1;	// Good!
		i6 = -i5;	// Overflow!!!
	}
	catch (const std::overflow_error &)
	{
		std::cout << "Overflowed!" << std::endl;
	}
	catch (...)
	{
		std::cout << "Unexpected error" << std::endl;
	}

	TestStdArrayAdd(i1, i4, i0);

	// Adding with negative integer overflow risk.
	std::cout << std::endl;
	std::cout << "negative integer overflow risk" << std::endl;
	TestStdArrayAdd(i4, i5, i6);

	// Adding positive integer overflow risk.
	std::cout << std::endl;
	std::cout << "positive integer overflow risk" << std::endl;
	TestStdArrayAdd(i1, i2, i3);
		
	Add(i4, 2);
	i4 += 2;

	// Round off
	std::array<double, 3> d1 = {0.4, 0.5, 0.6}, d2, d3, d4;
	Add(d1, 1.0, d2);
	Add(d1, 2.0, d3);
	Add(d1, d3, d4);
	RoundAs(d2, i2);
	RoundAs(d3, i3);

	// Multiplication
	Multiply(i1, 2.0, d1);
	d2 = i1 * 2.0;
	//d2 = Imaging::operator*(i1, 2.0);
	if (d1 == d2)
		std::cout << "Multiplying an array with a scalar was successful." << std::endl;
	else
		std::cout << "Multiplying an array with a scalar was NOT successful." << std::endl;
	Multiply(i1, d1, d3);
	d4 = i1 * d1;
	//d4 = Imaging::operator*(i1, d1);
	if (d3 == d4)
		std::cout << "Multiplying an array with another array was successful." << std::endl;
	else
		std::cout << "Multiplying an array with another array was NOT successful." << std::endl;

	Multiply(d1, 2.0);
	d1 *= 2.0;
	Multiply(d1, d2);
	d1 *= d2;

	// Normalization
	d1 = Imaging::GetNormedVector(i1);
	Copy(i1, d2);
	Normalize(d2);
	if (d1 == d2)
		std::cout << "Normalization was successful." << std::endl;
	else
		std::cout << "Normalization was NOT successful." << std::endl;

	std::cout << "Test for safe arithmetic operation for std::array<T, N> has been completed."
		<< std::endl;
}

void TestUtilities(void)
{
	std::cout << std::endl << "Test for Utilities has started." << std::endl;
	TestsSafeCast();
	TestSafeArithmetic();
	TestStdArray();
	std::cout << "Test for Utilities has been completed." << std::endl;
}