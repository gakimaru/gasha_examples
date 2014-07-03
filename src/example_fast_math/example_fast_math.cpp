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
template<class FAST_ARITH, typename TYPE>
void testForOperationPerformance(const char* caption)
{
	typedef FAST_ARITH fast_arith_class;

	printf("--------------------------------------------------------------------------------\n");
	printf("[ Test for performance of operation : %s ] (* %d times repeat)\n", caption, TEST_REPEAT_NUM);

	std::mt19937 rnd_engine(0);
	std::uniform_real_distribution<TYPE> rnd_distribution(static_cast<TYPE>(0.5), static_cast<TYPE>(1.));

	auto begin = std::chrono::system_clock::now();

	TYPE total = 0.f;
	for (int div = 1; div < TEST_REPEAT_NUM; ++div)
	{
		const TYPE val1 = rnd_distribution(rnd_engine);
		const TYPE val2 = rnd_distribution(rnd_engine);
		const TYPE val3 = rnd_distribution(rnd_engine);
		const TYPE val4 = rnd_distribution(rnd_engine);
		const TYPE val5 = rnd_distribution(rnd_engine);
		const TYPE result = fast_arith_class(val1) + fast_arith_class(val2) - fast_arith_class(val3) * fast_arith_class(val4) / fast_arith_class(val5);
		total += result;
	}
	auto end = std::chrono::system_clock::now();
	auto duration = end - begin;
	double elapsed_time = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count()) / 1000000000;
	printf("total = %.9lf : time=%.9lf sec\n", static_cast<double>(total), elapsed_time);
}

//----------------------------------------
//高速平方根のパフォーマンス計測
template<class FAST_ARITH, typename TYPE>
void testForSqrtPerformance(const char* caption)
{
	typedef FAST_ARITH fast_arith_class;
	
	printf("--------------------------------------------------------------------------------\n");
	printf("[ Test for performance of sqrt : %s ] (* %d times repeat)\n", caption, TEST_REPEAT_NUM);

	std::mt19937 rnd_engine(0);
	std::uniform_real_distribution<TYPE> rnd_distribution(0.5f, 1.f);

	auto begin = std::chrono::system_clock::now();

	TYPE total = 0.f;
	for (int div = 1; div < TEST_REPEAT_NUM; ++div)
	{
		const TYPE value = rnd_distribution(rnd_engine);
		const TYPE result = sqrt(fast_arith_class(value));
		total += result;
	}
	auto end = std::chrono::system_clock::now();
	auto duration = end - begin;
	double elapsed_time = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count()) / 1000000000;
	printf("total = %.9lf : time=%.9lf sec\n", static_cast<double>(total), elapsed_time);
}

//----------------------------------------
//簡易計算テスト
template<class FAST_ARITH, typename TYPE>
void testForCalc(const char* caption)
{
	std::mt19937 rnd_engine(0);
	std::uniform_real_distribution<TYPE> rnd_distribution(0.5f, 1.f);
	const float a = rnd_distribution(rnd_engine);
	const float b = rnd_distribution(rnd_engine);
	const float c = rnd_distribution(rnd_engine);
	const float d = rnd_distribution(rnd_engine);
	const float e = rnd_distribution(rnd_engine);
	const float f = FAST_ARITH(a) + FAST_ARITH(b) - FAST_ARITH(c) * FAST_ARITH(d) / FAST_ARITH(e);
	printf("[%-8.8s] %.5f + %5f - %.5f * %.5f / %.5f = %.5f\n", caption, a, b, c, d, e, f);
}
template<typename TYPE>
void testForCalcDirect(const char* caption)
{
	std::mt19937 rnd_engine(0);
	std::uniform_real_distribution<TYPE> rnd_distribution(0.5f, 1.f);
	const float a = rnd_distribution(rnd_engine);
	const float b = rnd_distribution(rnd_engine);
	const float c = rnd_distribution(rnd_engine);
	const float d = rnd_distribution(rnd_engine);
	const float e = rnd_distribution(rnd_engine);
	const float f = a + b - c * d / e;
	printf("[%-8.8s] %.5f + %5f - %.5f * %.5f / %.5f = %.5f\n", caption, a, b, c, d, e, f);
}

