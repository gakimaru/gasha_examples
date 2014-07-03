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
	printf("[ Test for performance of operation : %s ] (* %d times repeat)\n", caption, TEST_OPERATION_REPEAT_NUM);

	std::mt19937 rnd_engine(0);
	std::uniform_real_distribution<TYPE> rnd_distribution(static_cast<TYPE>(0.5), static_cast<TYPE>(1.));

	auto begin = std::chrono::system_clock::now();

	TYPE total = 0.f;
	for (int div = 1; div < TEST_OPERATION_REPEAT_NUM; ++div)
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
template<typename TYPE>
void testForOperationPerformanceDirect(const char* caption)
{
	printf("--------------------------------------------------------------------------------\n");
	printf("[ Test for performance of operation : %s ] (* %d times repeat)\n", caption, TEST_OPERATION_REPEAT_NUM);

	std::mt19937 rnd_engine(0);
	std::uniform_real_distribution<TYPE> rnd_distribution(static_cast<TYPE>(0.5), static_cast<TYPE>(1.));

	auto begin = std::chrono::system_clock::now();

	TYPE total = 0.f;
	for (int div = 1; div < TEST_OPERATION_REPEAT_NUM; ++div)
	{
		const TYPE val1 = rnd_distribution(rnd_engine);
		const TYPE val2 = rnd_distribution(rnd_engine);
		const TYPE val3 = rnd_distribution(rnd_engine);
		const TYPE val4 = rnd_distribution(rnd_engine);
		const TYPE val5 = rnd_distribution(rnd_engine);
		const TYPE result = val1 + val2 - val3 * val4 / val5;
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
	printf("[ Test for performance of sqrt : %s ] (* %d times repeat)\n", caption, TEST_SQRT_REPEAT_NUM);

	std::mt19937 rnd_engine(0);
	std::uniform_real_distribution<TYPE> rnd_distribution(0.5f, 1.f);

	auto begin = std::chrono::system_clock::now();

	TYPE total = 0.f;
	for (int div = 1; div < TEST_SQRT_REPEAT_NUM; ++div)
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
template<typename TYPE>
void testForSqrtPerformanceDirect(const char* caption)
{
	printf("--------------------------------------------------------------------------------\n");
	printf("[ Test for performance of sqrt : %s ] (* %d times repeat)\n", caption, TEST_SQRT_REPEAT_NUM);

	std::mt19937 rnd_engine(0);
	std::uniform_real_distribution<TYPE> rnd_distribution(0.5f, 1.f);

	auto begin = std::chrono::system_clock::now();

	TYPE total = 0.f;
	for (int div = 1; div < TEST_SQRT_REPEAT_NUM; ++div)
	{
		const TYPE value = rnd_distribution(rnd_engine);
		const TYPE result = sqrt(value);
		total += result;
	}
	auto end = std::chrono::system_clock::now();
	auto duration = end - begin;
	double elapsed_time = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count()) / 1000000000;
	printf("total = %.9lf : time=%.9lf sec\n", static_cast<double>(total), elapsed_time);
}

//----------------------------------------
//高速ベクトル演算のパフォーマンス計測
template<class FAST_ARITH, typename TYPE, std::size_t N>
void testForVectorPerformance(const char* caption)
{
	typedef FAST_ARITH fast_arith_class;

	printf("--------------------------------------------------------------------------------\n");
	printf("[ Test for performance of vector : %s ] (* %d times repeat)\n", caption, TEST_VECTOR_REPEAT_NUM);

	std::mt19937 rnd_engine(0);
	std::uniform_real_distribution<TYPE> rnd_distribution(0.5f, 1.f);

	auto begin = std::chrono::system_clock::now();

	TYPE total = 0.f;
	for (int div = 1; div < TEST_VECTOR_REPEAT_NUM; ++div)
	{
		const TYPE vec1[N] = { rnd_distribution(rnd_engine), rnd_distribution(rnd_engine), rnd_distribution(rnd_engine) };
		const TYPE vec2[N] = { rnd_distribution(rnd_engine), rnd_distribution(rnd_engine), rnd_distribution(rnd_engine) };
		const TYPE scalar = rnd_distribution(rnd_engine);
		const TYPE result1 = norm(fast_arith_class(vec1));
		const TYPE result2 = normSq(fast_arith_class(vec1));
		const auto result3 = merge(fast_arith_class(vec1), fast_arith_class(vec2));
		const auto result4 = difference(fast_arith_class(vec1), fast_arith_class(vec2));
		const TYPE result5 = length(fast_arith_class(vec1), fast_arith_class(vec2));
		const TYPE result6 = lengthSq(fast_arith_class(vec1), fast_arith_class(vec2));
		const auto result7 = normalize(fast_arith_class(vec1));
		const auto result8 = mul(fast_arith_class(vec1), scalar);
		const TYPE result9 = dot(fast_arith_class(vec1), fast_arith_class(vec2));
		const TYPE result10 = normalizedDot(fast_arith_class(vec1), fast_arith_class(vec2));
		const auto result11 = cross(fast_arith_class(vec1), fast_arith_class(vec2));
		total += (
			result1 +
			result2 +
			result3[0] + result3[1] + result3[2] +
			result4[0] + result4[1] + result4[2] +
			result5 +
			result6 +
			result7[0] + result7[1] + result7[2] +
			result8[0] + result8[1] + result8[2] +
			result9 +
			result10 +
			result11[0] + result11[1] + result11[2]
			);
	}
	auto end = std::chrono::system_clock::now();
	auto duration = end - begin;
	double elapsed_time = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count()) / 1000000000;
	printf("total = %.9lf : time=%.9lf sec\n", static_cast<double>(total), elapsed_time);
}
template<typename TYPE, std::size_t N>
void testForVectorPerformanceDirect(const char* caption)
{
	printf("--------------------------------------------------------------------------------\n");
	printf("[ Test for performance of vector : %s ] (* %d times repeat)\n", caption, TEST_VECTOR_REPEAT_NUM);

	std::mt19937 rnd_engine(0);
	std::uniform_real_distribution<TYPE> rnd_distribution(0.5f, 1.f);

	auto begin = std::chrono::system_clock::now();

	TYPE total = 0.f;
	for (int div = 1; div < TEST_VECTOR_REPEAT_NUM; ++div)
	{
		const TYPE vec1[N] = { rnd_distribution(rnd_engine), rnd_distribution(rnd_engine), rnd_distribution(rnd_engine) };
		const TYPE vec2[N] = { rnd_distribution(rnd_engine), rnd_distribution(rnd_engine), rnd_distribution(rnd_engine) };
		const TYPE scalar = rnd_distribution(rnd_engine);
		const TYPE result1 = norm(vec1);
		const TYPE result2 = normSq(vec1);
		TYPE result3[N] = { 0 }; merge(result3, vec1, vec2);
		TYPE result4[N] = { 0 }; difference(result4, vec1, vec2);
		const TYPE result5 = length(vec1, vec2);
		const TYPE result6 = lengthSq(vec1, vec2);
		TYPE result7[N] = { 0 }; normalize(result7, vec1);
		TYPE result8[N] = { 0 }; mul(result8, vec1, scalar);
		const TYPE result9 = dot(vec1, vec2);
		const TYPE result10 = normalizedDot(vec1, vec2);
		TYPE result11[N] = { 0 }; cross(result11, vec1, vec2);
		total += (
			result1 +
			result2 +
			result3[0] + result3[1] + result3[2] +
			result4[0] + result4[1] + result4[2] +
			result5 +
			result6 +
			result7[0] + result7[1] + result7[2] +
			result8[0] + result8[1] + result8[2] +
			result9 +
			result10 +
			result11[0] + result11[1] + result11[2]
			);
	}
	auto end = std::chrono::system_clock::now();
	auto duration = end - begin;
	double elapsed_time = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count()) / 1000000000;
	printf("total = %.9lf : time=%.9lf sec\n", static_cast<double>(total), elapsed_time);
}
template<class FAST_ARITH, typename TYPE, std::size_t N>
void testForVectorPerformanceDummy(const char* caption)
{
	typedef FAST_ARITH fast_arith_class;

	printf("--------------------------------------------------------------------------------\n");
	printf("[ Test for performance of vector : %s ] (* %d times repeat)\n", caption, TEST_VECTOR_REPEAT_NUM);

	std::mt19937 rnd_engine(0);
	std::uniform_real_distribution<TYPE> rnd_distribution(0.5f, 1.f);

	auto begin = std::chrono::system_clock::now();

	TYPE total = 0.f;
	for (int div = 1; div < TEST_VECTOR_REPEAT_NUM; ++div)
	{
		const TYPE vec1[N] = { rnd_distribution(rnd_engine), rnd_distribution(rnd_engine), rnd_distribution(rnd_engine) };
		const TYPE vec2[N] = { rnd_distribution(rnd_engine), rnd_distribution(rnd_engine), rnd_distribution(rnd_engine) };
		const TYPE scalar = rnd_distribution(rnd_engine);
		const TYPE result1 = norm(fast_arith_class(vec1));
		const TYPE result2 = normSq(fast_arith_class(vec1));
		TYPE result3[N] = { 0 }; merge(result3, fast_arith_class(vec1), fast_arith_class(vec2));
		TYPE result4[N] = { 0 }; difference(result4, fast_arith_class(vec1), fast_arith_class(vec2));
		const TYPE result5 = length(fast_arith_class(vec1), fast_arith_class(vec2));
		const TYPE result6 = lengthSq(fast_arith_class(vec1), fast_arith_class(vec2));
		TYPE result7[N] = { 0 }; normalize(result7, fast_arith_class(vec1));
		TYPE result8[N] = { 0 }; mul(result8, fast_arith_class(vec1), scalar);
		const TYPE result9 = dot(fast_arith_class(vec1), fast_arith_class(vec2));
		const TYPE result10 = normalizedDot(fast_arith_class(vec1), fast_arith_class(vec2));
		TYPE result11[N] = { 0 }; cross(result11, fast_arith_class(vec1), fast_arith_class(vec2));
		total += (
			result1 +
			result2 +
			result3[0] + result3[1] + result3[2] +
			result4[0] + result4[1] + result4[2] +
			result5 +
			result6 +
			result7[0] + result7[1] + result7[2] +
			result8[0] + result8[1] + result8[2] +
			result9 +
			result10 +
			result11[0] + result11[1] + result11[2]
			);
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

	//高速ベクトル演算のテスト
	printf("--------------------------------------------------------------------------------\n");
	printf("[ Test for vector operation ]\n");

	{
		#define TEST_VECTOR_OPE1(expr, space)  { double result = static_cast<double>(expr); printf(#expr space " = %.5lf\n", result); }
		#define TEST_VECTOR_OPE2(expr, space, result)  { expr; printf(#expr space " = [%.5lf][%.5f]\n", static_cast<double>(result[0]), static_cast<double>(result[1])); }
		#define TEST_VECTOR_OPE3(expr, space, result)  { expr; printf(#expr space " = [%.5lf][%.5f][%.5f]\n", static_cast<double>(result[0]), static_cast<double>(result[1]), static_cast<double>(result[2])); }
		printf("- 2D vector -\n");
		float vec2_1[2] = { 1.f, 2.f };
		float vec2_2[2] = { 3.f, 4.f };
		float vec2_result[2];
		printf("vec2_1=[%.5f][%.5f]\n", vec2_1[0], vec2_1[1]);
		printf("vec2_2=[%.5f][%.5f]\n", vec2_2[0], vec2_2[1]);
		TEST_VECTOR_OPE1(norm(vec2_1), "                            ");
		TEST_VECTOR_OPE1(normSq(vec2_1), "                          ");
		TEST_VECTOR_OPE2(merge(vec2_result, vec2_1, vec2_2), "      ", vec2_result);
		TEST_VECTOR_OPE2(difference(vec2_result, vec2_1, vec2_2), " ", vec2_result);
		TEST_VECTOR_OPE1(length(vec2_1, vec2_2), "                  ");
		TEST_VECTOR_OPE1(lengthSq(vec2_1, vec2_2), "                ");
		TEST_VECTOR_OPE2(normalize(vec2_result, vec2_1), "          ", vec2_result);
		TEST_VECTOR_OPE2(mul(vec2_result, vec2_1, 10.f), "          ", vec2_result);
		TEST_VECTOR_OPE1(dot(vec2_1, vec2_2), "                     ");
		TEST_VECTOR_OPE1(normalizedDot(vec2_1, vec2_2), "           ");
		
		TEST_VECTOR_OPE1(norm(fastArith_2f(vec2_1)), "                                                        ");
		TEST_VECTOR_OPE1(normSq(fastArith_2f(vec2_1)), "                                                      ");
		TEST_VECTOR_OPE2(fastArith_2f vec2_result = merge(fastArith_2f(vec2_1), fastArith_2f(vec2_2)), "      ", vec2_result);
		TEST_VECTOR_OPE2(fastArith_2f vec2_result = difference(fastArith_2f(vec2_1), fastArith_2f(vec2_2)), " ", vec2_result);
		TEST_VECTOR_OPE1(length(fastArith_2f(vec2_1), fastArith_2f(vec2_2)), "                                ");
		TEST_VECTOR_OPE1(lengthSq(fastArith_2f(vec2_1), fastArith_2f(vec2_2)), "                              ");
		TEST_VECTOR_OPE2(fastArith_2f vec2_result = normalize(fastArith_2f(vec2_1)), "                        ", vec2_result);
		TEST_VECTOR_OPE2(fastArith_2f vec2_result = mul(fastArith_2f(vec2_1), 10.f), "                        ", vec2_result);
		TEST_VECTOR_OPE1(dot(fastArith_2f(vec2_1), fastArith_2f(vec2_2)), "                                   ");
		TEST_VECTOR_OPE1(normalizedDot(fastArith_2f(vec2_1), fastArith_2f(vec2_2)), "                         ");

		TEST_VECTOR_OPE1(norm(fastestArith_2f(vec2_1)), "                                                              ");
		TEST_VECTOR_OPE1(normSq(fastestArith_2f(vec2_1)), "                                                            ");
		TEST_VECTOR_OPE2(fastestArith_2f vec2_result = merge(fastestArith_2f(vec2_1), fastestArith_2f(vec2_2)), "      ", vec2_result);
		TEST_VECTOR_OPE2(fastestArith_2f vec2_result = difference(fastestArith_2f(vec2_1), fastestArith_2f(vec2_2)), " ", vec2_result);
		TEST_VECTOR_OPE1(length(fastestArith_2f(vec2_1), fastestArith_2f(vec2_2)), "                                   ");
		TEST_VECTOR_OPE1(lengthSq(fastestArith_2f(vec2_1), fastestArith_2f(vec2_2)), "                                 ");
		TEST_VECTOR_OPE2(fastestArith_2f vec2_result = normalize(fastestArith_2f(vec2_1)), "                           ", vec2_result);
		TEST_VECTOR_OPE2(fastestArith_2f vec2_result = mul(fastestArith_2f(vec2_1), 10.f), "                           ", vec2_result);
		TEST_VECTOR_OPE1(dot(fastestArith_2f(vec2_1), fastestArith_2f(vec2_2)), "                                      ");
		TEST_VECTOR_OPE1(normalizedDot(fastestArith_2f(vec2_1), fastestArith_2f(vec2_2)), "                            ");

		TEST_VECTOR_OPE1(norm(semifastArith_2f(vec2_1)), "                                                                ");
		TEST_VECTOR_OPE1(normSq(semifastArith_2f(vec2_1)), "                                                              ");
		TEST_VECTOR_OPE2(semifastArith_2f vec2_result = merge(semifastArith_2f(vec2_1), semifastArith_2f(vec2_2)), "      ", vec2_result);
		TEST_VECTOR_OPE2(semifastArith_2f vec2_result = difference(semifastArith_2f(vec2_1), semifastArith_2f(vec2_2)), " ", vec2_result);
		TEST_VECTOR_OPE1(length(semifastArith_2f(vec2_1), semifastArith_2f(vec2_2)), "                                    ");
		TEST_VECTOR_OPE1(lengthSq(semifastArith_2f(vec2_1), semifastArith_2f(vec2_2)), "                                  ");
		TEST_VECTOR_OPE2(semifastArith_2f vec2_result = normalize(semifastArith_2f(vec2_1)), "                            ", vec2_result);
		TEST_VECTOR_OPE2(semifastArith_2f vec2_result = mul(semifastArith_2f(vec2_1), 10.f), "                            ", vec2_result);
		TEST_VECTOR_OPE1(dot(semifastArith_2f(vec2_1), semifastArith_2f(vec2_2)), "                                       ");
		TEST_VECTOR_OPE1(normalizedDot(semifastArith_2f(vec2_1), semifastArith_2f(vec2_2)), "                             ");

		TEST_VECTOR_OPE1(norm(sseArith_2f(vec2_1)), "                                                      ");
		TEST_VECTOR_OPE1(normSq(sseArith_2f(vec2_1)), "                                                    ");
		TEST_VECTOR_OPE2(sseArith_2f vec2_result = merge(sseArith_2f(vec2_1), sseArith_2f(vec2_2)), "      ", vec2_result);
		TEST_VECTOR_OPE2(sseArith_2f vec2_result = difference(sseArith_2f(vec2_1), sseArith_2f(vec2_2)), " ", vec2_result);
		TEST_VECTOR_OPE1(length(sseArith_2f(vec2_1), sseArith_2f(vec2_2)), "                               ");
		TEST_VECTOR_OPE1(lengthSq(sseArith_2f(vec2_1), sseArith_2f(vec2_2)), "                             ");
		TEST_VECTOR_OPE2(sseArith_2f vec2_result = normalize(sseArith_2f(vec2_1)), "                       ", vec2_result);
		TEST_VECTOR_OPE2(sseArith_2f vec2_result = mul(sseArith_2f(vec2_1), 10.f), "                       ", vec2_result);
		TEST_VECTOR_OPE1(dot(sseArith_2f(vec2_1), sseArith_2f(vec2_2)), "                                  ");
		TEST_VECTOR_OPE1(normalizedDot(sseArith_2f(vec2_1), sseArith_2f(vec2_2)), "                        ");

		TEST_VECTOR_OPE1(norm(normalArith_2f(vec2_1)), "                                                            ");
		TEST_VECTOR_OPE1(normSq(normalArith_2f(vec2_1)), "                                                          ");
		TEST_VECTOR_OPE2(normalArith_2f vec2_result = merge(normalArith_2f(vec2_1), normalArith_2f(vec2_2)), "      ", vec2_result);
		TEST_VECTOR_OPE2(normalArith_2f vec2_result = difference(normalArith_2f(vec2_1), normalArith_2f(vec2_2)), " ", vec2_result);
		TEST_VECTOR_OPE1(length(normalArith_2f(vec2_1), normalArith_2f(vec2_2)), "                                  ");
		TEST_VECTOR_OPE1(lengthSq(normalArith_2f(vec2_1), normalArith_2f(vec2_2)), "                                ");
		TEST_VECTOR_OPE2(normalArith_2f vec2_result = normalize(normalArith_2f(vec2_1)), "                          ", vec2_result);
		TEST_VECTOR_OPE2(normalArith_2f vec2_result = mul(normalArith_2f(vec2_1), 10.f), "                          ", vec2_result);
		TEST_VECTOR_OPE1(dot(normalArith_2f(vec2_1), normalArith_2f(vec2_2)), "                                     ");
		TEST_VECTOR_OPE1(normalizedDot(normalArith_2f(vec2_1), normalArith_2f(vec2_2)), "                           ");

		TEST_VECTOR_OPE1(norm(dummyArith_2f(vec2_1)), "                                           ");
		TEST_VECTOR_OPE1(normSq(dummyArith_2f(vec2_1)), "                                         ");
		TEST_VECTOR_OPE2(merge(vec2_result, dummyArith_2f(vec2_1), dummyArith_2f(vec2_2)), "      ", vec2_result);
		TEST_VECTOR_OPE2(difference(vec2_result, dummyArith_2f(vec2_1), dummyArith_2f(vec2_2)), " ", vec2_result);
		TEST_VECTOR_OPE1(length(dummyArith_2f(vec2_1), dummyArith_2f(vec2_2)), "                  ");
		TEST_VECTOR_OPE1(lengthSq(dummyArith_2f(vec2_1), dummyArith_2f(vec2_2)), "                ");
		TEST_VECTOR_OPE2(normalize(vec2_result, dummyArith_2f(vec2_1)), "                         ", vec2_result);
		TEST_VECTOR_OPE2(mul(vec2_result, dummyArith_2f(vec2_1), 10.f), "                         ", vec2_result);
		TEST_VECTOR_OPE1(dot(dummyArith_2f(vec2_1), dummyArith_2f(vec2_2)), "                     ");
		TEST_VECTOR_OPE1(normalizedDot(dummyArith_2f(vec2_1), dummyArith_2f(vec2_2)), "           ");

		printf("- 3D vector -\n");
		float vec3_1[3] = { 1.f, 2.f, 3.f };
		float vec3_2[3] = { 4.f, 5.f, 6.f };
		float vec3_result[3];
		printf("vec3_1=[%.5f][%.5f][%.5f]\n", vec3_1[0], vec3_1[1], vec3_1[2]);
		printf("vec3_2=[%.5f][%.5f][%.5f]\n", vec3_2[0], vec3_2[1], vec3_2[2]);
		TEST_VECTOR_OPE1(norm(vec3_1), "                            ");
		TEST_VECTOR_OPE1(normSq(vec3_1), "                          ");
		TEST_VECTOR_OPE3(merge(vec3_result, vec3_1, vec3_2), "      ", vec3_result);
		TEST_VECTOR_OPE3(difference(vec3_result, vec3_1, vec3_2), " ", vec3_result);
		TEST_VECTOR_OPE1(length(vec3_1, vec3_2), "                  ");
		TEST_VECTOR_OPE1(lengthSq(vec3_1, vec3_2), "                ");
		TEST_VECTOR_OPE3(normalize(vec3_result, vec3_1), "          ", vec3_result);
		TEST_VECTOR_OPE3(mul(vec3_result, vec3_1, 10.f), "          ", vec3_result);
		TEST_VECTOR_OPE1(dot(vec3_1, vec3_2), "                     ");
		TEST_VECTOR_OPE1(normalizedDot(vec3_1, vec3_2), "           ");
		TEST_VECTOR_OPE3(cross(vec3_result, vec3_1, vec3_2), "      ", vec3_result);

		TEST_VECTOR_OPE1(norm(fastArith_3f(vec3_1)), "                                                        ");
		TEST_VECTOR_OPE1(normSq(fastArith_3f(vec3_1)), "                                                      ");
		TEST_VECTOR_OPE3(fastArith_3f vec3_result = merge(fastArith_3f(vec3_1), fastArith_3f(vec3_2)), "      ", vec3_result);
		TEST_VECTOR_OPE3(fastArith_3f vec3_result = difference(fastArith_3f(vec3_1), fastArith_3f(vec3_2)), " ", vec3_result);
		TEST_VECTOR_OPE1(length(fastArith_3f(vec3_1), fastArith_3f(vec3_2)), "                                ");
		TEST_VECTOR_OPE1(lengthSq(fastArith_3f(vec3_1), fastArith_3f(vec3_2)), "                              ");
		TEST_VECTOR_OPE3(fastArith_3f vec3_result = normalize(fastArith_3f(vec3_1)), "                        ", vec3_result);
		TEST_VECTOR_OPE3(fastArith_3f vec3_result = mul(fastArith_3f(vec3_1), 10.f), "                        ", vec3_result);
		TEST_VECTOR_OPE1(dot(fastArith_3f(vec3_1), fastArith_3f(vec3_2)), "                                   ");
		TEST_VECTOR_OPE1(normalizedDot(fastArith_3f(vec3_1), fastArith_3f(vec3_2)), "                         ");
		TEST_VECTOR_OPE3(fastArith_3f vec3_result = cross(fastArith_3f(vec3_1), fastArith_3f(vec3_2)), "      ", vec3_result);

		TEST_VECTOR_OPE1(norm(fastestArith_3f(vec3_1)), "                                                              ");
		TEST_VECTOR_OPE1(normSq(fastestArith_3f(vec3_1)), "                                                            ");
		TEST_VECTOR_OPE3(fastestArith_3f vec3_result = merge(fastestArith_3f(vec3_1), fastestArith_3f(vec3_2)), "      ", vec3_result);
		TEST_VECTOR_OPE3(fastestArith_3f vec3_result = difference(fastestArith_3f(vec3_1), fastestArith_3f(vec3_2)), " ", vec3_result);
		TEST_VECTOR_OPE1(length(fastestArith_3f(vec3_1), fastestArith_3f(vec3_2)), "                                   ");
		TEST_VECTOR_OPE1(lengthSq(fastestArith_3f(vec3_1), fastestArith_3f(vec3_2)), "                                 ");
		TEST_VECTOR_OPE3(fastestArith_3f vec3_result = normalize(fastestArith_3f(vec3_1)), "                           ", vec3_result);
		TEST_VECTOR_OPE3(fastestArith_3f vec3_result = mul(fastestArith_3f(vec3_1), 10.f), "                           ", vec3_result);
		TEST_VECTOR_OPE1(dot(fastestArith_3f(vec3_1), fastestArith_3f(vec3_2)), "                                      ");
		TEST_VECTOR_OPE1(normalizedDot(fastestArith_3f(vec3_1), fastestArith_3f(vec3_2)), "                            ");
		TEST_VECTOR_OPE3(fastestArith_3f vec3_result = cross(fastestArith_3f(vec3_1), fastestArith_3f(vec3_2)), "      ", vec3_result);

		TEST_VECTOR_OPE1(norm(semifastArith_3f(vec3_1)), "                                                                ");
		TEST_VECTOR_OPE1(normSq(semifastArith_3f(vec3_1)), "                                                              ");
		TEST_VECTOR_OPE3(semifastArith_3f vec3_result = merge(semifastArith_3f(vec3_1), semifastArith_3f(vec3_2)), "      ", vec3_result);
		TEST_VECTOR_OPE3(semifastArith_3f vec3_result = difference(semifastArith_3f(vec3_1), semifastArith_3f(vec3_2)), " ", vec3_result);
		TEST_VECTOR_OPE1(length(semifastArith_3f(vec3_1), semifastArith_3f(vec3_2)), "                                    ");
		TEST_VECTOR_OPE1(lengthSq(semifastArith_3f(vec3_1), semifastArith_3f(vec3_2)), "                                  ");
		TEST_VECTOR_OPE3(semifastArith_3f vec3_result = normalize(semifastArith_3f(vec3_1)), "                            ", vec3_result);
		TEST_VECTOR_OPE3(semifastArith_3f vec3_result = mul(semifastArith_3f(vec3_1), 10.f), "                            ", vec3_result);
		TEST_VECTOR_OPE1(dot(semifastArith_3f(vec3_1), semifastArith_3f(vec3_2)), "                                       ");
		TEST_VECTOR_OPE1(normalizedDot(semifastArith_3f(vec3_1), semifastArith_3f(vec3_2)), "                             ");
		TEST_VECTOR_OPE3(semifastArith_3f vec3_result = cross(semifastArith_3f(vec3_1), semifastArith_3f(vec3_2)), "      ", vec3_result);

		TEST_VECTOR_OPE1(norm(sseArith_3f(vec3_1)), "                                                      ");
		TEST_VECTOR_OPE1(normSq(sseArith_3f(vec3_1)), "                                                    ");
		TEST_VECTOR_OPE3(sseArith_3f vec3_result = merge(sseArith_3f(vec3_1), sseArith_3f(vec3_2)), "      ", vec3_result);
		TEST_VECTOR_OPE3(sseArith_3f vec3_result = difference(sseArith_3f(vec3_1), sseArith_3f(vec3_2)), " ", vec3_result);
		TEST_VECTOR_OPE1(length(sseArith_3f(vec3_1), sseArith_3f(vec3_2)), "                               ");
		TEST_VECTOR_OPE1(lengthSq(sseArith_3f(vec3_1), sseArith_3f(vec3_2)), "                             ");
		TEST_VECTOR_OPE3(sseArith_3f vec3_result = normalize(sseArith_3f(vec3_1)), "                       ", vec3_result);
		TEST_VECTOR_OPE3(sseArith_3f vec3_result = mul(sseArith_3f(vec3_1), 10.f), "                       ", vec3_result);
		TEST_VECTOR_OPE1(dot(sseArith_3f(vec3_1), sseArith_3f(vec3_2)), "                                  ");
		TEST_VECTOR_OPE1(normalizedDot(sseArith_3f(vec3_1), sseArith_3f(vec3_2)), "                        ");
		TEST_VECTOR_OPE3(sseArith_3f vec3_result = cross(sseArith_3f(vec3_1), sseArith_3f(vec3_2)), "      ", vec3_result);

		TEST_VECTOR_OPE1(norm(normalArith_3f(vec3_1)), "                                                            ");
		TEST_VECTOR_OPE1(normSq(normalArith_3f(vec3_1)), "                                                          ");
		TEST_VECTOR_OPE3(normalArith_3f vec3_result = merge(normalArith_3f(vec3_1), normalArith_3f(vec3_2)), "      ", vec3_result);
		TEST_VECTOR_OPE3(normalArith_3f vec3_result = difference(normalArith_3f(vec3_1), normalArith_3f(vec3_2)), " ", vec3_result);
		TEST_VECTOR_OPE1(length(normalArith_3f(vec3_1), normalArith_3f(vec3_2)), "                                  ");
		TEST_VECTOR_OPE1(lengthSq(normalArith_3f(vec3_1), normalArith_3f(vec3_2)), "                                ");
		TEST_VECTOR_OPE3(normalArith_3f vec3_result = normalize(normalArith_3f(vec3_1)), "                          ", vec3_result);
		TEST_VECTOR_OPE3(normalArith_3f vec3_result = mul(normalArith_3f(vec3_1), 10.f), "                          ", vec3_result);
		TEST_VECTOR_OPE1(dot(normalArith_3f(vec3_1), normalArith_3f(vec3_2)), "                                     ");
		TEST_VECTOR_OPE1(normalizedDot(normalArith_3f(vec3_1), normalArith_3f(vec3_2)), "                           ");
		TEST_VECTOR_OPE3(normalArith_3f vec3_result = cross(normalArith_3f(vec3_1), normalArith_3f(vec3_2)), "      ", vec3_result);

		TEST_VECTOR_OPE1(norm(dummyArith_3f(vec3_1)), "                                           ");
		TEST_VECTOR_OPE1(normSq(dummyArith_3f(vec3_1)), "                                         ");
		TEST_VECTOR_OPE3(merge(vec3_result, dummyArith_3f(vec3_1), dummyArith_3f(vec3_2)), "      ", vec3_result);
		TEST_VECTOR_OPE3(difference(vec3_result, dummyArith_3f(vec3_1), dummyArith_3f(vec3_2)), " ", vec3_result);
		TEST_VECTOR_OPE1(length(dummyArith_3f(vec3_1), dummyArith_3f(vec3_2)), "                  ");
		TEST_VECTOR_OPE1(lengthSq(dummyArith_3f(vec3_1), dummyArith_3f(vec3_2)), "                ");
		TEST_VECTOR_OPE3(normalize(vec3_result, dummyArith_3f(vec3_1)), "                         ", vec3_result);
		TEST_VECTOR_OPE3(mul(vec3_result, dummyArith_3f(vec3_1), 10.f), "                         ", vec3_result);
		TEST_VECTOR_OPE1(dot(dummyArith_3f(vec3_1), dummyArith_3f(vec3_2)), "                     ");
		TEST_VECTOR_OPE1(normalizedDot(dummyArith_3f(vec3_1), dummyArith_3f(vec3_2)), "           ");
		TEST_VECTOR_OPE3(cross(vec3_result, dummyArith_3f(vec3_1), dummyArith_3f(vec3_2)), "      ", vec3_result);
	}

	//各種高速演算のパフォーマンス計測
	testForOperationPerformanceDirect<float>("direct:float");
	testForOperationPerformance<dummyArith_f, float>("dummy:float");
	testForOperationPerformance<normalArith_f, float>("normal:float");
	testForOperationPerformance<sseArith_f, float>("sse:float");
	testForOperationPerformance<fastArith_f, float>("fast:float");
	testForOperationPerformance<semifastArith_f, float>("semifast:float");
	testForOperationPerformance<fastestArith_f, float>("fastest:float");

	testForOperationPerformanceDirect<double>("direct:dobule");
	testForOperationPerformance<dummyArith_d, double>("dummy:dobule");
	testForOperationPerformance<normalArith_d, double>("normal:double");
	testForOperationPerformance<sseArith_d, double>("sse:double");
	testForOperationPerformance<fastArith_d, double>("fast:double");
	testForOperationPerformance<semifastArith_d, double>("semifast:double");
	testForOperationPerformance<fastestArith_d, double>("fastest:double");

	//各種高速平方根のパフォーマンス計測
	testForSqrtPerformanceDirect<float>("direct:float");
	testForSqrtPerformance<dummyArith_f, float>("dummy:float");
	testForSqrtPerformance<normalArith_f, float>("normal:float");
	testForSqrtPerformance<sseArith_f, float>("sse:float");
	testForSqrtPerformance<fastArith_f, float>("fast:float");
	testForSqrtPerformance<semifastArith_f, float>("semifast:float");
	testForSqrtPerformance<fastestArith_f, float>("fastest:float");

	testForSqrtPerformanceDirect<double>("direct:dobule");
	testForSqrtPerformance<dummyArith_d, double>("dummy:dobule");
	testForSqrtPerformance<normalArith_d, double>("normal:double");
	testForSqrtPerformance<sseArith_d, double>("sse:double");
	testForSqrtPerformance<fastArith_d, double>("fast:double");
	testForSqrtPerformance<semifastArith_d, double>("semifast:double");
	testForSqrtPerformance<fastestArith_d, double>("fastest:double");

	//各種高速ベクトル演算のパフォーマンス計測
	testForVectorPerformanceDirect<float, 3>("direct:float[3]");
	testForVectorPerformanceDummy<dummyArith_3f, float, 3>("dummy:float[3]");
	testForVectorPerformance<normalArith_3f, float, 3>("normal:float[3]");
	testForVectorPerformance<sseArith_3f, float, 3>("sse:float[3]");
	testForVectorPerformance<fastArith_3f, float, 3>("fast:float[3]");
	testForVectorPerformance<semifastArith_3f, float, 3>("semifast:float[3]");
	testForVectorPerformance<fastestArith_3f, float, 3>("fastest:float[3]");
}

// End of file
