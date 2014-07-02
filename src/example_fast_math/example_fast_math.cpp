//--------------------------------------------------------------------------------
// exmaple_fast_math.cpp
// 高速算術テスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_fast_math.h"//高速算術テスト

#include <gasha/fast_math.h>//高速算術

#include <cmath>//std::sqrt()
#include <stdio.h>//printf()

//【VC++】例外を無効化した状態で <chrono> <random> をインクルードすると、warning C4530 が発生する
//  warning C4530: C++ 例外処理を使っていますが、アンワインド セマンティクスは有効にはなりません。/EHsc を指定してください。
#pragma warning(disable: 4530)//C4530を抑える

#include <random>//C++11 std::random
#include <chrono>//C++11 std::chrono

GASHA_USING_NAMESPACE;//ネームスペース使用

//----------------------------------------
//高速除算のパフォーマンス計測
template<class DIV>
void testForDivisionPerformance(const char* caption)
{
	typedef DIV div_class;

	printf("--------------------------------------------------------------------------------\n");
	printf("[ Test for performance of division : %s ] (%d times division)\n", caption, TEST_REPEAT_NUM);

	std::mt19937 rnd_engine(0);
	std::uniform_real_distribution<float> rnd_distribution(0.1f, 1.f);

	auto begin = std::chrono::system_clock::now();

	float total = 0.f;
	for (int div = 1; div < TEST_REPEAT_NUM; ++div)
	{
		const float dividend = rnd_distribution(rnd_engine);
		const float divisor = rnd_distribution(rnd_engine);
		total += (dividend / div_class(divisor));
	}
	auto end = std::chrono::system_clock::now();
	auto duration = end - begin;
	double elapsed_time = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count()) / 1000000000;
	printf("total = %.9f : time=%.9lf sec\n", total, elapsed_time);
}

//----------------------------------------
//高速平方根のパフォーマンス計測
template<class FUNC>
void testForSqrtPerformance(const char* caption, FUNC sqrt_func)
{
	printf("--------------------------------------------------------------------------------\n");
	printf("[ Test for performance of sqrt : %s ] (%d times sqrt)\n", caption, TEST_REPEAT_NUM);

	std::mt19937 rnd_engine(0);
	std::uniform_real_distribution<float> rnd_distribution(0.1f, 1.f);

	auto begin = std::chrono::system_clock::now();

	float total = 0.f;
	for (int div = 1; div < TEST_REPEAT_NUM; ++div)
	{
		const float value = rnd_distribution(rnd_engine);
		total += sqrt_func(value);
	}
	auto end = std::chrono::system_clock::now();
	auto duration = end - begin;
	double elapsed_time = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count()) / 1000000000;
	printf("total = %.9f : time=%.9lf sec\n", total, elapsed_time);
}

//----------------------------------------
//高速算術テスト
void example_fast_math()
{
	//各種高速除算のテスト
	printf("--------------------------------------------------------------------------------\n");
	printf("[ Test for fast division ]\n");

	#define TEST_DIV(expr, space) printf(#expr space " = %.3f (%.9f)\n", (expr), (expr))
	#define TEST_DIV_SET(dividend, divisor) \
		printf("\n"); \
		TEST_DIV(dividend / divisor, "                "); \
		TEST_DIV(dividend / normal_div_f(divisor), "  "); \
		TEST_DIV(dividend / sse_div_f(divisor), "     "); \
		TEST_DIV(dividend / fast_div_f(divisor), "    "); \
		TEST_DIV(dividend / semifast_div_f(divisor), ""); \
		TEST_DIV(dividend / fastest_div_f(divisor), " ");

	TEST_DIV_SET(10.f, 1.f);
	TEST_DIV_SET(10.f, 2.f);
	TEST_DIV_SET(10.f, 3.f);
	TEST_DIV_SET(10.f, 4.f);
	TEST_DIV_SET(10.f, 5.f);
	TEST_DIV_SET(10.f, 10.f);
	TEST_DIV_SET(10.f, 100.f);
	TEST_DIV_SET(10.f, 1000.f);
	TEST_DIV_SET(10.f, 10000.f);
	TEST_DIV_SET(10.f, 100000.f);
	TEST_DIV_SET(10.f, 1000000.f);
	
	//各種高速平方根のテスト
	printf("--------------------------------------------------------------------------------\n");
	printf("[ Test for fast sqrt ]\n");

	#define TEST_SQRT(expr, space) printf(#expr space " = %.3f (%.9f)\n", (expr), (expr))
	#define TEST_SQRT_SET(value) \
		printf("\n"); \
		TEST_SQRT(std::sqrt(value), "    "); \
		TEST_SQRT(normal_sqrt(value), "  "); \
		TEST_SQRT(sse_sqrt(value), "     "); \
		TEST_SQRT(fast_sqrt(value), "    "); \
		TEST_SQRT(semifast_sqrt(value), ""); \

	TEST_SQRT_SET(1.f);
	TEST_SQRT_SET(2.f);
	TEST_SQRT_SET(3.f);
	TEST_SQRT_SET(4.f);
	TEST_SQRT_SET(5.f);
	TEST_SQRT_SET(10.f);
	TEST_SQRT_SET(100.f);
	TEST_SQRT_SET(1000.f);
	TEST_SQRT_SET(10000.f);
	TEST_SQRT_SET(100000.f);
	TEST_SQRT_SET(1000000.f);

	//各種高速除算のパフォーマンス計測
	testForDivisionPerformance<normal_div<float>>("normal");
	testForDivisionPerformance<sse_div<float>>("sse");
	testForDivisionPerformance<fast_div<float>>("fast");
	testForDivisionPerformance<semifast_div<float>>("semifast");
	testForDivisionPerformance<fastest_div<float>>("fastest");

	//各種高速平方根のパフォーマンス計測
	testForSqrtPerformance("normal", normal_sqrt<float>);
	testForSqrtPerformance("sse", sse_sqrt<float>);
	testForSqrtPerformance("fast", fast_sqrt<float>);
	testForSqrtPerformance("semifast", semifast_sqrt<float>);
}

// End of file