//----------------------------------------
//高速算術テスト
void example_fast_math()
{
	//各種高速演算のテスト
	printf("--------------------------------------------------------------------------------\n");
	printf("[ Test for fast operation (basic) ]\n");

	testForCalcDirect<float>("direct");
	testForCalc<dummyArith_f, float>("dummy");
	testForCalc<normalArith_f, float>("normal");
	testForCalc<sseArith_f, float>("sse");
	testForCalc<fastArith_f, float>("fast");
	testForCalc<semifastArith_f, float>("semifast");
	testForCalc<fastestArith_f, float>("fastest");
	
	//各種高速演算のテスト
	printf("--------------------------------------------------------------------------------\n");
	printf("[ Test for fast operation ]\n");

	#define TEST_OPE(expr, space)  { double result = static_cast<double>(expr); printf(#expr space " = %.3lf (%.9lf)\n", result, result); }
	#define TEST_ADD_SET(lvalue, rvalue) \
		{ \
			const double lvalue_d = static_cast<double>(lvalue); \
			const double rvalue_d = static_cast<double>(rvalue); \
			printf("\n"); \
			TEST_OPE(lvalue + rvalue, "                 "); \
			TEST_OPE(dummyArith_f(lvalue) + rvalue, "   "); \
			TEST_OPE(normalArith_f(lvalue) + rvalue, "  "); \
			TEST_OPE(sseArith_f(lvalue) + rvalue, "     "); \
			TEST_OPE(fastArith_f(lvalue) + rvalue, "    "); \
			TEST_OPE(semifastArith_f(lvalue) + rvalue, ""); \
			TEST_OPE(fastestArith_f(lvalue) + rvalue, " "); \
			TEST_OPE(dummyArith_d(lvalue_d) + rvalue_d, "   "); \
			TEST_OPE(normalArith_d(lvalue_d) + rvalue_d, "  "); \
			TEST_OPE(sseArith_d(lvalue_d) + rvalue_d, "     "); \
			TEST_OPE(fastArith_d(lvalue_d) + rvalue_d, "    "); \
			TEST_OPE(semifastArith_d(lvalue_d) + rvalue_d, ""); \
			TEST_OPE(fastestArith_d(lvalue_d) + rvalue_d, " "); \
	}
	#define TEST_SUB_SET(lvalue, rvalue) \
		{ \
			const double lvalue_d = static_cast<double>(lvalue); \
			const double rvalue_d = static_cast<double>(rvalue); \
			printf("\n"); \
			TEST_OPE(lvalue - rvalue, "                 "); \
			TEST_OPE(dummyArith_f(lvalue) - rvalue, "   "); \
			TEST_OPE(normalArith_f(lvalue) - rvalue, "  "); \
			TEST_OPE(sseArith_f(lvalue) - rvalue, "     "); \
			TEST_OPE(fastArith_f(lvalue) - rvalue, "    "); \
			TEST_OPE(semifastArith_f(lvalue) - rvalue, ""); \
			TEST_OPE(fastestArith_f(lvalue) - rvalue, " "); \
			TEST_OPE(dummyArith_d(lvalue_d) - rvalue_d, "   "); \
			TEST_OPE(normalArith_d(lvalue_d) - rvalue_d, "  "); \
			TEST_OPE(sseArith_d(lvalue_d) - rvalue_d, "     "); \
			TEST_OPE(fastArith_d(lvalue_d) - rvalue_d, "    "); \
			TEST_OPE(semifastArith_d(lvalue_d) - rvalue_d, ""); \
			TEST_OPE(fastestArith_d(lvalue_d) - rvalue_d, " "); \
		}
	#define TEST_MUL_SET(lvalue, rvalue) \
		{ \
			const double lvalue_d = static_cast<double>(lvalue); \
			const double rvalue_d = static_cast<double>(rvalue); \
			printf("\n"); \
			TEST_OPE(lvalue * rvalue, "                 "); \
			TEST_OPE(dummyArith_f(lvalue) * rvalue, "   "); \
			TEST_OPE(normalArith_f(lvalue) * rvalue, "  "); \
			TEST_OPE(sseArith_f(lvalue) * rvalue, "     "); \
			TEST_OPE(fastArith_f(lvalue) * rvalue, "    "); \
			TEST_OPE(semifastArith_f(lvalue) * rvalue, ""); \
			TEST_OPE(fastestArith_f(lvalue) * rvalue, " "); \
			TEST_OPE(dummyArith_d(lvalue_d) * rvalue_d, "   "); \
			TEST_OPE(normalArith_d(lvalue_d) * rvalue_d, "  "); \
			TEST_OPE(sseArith_d(lvalue_d) * rvalue_d, "     "); \
			TEST_OPE(fastArith_d(lvalue_d) * rvalue_d, "    "); \
			TEST_OPE(semifastArith_d(lvalue_d) * rvalue_d, ""); \
			TEST_OPE(fastestArith_d(lvalue_d) * rvalue_d, " "); \
		}
	#define TEST_DIV_SET(lvalue, rvalue) \
		{ \
			const double lvalue_d = static_cast<double>(lvalue); \
			const double rvalue_d = static_cast<double>(rvalue); \
			printf("\n"); \
			TEST_OPE(lvalue / rvalue, "                 "); \
			TEST_OPE(dummyArith_f(lvalue) / rvalue, "   "); \
			TEST_OPE(normalArith_f(lvalue) / rvalue, "  "); \
			TEST_OPE(sseArith_f(lvalue) / rvalue, "     "); \
			TEST_OPE(fastArith_f(lvalue) / rvalue, "    "); \
			TEST_OPE(semifastArith_f(lvalue) / rvalue, ""); \
			TEST_OPE(fastestArith_f(lvalue) / rvalue, " "); \
			TEST_OPE(dummyArith_d(lvalue_d) / rvalue_d, "   "); \
			TEST_OPE(normalArith_d(lvalue_d) / rvalue_d, "  "); \
			TEST_OPE(sseArith_d(lvalue_d) / rvalue_d, "     "); \
			TEST_OPE(fastArith_d(lvalue_d) / rvalue_d, "    "); \
			TEST_OPE(semifastArith_d(lvalue_d) / rvalue_d, ""); \
			TEST_OPE(fastestArith_d(lvalue_d) / rvalue_d, " "); \
		}

	TEST_ADD_SET(10.f, 1.f);
	TEST_ADD_SET(10.f, 2.f);
	TEST_ADD_SET(10.f, 3.f);
	TEST_ADD_SET(10.f, 4.f);
	TEST_ADD_SET(10.f, 5.f);
	TEST_ADD_SET(10.f, 10.f);
	TEST_ADD_SET(10.f, 100.f);
	TEST_ADD_SET(10.f, 1000.f);
	TEST_ADD_SET(10.f, 10000.f);
	TEST_ADD_SET(10.f, 100000.f);
	TEST_ADD_SET(10.f, 1000000.f);

	TEST_SUB_SET(10.f, 1.f);
	TEST_SUB_SET(10.f, 2.f);
	TEST_SUB_SET(10.f, 3.f);
	TEST_SUB_SET(10.f, 4.f);
	TEST_SUB_SET(10.f, 5.f);
	TEST_SUB_SET(10.f, 10.f);
	TEST_SUB_SET(10.f, 100.f);
	TEST_SUB_SET(10.f, 1000.f);
	TEST_SUB_SET(10.f, 10000.f);
	TEST_SUB_SET(10.f, 100000.f);
	TEST_SUB_SET(10.f, 1000000.f);

	TEST_MUL_SET(10.f, 1.f);
	TEST_MUL_SET(10.f, 2.f);
	TEST_MUL_SET(10.f, 3.f);
	TEST_MUL_SET(10.f, 4.f);
	TEST_MUL_SET(10.f, 5.f);
	TEST_MUL_SET(10.f, 10.f);
	TEST_MUL_SET(10.f, 100.f);
	TEST_MUL_SET(10.f, 1000.f);
	TEST_MUL_SET(10.f, 10000.f);
	TEST_MUL_SET(10.f, 100000.f);
	TEST_MUL_SET(10.f, 1000000.f);

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

	#define TEST_SQRT(expr, space) { double result = static_cast<double>(expr); printf(#expr space " = %.3lf (%.9lf)\n", result, result); }
	#define TEST_SQRT_SET(value) \
		{ \
			const double value_d = value; \
			printf("\n"); \
			TEST_SQRT(std::sqrt(value), "             "); \
			TEST_SQRT(sqrt(dummyArith_f(value)), "    "); \
			TEST_SQRT(sqrt(normalArith_f(value)), "   "); \
			TEST_SQRT(sqrt(sseArith_f(value)), "      "); \
			TEST_SQRT(sqrt(fastArith_f(value)), "     "); \
			TEST_SQRT(sqrt(semifastArith_f(value)), " "); \
			TEST_SQRT(sqrt(fastestArith_f(value)), "  "); \
			TEST_SQRT(sqrt(dummyArith_d(value_d)), "    "); \
			TEST_SQRT(sqrt(normalArith_d(value_d)), "   "); \
			TEST_SQRT(sqrt(sseArith_d(value_d)), "      "); \
			TEST_SQRT(sqrt(fastArith_d(value_d)), "     "); \
			TEST_SQRT(sqrt(semifastArith_d(value_d)), " "); \
			TEST_SQRT(sqrt(fastestArith_d(value_d)), "  "); \
		}

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

	//各種高速演算のパフォーマンス計測
	testForOperationPerformance<dummyArith_f, float>("dummy:float");
	testForOperationPerformance<normalArith_f, float>("normal:float");
	testForOperationPerformance<sseArith_f, float>("sse:float");
	testForOperationPerformance<fastArith_f, float>("fast:float");
	testForOperationPerformance<semifastArith_f, float>("semifast:float");
	testForOperationPerformance<fastestArith_f, float>("fastest:float");

	testForOperationPerformance<dummyArith_d, double>("dummy:dobule");
	testForOperationPerformance<normalArith_d, double>("normal:double");
	testForOperationPerformance<sseArith_d, double>("sse:double");
	testForOperationPerformance<fastArith_d, double>("fast:double");
	testForOperationPerformance<semifastArith_d, double>("semifast:double");
	testForOperationPerformance<fastestArith_d, double>("fastest:double");

	//各種高速平方根のパフォーマンス計測
	testForSqrtPerformance<dummyArith_f, float>("dummy:float");
	testForSqrtPerformance<normalArith_f, float>("normal:float");
	testForSqrtPerformance<sseArith_f, float>("sse:float");
	testForSqrtPerformance<fastArith_f, float>("fast:float");
	testForSqrtPerformance<semifastArith_f, float>("semifast:float");
	testForSqrtPerformance<fastestArith_f, float>("fastest:float");

	testForSqrtPerformance<dummyArith_d, double>("dummy:dobule");
	testForSqrtPerformance<normalArith_d, double>("normal:double");
	testForSqrtPerformance<sseArith_d, double>("sse:double");
	testForSqrtPerformance<fastArith_d, double>("fast:double");
	testForSqrtPerformance<semifastArith_d, double>("semifast:double");
	testForSqrtPerformance<fastestArith_d, double>("fastest:double");
}

// End of file
