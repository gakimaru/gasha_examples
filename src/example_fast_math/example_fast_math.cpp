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
#include <gasha/chrono.h>//時間処理ユーティリティ：nowTime(), calcElapsedTime()

#include <stdio.h>//printf()

//【VC++】例外を無効化した状態で <random> をインクルードすると、warning C4530 が発生する
//  warning C4530: C++ 例外処理を使っていますが、アンワインド セマンティクスは有効にはなりません。/EHsc を指定してください。
#pragma warning(disable: 4530)//C4530を抑える

#include <random>//C++11 std::random

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

	auto begin = nowTime();

	TYPE total = static_cast<TYPE>(0);
	for (int repeat = 1; repeat < TEST_OPERATION_REPEAT_NUM; ++repeat)
	{
		const TYPE val1 = rnd_distribution(rnd_engine);
		const TYPE val2 = rnd_distribution(rnd_engine);
		const TYPE val3 = rnd_distribution(rnd_engine);
		const TYPE val4 = rnd_distribution(rnd_engine);
		const TYPE val5 = rnd_distribution(rnd_engine);
		const TYPE result = fast_arith_class(val1) + fast_arith_class(val2) - fast_arith_class(val3) * fast_arith_class(val4) / fast_arith_class(val5);
		total += result;
	}
	double elapsed_time = calcElapsedTime(begin);
	printf("total = %.9lf : time=%.9lf sec\n", static_cast<double>(total), elapsed_time);
}
template<typename TYPE>
void testForOperationPerformanceDirect(const char* caption)
{
	printf("--------------------------------------------------------------------------------\n");
	printf("[ Test for performance of operation : %s ] (* %d times repeat)\n", caption, TEST_OPERATION_REPEAT_NUM);

	std::mt19937 rnd_engine(0);
	std::uniform_real_distribution<TYPE> rnd_distribution(static_cast<TYPE>(0.5), static_cast<TYPE>(1.));

	auto begin = nowTime();

	TYPE total = static_cast<TYPE>(0);
	for (int repeat = 1; repeat < TEST_OPERATION_REPEAT_NUM; ++repeat)
	{
		const TYPE val1 = rnd_distribution(rnd_engine);
		const TYPE val2 = rnd_distribution(rnd_engine);
		const TYPE val3 = rnd_distribution(rnd_engine);
		const TYPE val4 = rnd_distribution(rnd_engine);
		const TYPE val5 = rnd_distribution(rnd_engine);
		const TYPE result = val1 + val2 - val3 * val4 / val5;
		total += result;
	}
	double elapsed_time = calcElapsedTime(begin);
	printf("total = %.9lf : time=%.9lf sec\n", static_cast<double>(total), elapsed_time);
}

//----------------------------------------
//高速平方根のパフォーマンス計測
template<class FAST_ARITH, typename TYPE>
void testForSqrtPerformance(const char* caption)
{
	typedef FAST_ARITH fast_arith_class;
	
	printf("--------------------------------------------------------------------------------\n");
	printf("[ Test for performance of sqr : %s ] (* %d times repeat)\n", caption, TEST_SQRT_REPEAT_NUM);

	std::mt19937 rnd_engine(0);
	std::uniform_real_distribution<TYPE> rnd_distribution(0.5f, 1.f);

	auto begin = nowTime();

	TYPE total = static_cast<TYPE>(0);
	for (int repeat = 1; repeat < TEST_SQRT_REPEAT_NUM; ++repeat)
	{
		const TYPE value = rnd_distribution(rnd_engine);
		const TYPE result = sqr(fast_arith_class(value));
		total += result;
	}
	double elapsed_time = calcElapsedTime(begin);
	printf("total = %.9lf : time=%.9lf sec\n", static_cast<double>(total), elapsed_time);
}
template<typename TYPE>
void testForSqrtPerformanceDirect(const char* caption)
{
	printf("--------------------------------------------------------------------------------\n");
	printf("[ Test for performance of sqr : %s ] (* %d times repeat)\n", caption, TEST_SQRT_REPEAT_NUM);

	std::mt19937 rnd_engine(0);
	std::uniform_real_distribution<TYPE> rnd_distribution(0.5f, 1.f);

	auto begin = nowTime();

	TYPE total = static_cast<TYPE>(0);
	for (int repeat = 1; repeat < TEST_SQRT_REPEAT_NUM; ++repeat)
	{
		const TYPE value = rnd_distribution(rnd_engine);
		const TYPE result = sqr(value);
		total += result;
	}
	double elapsed_time = calcElapsedTime(begin);
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

	auto begin = nowTime();

	TYPE total = static_cast<TYPE>(0);
	for (int repeat = 1; repeat < TEST_VECTOR_REPEAT_NUM; ++repeat)
	{
		TYPE vec1[N] = { 0 };
		TYPE vec2[N] = { 0 };
		for (int i = 0; i < N && i < 3; ++i)
		{
			vec1[i] = rnd_distribution(rnd_engine);
			vec2[i] = rnd_distribution(rnd_engine);
		}
		const TYPE scalar = rnd_distribution(rnd_engine);
		const TYPE result1 = norm(fast_arith_class(vec1));
		const TYPE result2 = normSq(fast_arith_class(vec1));
		const auto result3 = merge(fast_arith_class(vec1), fast_arith_class(vec2));
		const auto result4 = difference(fast_arith_class(vec1), fast_arith_class(vec2));
		const TYPE result5 = length(fast_arith_class(vec1), fast_arith_class(vec2));
		const TYPE result6 = lengthSq(fast_arith_class(vec1), fast_arith_class(vec2));
		const auto result7 = normalize(fast_arith_class(vec1));
		const auto result8 = mul(fast_arith_class(vec1), scalar);
		const auto result9 = forward(fast_arith_class(vec1), scalar);
		const TYPE result10 = dot(fast_arith_class(vec1), fast_arith_class(vec2));
		const TYPE result11 = normalizedDot(fast_arith_class(vec1), fast_arith_class(vec2));
		total += (
				result1 +
				result2 +
				result5 +
				result6 +
				result10 +
				result11
			);
		for (int i = 0; i < N; ++i)
			total += (
				result3[i] +
				result4[i] +
				result7[i] +
				result8[i] +
				result9[i]
			);
		if (N == 3 || N == 4)
		{
			const auto result12 = cross(fast_arith_class(vec1), fast_arith_class(vec2));
			for (int i = 0; i < N; ++i)
				total += result12[i];
		}
	}
	double elapsed_time = calcElapsedTime(begin);
	printf("total = %.9lf : time=%.9lf sec\n", static_cast<double>(total), elapsed_time);
}
template<typename TYPE, std::size_t N>
void testForVectorPerformanceDirect(const char* caption)
{
	printf("--------------------------------------------------------------------------------\n");
	printf("[ Test for performance of vector : %s ] (* %d times repeat)\n", caption, TEST_VECTOR_REPEAT_NUM);

	std::mt19937 rnd_engine(0);
	std::uniform_real_distribution<TYPE> rnd_distribution(0.5f, 1.f);

	auto begin = nowTime();

	TYPE total = static_cast<TYPE>(0);
	for (int repeat = 1; repeat < TEST_VECTOR_REPEAT_NUM; ++repeat)
	{
		TYPE vec1[N] = { 0 };
		TYPE vec2[N] = { 0 };
		for (int i = 0; i < N && i < 3; ++i)
		{
			vec1[i] = rnd_distribution(rnd_engine);
			vec2[i] = rnd_distribution(rnd_engine);
		}
		const TYPE scalar = rnd_distribution(rnd_engine);
		const TYPE result1 = norm(vec1);
		const TYPE result2 = normSq(vec1);
		TYPE result3[N] = { 0 }; merge(result3, vec1, vec2);
		TYPE result4[N] = { 0 }; difference(result4, vec1, vec2);
		const TYPE result5 = length(vec1, vec2);
		const TYPE result6 = lengthSq(vec1, vec2);
		TYPE result7[N] = { 0 }; normalize(result7, vec1);
		TYPE result8[N] = { 0 }; mul(result8, vec1, scalar);
		TYPE result9[N] = { 0 }; forward(result9, vec1, scalar);
		const TYPE result10 = dot(vec1, vec2);
		const TYPE result11 = normalizedDot(vec1, vec2);
		total += (
			result1 +
			result2 +
			result5 +
			result6 +
			result10 +
			result11
			);
		for (int i = 0; i < N; ++i)
			total += (
				result3[i] +
				result4[i] +
				result7[i] +
				result8[i] +
				result9[i]
			);
		if (N == 3 || N == 4)
		{
			TYPE result12[N] = { 0 }; cross(result12, vec1, vec2);
			for (int i = 0; i < N; ++i)
				total += result12[i];
		}
	}
	double elapsed_time = calcElapsedTime(begin);
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

	auto begin = nowTime();

	TYPE total = static_cast<TYPE>(0);
	for (int repeat = 1; repeat < TEST_VECTOR_REPEAT_NUM; ++repeat)
	{
		TYPE vec1[N] = { 0 };
		TYPE vec2[N] = { 0 };
		for (int i = 0; i < N && i < 3; ++i)
		{
			vec1[i] = rnd_distribution(rnd_engine);
			vec2[i] = rnd_distribution(rnd_engine);
		}
		const TYPE scalar = rnd_distribution(rnd_engine);
		const TYPE result1 = norm(fast_arith_class(vec1));
		const TYPE result2 = normSq(fast_arith_class(vec1));
		TYPE result3[N] = { 0 }; merge(result3, fast_arith_class(vec1), fast_arith_class(vec2));
		TYPE result4[N] = { 0 }; difference(result4, fast_arith_class(vec1), fast_arith_class(vec2));
		const TYPE result5 = length(fast_arith_class(vec1), fast_arith_class(vec2));
		const TYPE result6 = lengthSq(fast_arith_class(vec1), fast_arith_class(vec2));
		TYPE result7[N] = { 0 }; normalize(result7, fast_arith_class(vec1));
		TYPE result8[N] = { 0 }; mul(result8, fast_arith_class(vec1), scalar);
		TYPE result9[N] = { 0 }; forward(result9, fast_arith_class(vec1), scalar);
		const TYPE result10 = dot(fast_arith_class(vec1), fast_arith_class(vec2));
		const TYPE result11 = normalizedDot(fast_arith_class(vec1), fast_arith_class(vec2));
		total += (
				result1 +
				result2 +
				result5 +
				result6 +
				result10 +
				result11
			);
		for (int i = 0; i < N; ++i)
			total += (
				result3[i] +
				result4[i] +
				result7[i] +
				result8[i] +
				result9[i]
			);
		if (N == 3 || N == 4)
		{
			TYPE result12[N] = { 0 }; cross(result12, fast_arith_class(vec1), fast_arith_class(vec2));
			for (int i = 0; i < N; ++i)
				total += result12[i];
		}
	}
	double elapsed_time = calcElapsedTime(begin);
	printf("total = %.9lf : time=%.9lf sec\n", static_cast<double>(total), elapsed_time);
}

//----------------------------------------
//高速行列演算のパフォーマンス計測
template<class FAST_ARITH, typename TYPE, std::size_t N, std::size_t M>
void testForMatrixPerformance(const char* caption)
{
	typedef FAST_ARITH fast_arith_class;

	printf("--------------------------------------------------------------------------------\n");
	printf("[ Test for performance of matrix : %s ] (* %d times repeat)\n", caption, TEST_MATRIX_REPEAT_NUM);

	std::mt19937 rnd_engine(0);
	std::uniform_real_distribution<TYPE> rnd_distribution(0.5f, 1.f);

	auto begin = nowTime();

	TYPE total = static_cast<TYPE>(0);
	for (int repeat = 1; repeat < TEST_MATRIX_REPEAT_NUM; ++repeat)
	{
		TYPE mat1[N][M] = { 0 };
		TYPE mat2[N][M] = { 0 };
		for (int i = 0; i < N; ++i)
		{
			for (int j = 0; j < M; ++j)
			{
				mat1[i][j] = rnd_distribution(rnd_engine);
				mat2[i][j] = rnd_distribution(rnd_engine);
			}
		}
		const TYPE scalar = rnd_distribution(rnd_engine);
		const auto result1 = add(fast_arith_class(mat1), fast_arith_class(mat2));
		const auto result2 = sub(fast_arith_class(mat1), fast_arith_class(mat2));
		const auto result3 = mul(fast_arith_class(mat1), scalar);
		const auto result4 = mul(fast_arith_class(mat1), fast_arith_class(mat2));
		for (int i = 0; i < N; ++i)
		{
			for (int j = 0; j < M; ++j)
			{
				total += (
					result1[i][j] +
					result2[i][j] +
					result3[i][j] +
					result4[i][j]
					);
			}
		}
	}
	double elapsed_time = calcElapsedTime(begin);
	printf("total = %.9lf : time=%.9lf sec\n", static_cast<double>(total), elapsed_time);
}
template<typename TYPE, std::size_t N, std::size_t M>
void testForMatrixPerformanceDirect(const char* caption)
{
	printf("--------------------------------------------------------------------------------\n");
	printf("[ Test for performance of matrix : %s ] (* %d times repeat)\n", caption, TEST_MATRIX_REPEAT_NUM);

	std::mt19937 rnd_engine(0);
	std::uniform_real_distribution<TYPE> rnd_distribution(0.5f, 1.f);

	auto begin = nowTime();

	TYPE total = static_cast<TYPE>(0);
	for (int repeat = 1; repeat < TEST_MATRIX_REPEAT_NUM; ++repeat)
	{
		TYPE mat1[N][M] = { 0 };
		TYPE mat2[N][M] = { 0 };
		for (int i = 0; i < N; ++i)
		{
			for (int j = 0; j < M; ++j)
			{
				mat1[i][j] = rnd_distribution(rnd_engine);
				mat2[i][j] = rnd_distribution(rnd_engine);
			}
		}
		TYPE scalar = rnd_distribution(rnd_engine);
		TYPE result1[N][M] = { 0 };
		TYPE result2[N][M] = { 0 };
		TYPE result3[N][M] = { 0 };
		TYPE result4[N][M] = { 0 };
		add(result1, mat1, mat2);
		sub(result2, mat1, mat2);
		mul(result3, mat1, scalar);
		mul(result4, mat1, mat2);
		for (int i = 0; i < N; ++i)
		{
			for (int j = 0; j < M; ++j)
			{
				total += (
						result1[i][j] +
						result2[i][j] +
						result3[i][j] +
						result4[i][j]
					);
			}
		}
	}
	double elapsed_time = calcElapsedTime(begin);
	printf("total = %.9lf : time=%.9lf sec\n", static_cast<double>(total), elapsed_time);
}
template<typename TYPE, std::size_t N, std::size_t M>
void testForMatrixPerformanceDirectLU(const char* caption)
{
	printf("--------------------------------------------------------------------------------\n");
	printf("[ Test for performance of matrix : %s ] (* %d times repeat)\n", caption, TEST_MATRIX_REPEAT_NUM);

	std::mt19937 rnd_engine(0);
	std::uniform_real_distribution<TYPE> rnd_distribution(0.5f, 1.f);

	auto begin = nowTime();

	TYPE total = static_cast<TYPE>(0);
	for (int repeat = 1; repeat < TEST_MATRIX_REPEAT_NUM; ++repeat)
	{
		TYPE mat1[N][M] = { 0 };
		TYPE mat2[N][M] = { 0 };
		for (int i = 0; i < N; ++i)
		{
			for (int j = 0; j < M; ++j)
			{
				mat1[i][j] = rnd_distribution(rnd_engine);
				mat2[i][j] = rnd_distribution(rnd_engine);
			}
		}
		TYPE scalar = rnd_distribution(rnd_engine);
		TYPE result1[N][M] = { 0 };
		TYPE result2[N][M] = { 0 };
		TYPE result3[N][M] = { 0 };
		TYPE result4[N][M] = { 0 };
		addLU(result1, mat1, mat2);
		subLU(result2, mat1, mat2);
		mulLU(result3, mat1, scalar);
		mulLU(result4, mat1, mat2);
		for (int i = 0; i < N; ++i)
		{
			for (int j = 0; j < M; ++j)
			{
				total += (
					result1[i][j] +
					result2[i][j] +
					result3[i][j] +
					result4[i][j]
					);
			}
		}
	}
	double elapsed_time = calcElapsedTime(begin);
	printf("total = %.9lf : time=%.9lf sec\n", static_cast<double>(total), elapsed_time);
}
template<class FAST_ARITH, typename TYPE, std::size_t N, std::size_t M>
void testForMatrixPerformanceDummy(const char* caption)
{
	typedef FAST_ARITH fast_arith_class;

	printf("--------------------------------------------------------------------------------\n");
	printf("[ Test for performance of matrix : %s ] (* %d times repeat)\n", caption, TEST_MATRIX_REPEAT_NUM);

	std::mt19937 rnd_engine(0);
	std::uniform_real_distribution<TYPE> rnd_distribution(0.5f, 1.f);

	auto begin = nowTime();

	TYPE total = static_cast<TYPE>(0);
	for (int repeat = 1; repeat < TEST_MATRIX_REPEAT_NUM; ++repeat)
	{
		TYPE mat1[N][M] = { 0 };
		TYPE mat2[N][M] = { 0 };
		for (int i = 0; i < N; ++i)
		{
			for (int j = 0; j < M; ++j)
			{
				mat1[i][j] = rnd_distribution(rnd_engine);
				mat2[i][j] = rnd_distribution(rnd_engine);
			}
		}
		TYPE scalar = rnd_distribution(rnd_engine);
		TYPE result1[N][M] = { 0 };
		TYPE result2[N][M] = { 0 };
		TYPE result3[N][M] = { 0 };
		TYPE result4[N][M] = { 0 };
		add(result1, fast_arith_class(mat1), fast_arith_class(mat2));
		sub(result2, fast_arith_class(mat1), fast_arith_class(mat2));
		mul(result3, fast_arith_class(mat1), scalar);
		mul(result4, fast_arith_class(mat1), fast_arith_class(mat2));
		for (int i = 0; i < N; ++i)
		{
			for (int j = 0; j < M; ++j)
			{
				total += (
					result1[i][j] +
					result2[i][j] +
					result3[i][j] +
					result4[i][j]
					);
			}
		}
	}
	double elapsed_time = calcElapsedTime(begin);
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
	testForCalc<dummyA_f, float>("dummy");
	testForCalc<normA_f, float>("normal");
	testForCalc<sseA_f, float>("sse");
	testForCalc<fastA_f, float>("fast");
	testForCalc<semiA_f, float>("semifast");
	testForCalc<fastestA_f, float>("fastest");
	
	//各種高速演算のテスト
	printf("\n");
	printf("--------------------------------------------------------------------------------\n");
	printf("[ Test for fast operation ]\n");

	#define TEST_OPE(expr, space)  { double result = static_cast<double>(expr); printf(#expr space " = %.3lf (%.9lf)\n", result, result); }
	#define TEST_ADD_SET(lvalue, rvalue) \
		{ \
			const double lvalue_d = static_cast<double>(lvalue); \
			const double rvalue_d = static_cast<double>(rvalue); \
			printf("\n"); \
			TEST_OPE(lvalue + rvalue, "                 "); \
			TEST_OPE(dummyA_f(lvalue) + rvalue, "   "); \
			TEST_OPE(normA_f(lvalue) + rvalue, "  "); \
			TEST_OPE(sseA_f(lvalue) + rvalue, "     "); \
			TEST_OPE(fastA_f(lvalue) + rvalue, "    "); \
			TEST_OPE(semiA_f(lvalue) + rvalue, ""); \
			TEST_OPE(fastestA_f(lvalue) + rvalue, " "); \
			TEST_OPE(dummyA_d(lvalue_d) + rvalue_d, "   "); \
			TEST_OPE(normA_d(lvalue_d) + rvalue_d, "  "); \
			TEST_OPE(sseA_d(lvalue_d) + rvalue_d, "     "); \
			TEST_OPE(fastA_d(lvalue_d) + rvalue_d, "    "); \
			TEST_OPE(semiA_d(lvalue_d) + rvalue_d, ""); \
			TEST_OPE(fastestA_d(lvalue_d) + rvalue_d, " "); \
	}
	#define TEST_SUB_SET(lvalue, rvalue) \
		{ \
			const double lvalue_d = static_cast<double>(lvalue); \
			const double rvalue_d = static_cast<double>(rvalue); \
			printf("\n"); \
			TEST_OPE(lvalue - rvalue, "                 "); \
			TEST_OPE(dummyA_f(lvalue) - rvalue, "   "); \
			TEST_OPE(normA_f(lvalue) - rvalue, "  "); \
			TEST_OPE(sseA_f(lvalue) - rvalue, "     "); \
			TEST_OPE(fastA_f(lvalue) - rvalue, "    "); \
			TEST_OPE(semiA_f(lvalue) - rvalue, ""); \
			TEST_OPE(fastestA_f(lvalue) - rvalue, " "); \
			TEST_OPE(dummyA_d(lvalue_d) - rvalue_d, "   "); \
			TEST_OPE(normA_d(lvalue_d) - rvalue_d, "  "); \
			TEST_OPE(sseA_d(lvalue_d) - rvalue_d, "     "); \
			TEST_OPE(fastA_d(lvalue_d) - rvalue_d, "    "); \
			TEST_OPE(semiA_d(lvalue_d) - rvalue_d, ""); \
			TEST_OPE(fastestA_d(lvalue_d) - rvalue_d, " "); \
		}
	#define TEST_MUL_SET(lvalue, rvalue) \
		{ \
			const double lvalue_d = static_cast<double>(lvalue); \
			const double rvalue_d = static_cast<double>(rvalue); \
			printf("\n"); \
			TEST_OPE(lvalue * rvalue, "                 "); \
			TEST_OPE(dummyA_f(lvalue) * rvalue, "   "); \
			TEST_OPE(normA_f(lvalue) * rvalue, "  "); \
			TEST_OPE(sseA_f(lvalue) * rvalue, "     "); \
			TEST_OPE(fastA_f(lvalue) * rvalue, "    "); \
			TEST_OPE(semiA_f(lvalue) * rvalue, ""); \
			TEST_OPE(fastestA_f(lvalue) * rvalue, " "); \
			TEST_OPE(dummyA_d(lvalue_d) * rvalue_d, "   "); \
			TEST_OPE(normA_d(lvalue_d) * rvalue_d, "  "); \
			TEST_OPE(sseA_d(lvalue_d) * rvalue_d, "     "); \
			TEST_OPE(fastA_d(lvalue_d) * rvalue_d, "    "); \
			TEST_OPE(semiA_d(lvalue_d) * rvalue_d, ""); \
			TEST_OPE(fastestA_d(lvalue_d) * rvalue_d, " "); \
		}
	#define TEST_DIV_SET(lvalue, rvalue) \
		{ \
			const double lvalue_d = static_cast<double>(lvalue); \
			const double rvalue_d = static_cast<double>(rvalue); \
			printf("\n"); \
			TEST_OPE(lvalue / rvalue, "                 "); \
			TEST_OPE(dummyA_f(lvalue) / rvalue, "   "); \
			TEST_OPE(normA_f(lvalue) / rvalue, "  "); \
			TEST_OPE(sseA_f(lvalue) / rvalue, "     "); \
			TEST_OPE(fastA_f(lvalue) / rvalue, "    "); \
			TEST_OPE(semiA_f(lvalue) / rvalue, ""); \
			TEST_OPE(fastestA_f(lvalue) / rvalue, " "); \
			TEST_OPE(dummyA_d(lvalue_d) / rvalue_d, "   "); \
			TEST_OPE(normA_d(lvalue_d) / rvalue_d, "  "); \
			TEST_OPE(sseA_d(lvalue_d) / rvalue_d, "     "); \
			TEST_OPE(fastA_d(lvalue_d) / rvalue_d, "    "); \
			TEST_OPE(semiA_d(lvalue_d) / rvalue_d, ""); \
			TEST_OPE(fastestA_d(lvalue_d) / rvalue_d, " "); \
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
	printf("\n");
	printf("--------------------------------------------------------------------------------\n");
	printf("[ Test for fast sqr ]\n");

	#define TEST_SQRT(expr, space) { double result = static_cast<double>(expr); printf(#expr space " = %.3lf (%.9lf)\n", result, result); }
	#define TEST_SQRT_SET(value) \
		{ \
			const double value_d = value; \
			printf("\n"); \
			TEST_SQRT(std::sqrt(value), "             "); \
			TEST_SQRT(sqr(dummyA_f(value)), "    "); \
			TEST_SQRT(sqr(normA_f(value)), "   "); \
			TEST_SQRT(sqr(sseA_f(value)), "      "); \
			TEST_SQRT(sqr(fastA_f(value)), "     "); \
			TEST_SQRT(sqr(semiA_f(value)), " "); \
			TEST_SQRT(sqr(fastestA_f(value)), "  "); \
			TEST_SQRT(sqr(dummyA_d(value_d)), "    "); \
			TEST_SQRT(sqr(normA_d(value_d)), "   "); \
			TEST_SQRT(sqr(sseA_d(value_d)), "      "); \
			TEST_SQRT(sqr(fastA_d(value_d)), "     "); \
			TEST_SQRT(sqr(semiA_d(value_d)), " "); \
			TEST_SQRT(sqr(fastestA_d(value_d)), "  "); \
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
	printf("\n");
	printf("--------------------------------------------------------------------------------\n");
	printf("[ Test for vector operation ]\n");

	{
		#define TEST_VECTOR_OPE1(expr, space)  { double result = static_cast<double>(expr); printf(#expr space " = %.5lf\n", result); }
		#define TEST_VECTOR_OPE2(expr, space, result)  { expr; printf(#expr space " = [%.5lf][%.5f]\n", static_cast<double>(result[0]), static_cast<double>(result[1])); }
		#define TEST_VECTOR_OPE3(expr, space, result)  { expr; printf(#expr space " = [%.5lf][%.5f][%.5f]\n", static_cast<double>(result[0]), static_cast<double>(result[1]), static_cast<double>(result[2])); }
		printf("\n");
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
		TEST_VECTOR_OPE2(forward(vec2_result, vec2_1, 10.f), "      ", vec2_result);
		TEST_VECTOR_OPE1(dot(vec2_1, vec2_2), "                     ");
		TEST_VECTOR_OPE1(normalizedDot(vec2_1, vec2_2), "           ");
		
		TEST_VECTOR_OPE1(norm(fastA_2f(vec2_1)), "                                                ");
		TEST_VECTOR_OPE1(normSq(fastA_2f(vec2_1)), "                                              ");
		TEST_VECTOR_OPE2(fastA_2f vec2_result = merge(fastA_2f(vec2_1), fastA_2f(vec2_2)), "      ", vec2_result);
		TEST_VECTOR_OPE2(fastA_2f vec2_result = difference(fastA_2f(vec2_1), fastA_2f(vec2_2)), " ", vec2_result);
		TEST_VECTOR_OPE1(length(fastA_2f(vec2_1), fastA_2f(vec2_2)), "                            ");
		TEST_VECTOR_OPE1(lengthSq(fastA_2f(vec2_1), fastA_2f(vec2_2)), "                          ");
		TEST_VECTOR_OPE2(fastA_2f vec2_result = normalize(fastA_2f(vec2_1)), "                    ", vec2_result);
		TEST_VECTOR_OPE2(fastA_2f vec2_result = mul(fastA_2f(vec2_1), 10.f), "                    ", vec2_result);
		TEST_VECTOR_OPE2(fastA_2f vec2_result = forward(fastA_2f(vec2_1), 10.f), "                ", vec2_result);
		TEST_VECTOR_OPE1(dot(fastA_2f(vec2_1), fastA_2f(vec2_2)), "                               ");
		TEST_VECTOR_OPE1(normalizedDot(fastA_2f(vec2_1), fastA_2f(vec2_2)), "                     ");

		TEST_VECTOR_OPE1(norm(fastestA_2f(vec2_1)), "                                                      ");
		TEST_VECTOR_OPE1(normSq(fastestA_2f(vec2_1)), "                                                    ");
		TEST_VECTOR_OPE2(fastestA_2f vec2_result = merge(fastestA_2f(vec2_1), fastestA_2f(vec2_2)), "      ", vec2_result);
		TEST_VECTOR_OPE2(fastestA_2f vec2_result = difference(fastestA_2f(vec2_1), fastestA_2f(vec2_2)), " ", vec2_result);
		TEST_VECTOR_OPE1(length(fastestA_2f(vec2_1), fastestA_2f(vec2_2)), "                               ");
		TEST_VECTOR_OPE1(lengthSq(fastestA_2f(vec2_1), fastestA_2f(vec2_2)), "                             ");
		TEST_VECTOR_OPE2(fastestA_2f vec2_result = normalize(fastestA_2f(vec2_1)), "                       ", vec2_result);
		TEST_VECTOR_OPE2(fastestA_2f vec2_result = mul(fastestA_2f(vec2_1), 10.f), "                       ", vec2_result);
		TEST_VECTOR_OPE2(fastestA_2f vec2_result = forward(fastestA_2f(vec2_1), 10.f), "                   ", vec2_result);
		TEST_VECTOR_OPE1(dot(fastestA_2f(vec2_1), fastestA_2f(vec2_2)), "                                  ");
		TEST_VECTOR_OPE1(normalizedDot(fastestA_2f(vec2_1), fastestA_2f(vec2_2)), "                        ");

		TEST_VECTOR_OPE1(norm(semiA_2f(vec2_1)), "                                                ");
		TEST_VECTOR_OPE1(normSq(semiA_2f(vec2_1)), "                                              ");
		TEST_VECTOR_OPE2(semiA_2f vec2_result = merge(semiA_2f(vec2_1), semiA_2f(vec2_2)), "      ", vec2_result);
		TEST_VECTOR_OPE2(semiA_2f vec2_result = difference(semiA_2f(vec2_1), semiA_2f(vec2_2)), " ", vec2_result);
		TEST_VECTOR_OPE1(length(semiA_2f(vec2_1), semiA_2f(vec2_2)), "                            ");
		TEST_VECTOR_OPE1(lengthSq(semiA_2f(vec2_1), semiA_2f(vec2_2)), "                          ");
		TEST_VECTOR_OPE2(semiA_2f vec2_result = normalize(semiA_2f(vec2_1)), "                    ", vec2_result);
		TEST_VECTOR_OPE2(semiA_2f vec2_result = mul(semiA_2f(vec2_1), 10.f), "                    ", vec2_result);
		TEST_VECTOR_OPE2(semiA_2f vec2_result = forward(semiA_2f(vec2_1), 10.f), "                ", vec2_result);
		TEST_VECTOR_OPE1(dot(semiA_2f(vec2_1), semiA_2f(vec2_2)), "                               ");
		TEST_VECTOR_OPE1(normalizedDot(semiA_2f(vec2_1), semiA_2f(vec2_2)), "                     ");

		TEST_VECTOR_OPE1(norm(sseA_2f(vec2_1)), "                                              ");
		TEST_VECTOR_OPE1(normSq(sseA_2f(vec2_1)), "                                            ");
		TEST_VECTOR_OPE2(sseA_2f vec2_result = merge(sseA_2f(vec2_1), sseA_2f(vec2_2)), "      ", vec2_result);
		TEST_VECTOR_OPE2(sseA_2f vec2_result = difference(sseA_2f(vec2_1), sseA_2f(vec2_2)), " ", vec2_result);
		TEST_VECTOR_OPE1(length(sseA_2f(vec2_1), sseA_2f(vec2_2)), "                           ");
		TEST_VECTOR_OPE1(lengthSq(sseA_2f(vec2_1), sseA_2f(vec2_2)), "                         ");
		TEST_VECTOR_OPE2(sseA_2f vec2_result = normalize(sseA_2f(vec2_1)), "                   ", vec2_result);
		TEST_VECTOR_OPE2(sseA_2f vec2_result = mul(sseA_2f(vec2_1), 10.f), "                   ", vec2_result);
		TEST_VECTOR_OPE2(sseA_2f vec2_result = forward(sseA_2f(vec2_1), 10.f), "               ", vec2_result);
		TEST_VECTOR_OPE1(dot(sseA_2f(vec2_1), sseA_2f(vec2_2)), "                              ");
		TEST_VECTOR_OPE1(normalizedDot(sseA_2f(vec2_1), sseA_2f(vec2_2)), "                    ");

		TEST_VECTOR_OPE1(norm(normA_2f(vec2_1)), "                                                ");
		TEST_VECTOR_OPE1(normSq(normA_2f(vec2_1)), "                                              ");
		TEST_VECTOR_OPE2(normA_2f vec2_result = merge(normA_2f(vec2_1), normA_2f(vec2_2)), "      ", vec2_result);
		TEST_VECTOR_OPE2(normA_2f vec2_result = difference(normA_2f(vec2_1), normA_2f(vec2_2)), " ", vec2_result);
		TEST_VECTOR_OPE1(length(normA_2f(vec2_1), normA_2f(vec2_2)), "                            ");
		TEST_VECTOR_OPE1(lengthSq(normA_2f(vec2_1), normA_2f(vec2_2)), "                          ");
		TEST_VECTOR_OPE2(normA_2f vec2_result = normalize(normA_2f(vec2_1)), "                    ", vec2_result);
		TEST_VECTOR_OPE2(normA_2f vec2_result = mul(normA_2f(vec2_1), 10.f), "                    ", vec2_result);
		TEST_VECTOR_OPE2(normA_2f vec2_result = forward(normA_2f(vec2_1), 10.f), "                ", vec2_result);
		TEST_VECTOR_OPE1(dot(normA_2f(vec2_1), normA_2f(vec2_2)), "                               ");
		TEST_VECTOR_OPE1(normalizedDot(normA_2f(vec2_1), normA_2f(vec2_2)), "                     ");

		TEST_VECTOR_OPE1(norm(dummyA_2f(vec2_1)), "                                       ");
		TEST_VECTOR_OPE1(normSq(dummyA_2f(vec2_1)), "                                     ");
		TEST_VECTOR_OPE2(merge(vec2_result, dummyA_2f(vec2_1), dummyA_2f(vec2_2)), "      ", vec2_result);
		TEST_VECTOR_OPE2(difference(vec2_result, dummyA_2f(vec2_1), dummyA_2f(vec2_2)), " ", vec2_result);
		TEST_VECTOR_OPE1(length(dummyA_2f(vec2_1), dummyA_2f(vec2_2)), "                  ");
		TEST_VECTOR_OPE1(lengthSq(dummyA_2f(vec2_1), dummyA_2f(vec2_2)), "                ");
		TEST_VECTOR_OPE2(normalize(vec2_result, dummyA_2f(vec2_1)), "                     ", vec2_result);
		TEST_VECTOR_OPE2(mul(vec2_result, dummyA_2f(vec2_1), 10.f), "                     ", vec2_result);
		TEST_VECTOR_OPE2(forward(vec2_result, dummyA_2f(vec2_1), 10.f), "                 ", vec2_result);
		TEST_VECTOR_OPE1(dot(dummyA_2f(vec2_1), dummyA_2f(vec2_2)), "                     ");
		TEST_VECTOR_OPE1(normalizedDot(dummyA_2f(vec2_1), dummyA_2f(vec2_2)), "           ");

		printf("\n");
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
		TEST_VECTOR_OPE3(forward(vec3_result, vec3_1, 10.f), "      ", vec3_result);
		TEST_VECTOR_OPE1(dot(vec3_1, vec3_2), "                     ");
		TEST_VECTOR_OPE1(normalizedDot(vec3_1, vec3_2), "           ");
		TEST_VECTOR_OPE3(cross(vec3_result, vec3_1, vec3_2), "      ", vec3_result);

		TEST_VECTOR_OPE1(norm(fastA_3f(vec3_1)), "                                                ");
		TEST_VECTOR_OPE1(normSq(fastA_3f(vec3_1)), "                                              ");
		TEST_VECTOR_OPE3(fastA_3f vec3_result = merge(fastA_3f(vec3_1), fastA_3f(vec3_2)), "      ", vec3_result);
		TEST_VECTOR_OPE3(fastA_3f vec3_result = difference(fastA_3f(vec3_1), fastA_3f(vec3_2)), " ", vec3_result);
		TEST_VECTOR_OPE1(length(fastA_3f(vec3_1), fastA_3f(vec3_2)), "                            ");
		TEST_VECTOR_OPE1(lengthSq(fastA_3f(vec3_1), fastA_3f(vec3_2)), "                          ");
		TEST_VECTOR_OPE3(fastA_3f vec3_result = normalize(fastA_3f(vec3_1)), "                    ", vec3_result);
		TEST_VECTOR_OPE3(fastA_3f vec3_result = mul(fastA_3f(vec3_1), 10.f), "                    ", vec3_result);
		TEST_VECTOR_OPE3(fastA_3f vec3_result = forward(fastA_3f(vec3_1), 10.f), "                ", vec3_result);
		TEST_VECTOR_OPE1(dot(fastA_3f(vec3_1), fastA_3f(vec3_2)), "                               ");
		TEST_VECTOR_OPE1(normalizedDot(fastA_3f(vec3_1), fastA_3f(vec3_2)), "                     ");
		TEST_VECTOR_OPE3(fastA_3f vec3_result = cross(fastA_3f(vec3_1), fastA_3f(vec3_2)), "      ", vec3_result);

		TEST_VECTOR_OPE1(norm(fastestA_3f(vec3_1)), "                                                      ");
		TEST_VECTOR_OPE1(normSq(fastestA_3f(vec3_1)), "                                                    ");
		TEST_VECTOR_OPE3(fastestA_3f vec3_result = merge(fastestA_3f(vec3_1), fastestA_3f(vec3_2)), "      ", vec3_result);
		TEST_VECTOR_OPE3(fastestA_3f vec3_result = difference(fastestA_3f(vec3_1), fastestA_3f(vec3_2)), " ", vec3_result);
		TEST_VECTOR_OPE1(length(fastestA_3f(vec3_1), fastestA_3f(vec3_2)), "                               ");
		TEST_VECTOR_OPE1(lengthSq(fastestA_3f(vec3_1), fastestA_3f(vec3_2)), "                             ");
		TEST_VECTOR_OPE3(fastestA_3f vec3_result = normalize(fastestA_3f(vec3_1)), "                       ", vec3_result);
		TEST_VECTOR_OPE3(fastestA_3f vec3_result = mul(fastestA_3f(vec3_1), 10.f), "                       ", vec3_result);
		TEST_VECTOR_OPE3(fastestA_3f vec3_result = forward(fastestA_3f(vec3_1), 10.f), "                   ", vec3_result);
		TEST_VECTOR_OPE1(dot(fastestA_3f(vec3_1), fastestA_3f(vec3_2)), "                                  ");
		TEST_VECTOR_OPE1(normalizedDot(fastestA_3f(vec3_1), fastestA_3f(vec3_2)), "                        ");
		TEST_VECTOR_OPE3(fastestA_3f vec3_result = cross(fastestA_3f(vec3_1), fastestA_3f(vec3_2)), "      ", vec3_result);

		TEST_VECTOR_OPE1(norm(semiA_3f(vec3_1)), "                                                ");
		TEST_VECTOR_OPE1(normSq(semiA_3f(vec3_1)), "                                              ");
		TEST_VECTOR_OPE3(semiA_3f vec3_result = merge(semiA_3f(vec3_1), semiA_3f(vec3_2)), "      ", vec3_result);
		TEST_VECTOR_OPE3(semiA_3f vec3_result = difference(semiA_3f(vec3_1), semiA_3f(vec3_2)), " ", vec3_result);
		TEST_VECTOR_OPE1(length(semiA_3f(vec3_1), semiA_3f(vec3_2)), "                            ");
		TEST_VECTOR_OPE1(lengthSq(semiA_3f(vec3_1), semiA_3f(vec3_2)), "                          ");
		TEST_VECTOR_OPE3(semiA_3f vec3_result = normalize(semiA_3f(vec3_1)), "                    ", vec3_result);
		TEST_VECTOR_OPE3(semiA_3f vec3_result = mul(semiA_3f(vec3_1), 10.f), "                    ", vec3_result);
		TEST_VECTOR_OPE3(semiA_3f vec3_result = forward(semiA_3f(vec3_1), 10.f), "                ", vec3_result);
		TEST_VECTOR_OPE1(dot(semiA_3f(vec3_1), semiA_3f(vec3_2)), "                               ");
		TEST_VECTOR_OPE1(normalizedDot(semiA_3f(vec3_1), semiA_3f(vec3_2)), "                     ");
		TEST_VECTOR_OPE3(semiA_3f vec3_result = cross(semiA_3f(vec3_1), semiA_3f(vec3_2)), "      ", vec3_result);

		TEST_VECTOR_OPE1(norm(sseA_3f(vec3_1)), "                                              ");
		TEST_VECTOR_OPE1(normSq(sseA_3f(vec3_1)), "                                            ");
		TEST_VECTOR_OPE3(sseA_3f vec3_result = merge(sseA_3f(vec3_1), sseA_3f(vec3_2)), "      ", vec3_result);
		TEST_VECTOR_OPE3(sseA_3f vec3_result = difference(sseA_3f(vec3_1), sseA_3f(vec3_2)), " ", vec3_result);
		TEST_VECTOR_OPE1(length(sseA_3f(vec3_1), sseA_3f(vec3_2)), "                           ");
		TEST_VECTOR_OPE1(lengthSq(sseA_3f(vec3_1), sseA_3f(vec3_2)), "                         ");
		TEST_VECTOR_OPE3(sseA_3f vec3_result = normalize(sseA_3f(vec3_1)), "                   ", vec3_result);
		TEST_VECTOR_OPE3(sseA_3f vec3_result = mul(sseA_3f(vec3_1), 10.f), "                   ", vec3_result);
		TEST_VECTOR_OPE3(sseA_3f vec3_result = forward(sseA_3f(vec3_1), 10.f), "               ", vec3_result);
		TEST_VECTOR_OPE1(dot(sseA_3f(vec3_1), sseA_3f(vec3_2)), "                              ");
		TEST_VECTOR_OPE1(normalizedDot(sseA_3f(vec3_1), sseA_3f(vec3_2)), "                    ");
		TEST_VECTOR_OPE3(sseA_3f vec3_result = cross(sseA_3f(vec3_1), sseA_3f(vec3_2)), "      ", vec3_result);

		TEST_VECTOR_OPE1(norm(normA_3f(vec3_1)), "                                                ");
		TEST_VECTOR_OPE1(normSq(normA_3f(vec3_1)), "                                              ");
		TEST_VECTOR_OPE3(normA_3f vec3_result = merge(normA_3f(vec3_1), normA_3f(vec3_2)), "      ", vec3_result);
		TEST_VECTOR_OPE3(normA_3f vec3_result = difference(normA_3f(vec3_1), normA_3f(vec3_2)), " ", vec3_result);
		TEST_VECTOR_OPE1(length(normA_3f(vec3_1), normA_3f(vec3_2)), "                            ");
		TEST_VECTOR_OPE1(lengthSq(normA_3f(vec3_1), normA_3f(vec3_2)), "                          ");
		TEST_VECTOR_OPE3(normA_3f vec3_result = normalize(normA_3f(vec3_1)), "                    ", vec3_result);
		TEST_VECTOR_OPE3(normA_3f vec3_result = mul(normA_3f(vec3_1), 10.f), "                    ", vec3_result);
		TEST_VECTOR_OPE3(normA_3f vec3_result = forward(normA_3f(vec3_1), 10.f), "                ", vec3_result);
		TEST_VECTOR_OPE1(dot(normA_3f(vec3_1), normA_3f(vec3_2)), "                               ");
		TEST_VECTOR_OPE1(normalizedDot(normA_3f(vec3_1), normA_3f(vec3_2)), "                     ");
		TEST_VECTOR_OPE3(normA_3f vec3_result = cross(normA_3f(vec3_1), normA_3f(vec3_2)), "      ", vec3_result);

		TEST_VECTOR_OPE1(norm(dummyA_3f(vec3_1)), "                                       ");
		TEST_VECTOR_OPE1(normSq(dummyA_3f(vec3_1)), "                                     ");
		TEST_VECTOR_OPE3(merge(vec3_result, dummyA_3f(vec3_1), dummyA_3f(vec3_2)), "      ", vec3_result);
		TEST_VECTOR_OPE3(difference(vec3_result, dummyA_3f(vec3_1), dummyA_3f(vec3_2)), " ", vec3_result);
		TEST_VECTOR_OPE1(length(dummyA_3f(vec3_1), dummyA_3f(vec3_2)), "                  ");
		TEST_VECTOR_OPE1(lengthSq(dummyA_3f(vec3_1), dummyA_3f(vec3_2)), "                ");
		TEST_VECTOR_OPE3(normalize(vec3_result, dummyA_3f(vec3_1)), "                     ", vec3_result);
		TEST_VECTOR_OPE3(mul(vec3_result, dummyA_3f(vec3_1), 10.f), "                     ", vec3_result);
		TEST_VECTOR_OPE3(forward(vec3_result, dummyA_3f(vec3_1), 10.f), "                 ", vec3_result);
		TEST_VECTOR_OPE1(dot(dummyA_3f(vec3_1), dummyA_3f(vec3_2)), "                     ");
		TEST_VECTOR_OPE1(normalizedDot(dummyA_3f(vec3_1), dummyA_3f(vec3_2)), "           ");
		TEST_VECTOR_OPE3(cross(vec3_result, dummyA_3f(vec3_1), dummyA_3f(vec3_2)), "      ", vec3_result);

		printf("\n");
		printf("- 4D vector -\n");
		float vec4_1[4] = { 1.f, 2.f, 3.f, 0.f };
		float vec4_2[4] = { 4.f, 5.f, 6.f, 0.f };
		float vec4_result[4];
		printf("vec4_1=[%.5f][%.5f][%.5f]\n", vec4_1[0], vec4_1[1], vec4_1[2]);
		printf("vec4_2=[%.5f][%.5f][%.5f]\n", vec4_2[0], vec4_2[1], vec4_2[2]);
		TEST_VECTOR_OPE1(norm(vec4_1), "                            ");
		TEST_VECTOR_OPE1(normSq(vec4_1), "                          ");
		TEST_VECTOR_OPE3(merge(vec4_result, vec4_1, vec4_2), "      ", vec4_result);
		TEST_VECTOR_OPE3(difference(vec4_result, vec4_1, vec4_2), " ", vec4_result);
		TEST_VECTOR_OPE1(length(vec4_1, vec4_2), "                  ");
		TEST_VECTOR_OPE1(lengthSq(vec4_1, vec4_2), "                ");
		TEST_VECTOR_OPE3(normalize(vec4_result, vec4_1), "          ", vec4_result);
		TEST_VECTOR_OPE3(mul(vec4_result, vec4_1, 10.f), "          ", vec4_result);
		TEST_VECTOR_OPE3(forward(vec4_result, vec4_1, 10.f), "      ", vec4_result);
		TEST_VECTOR_OPE1(dot(vec4_1, vec4_2), "                     ");
		TEST_VECTOR_OPE1(normalizedDot(vec4_1, vec4_2), "           ");
		TEST_VECTOR_OPE3(cross(vec4_result, vec4_1, vec4_2), "      ", vec4_result);

		TEST_VECTOR_OPE1(norm(fastA_4f(vec4_1)), "                                                ");
		TEST_VECTOR_OPE1(normSq(fastA_4f(vec4_1)), "                                              ");
		TEST_VECTOR_OPE3(fastA_4f vec4_result = merge(fastA_4f(vec4_1), fastA_4f(vec4_2)), "      ", vec4_result);
		TEST_VECTOR_OPE3(fastA_4f vec4_result = difference(fastA_4f(vec4_1), fastA_4f(vec4_2)), " ", vec4_result);
		TEST_VECTOR_OPE1(length(fastA_4f(vec4_1), fastA_4f(vec4_2)), "                            ");
		TEST_VECTOR_OPE1(lengthSq(fastA_4f(vec4_1), fastA_4f(vec4_2)), "                          ");
		TEST_VECTOR_OPE3(fastA_4f vec4_result = normalize(fastA_4f(vec4_1)), "                    ", vec4_result);
		TEST_VECTOR_OPE3(fastA_4f vec4_result = mul(fastA_4f(vec4_1), 10.f), "                    ", vec4_result);
		TEST_VECTOR_OPE3(fastA_4f vec4_result = forward(fastA_4f(vec4_1), 10.f), "                ", vec4_result);
		TEST_VECTOR_OPE1(dot(fastA_4f(vec4_1), fastA_4f(vec4_2)), "                               ");
		TEST_VECTOR_OPE1(normalizedDot(fastA_4f(vec4_1), fastA_4f(vec4_2)), "                     ");
		TEST_VECTOR_OPE3(fastA_4f vec4_result = cross(fastA_4f(vec4_1), fastA_4f(vec4_2)), "      ", vec4_result);

		TEST_VECTOR_OPE1(norm(fastestA_4f(vec4_1)), "                                                      ");
		TEST_VECTOR_OPE1(normSq(fastestA_4f(vec4_1)), "                                                    ");
		TEST_VECTOR_OPE3(fastestA_4f vec4_result = merge(fastestA_4f(vec4_1), fastestA_4f(vec4_2)), "      ", vec4_result);
		TEST_VECTOR_OPE3(fastestA_4f vec4_result = difference(fastestA_4f(vec4_1), fastestA_4f(vec4_2)), " ", vec4_result);
		TEST_VECTOR_OPE1(length(fastestA_4f(vec4_1), fastestA_4f(vec4_2)), "                               ");
		TEST_VECTOR_OPE1(lengthSq(fastestA_4f(vec4_1), fastestA_4f(vec4_2)), "                             ");
		TEST_VECTOR_OPE3(fastestA_4f vec4_result = normalize(fastestA_4f(vec4_1)), "                       ", vec4_result);
		TEST_VECTOR_OPE3(fastestA_4f vec4_result = mul(fastestA_4f(vec4_1), 10.f), "                       ", vec4_result);
		TEST_VECTOR_OPE3(fastestA_4f vec4_result = forward(fastestA_4f(vec4_1), 10.f), "                   ", vec4_result);
		TEST_VECTOR_OPE1(dot(fastestA_4f(vec4_1), fastestA_4f(vec4_2)), "                                  ");
		TEST_VECTOR_OPE1(normalizedDot(fastestA_4f(vec4_1), fastestA_4f(vec4_2)), "                        ");
		TEST_VECTOR_OPE3(fastestA_4f vec4_result = cross(fastestA_4f(vec4_1), fastestA_4f(vec4_2)), "      ", vec4_result);

		TEST_VECTOR_OPE1(norm(semiA_4f(vec4_1)), "                                                ");
		TEST_VECTOR_OPE1(normSq(semiA_4f(vec4_1)), "                                              ");
		TEST_VECTOR_OPE3(semiA_4f vec4_result = merge(semiA_4f(vec4_1), semiA_4f(vec4_2)), "      ", vec4_result);
		TEST_VECTOR_OPE3(semiA_4f vec4_result = difference(semiA_4f(vec4_1), semiA_4f(vec4_2)), " ", vec4_result);
		TEST_VECTOR_OPE1(length(semiA_4f(vec4_1), semiA_4f(vec4_2)), "                            ");
		TEST_VECTOR_OPE1(lengthSq(semiA_4f(vec4_1), semiA_4f(vec4_2)), "                          ");
		TEST_VECTOR_OPE3(semiA_4f vec4_result = normalize(semiA_4f(vec4_1)), "                    ", vec4_result);
		TEST_VECTOR_OPE3(semiA_4f vec4_result = mul(semiA_4f(vec4_1), 10.f), "                    ", vec4_result);
		TEST_VECTOR_OPE3(semiA_4f vec4_result = forward(semiA_4f(vec4_1), 10.f), "                ", vec4_result);
		TEST_VECTOR_OPE1(dot(semiA_4f(vec4_1), semiA_4f(vec4_2)), "                               ");
		TEST_VECTOR_OPE1(normalizedDot(semiA_4f(vec4_1), semiA_4f(vec4_2)), "                     ");
		TEST_VECTOR_OPE3(semiA_4f vec4_result = cross(semiA_4f(vec4_1), semiA_4f(vec4_2)), "      ", vec4_result);

		TEST_VECTOR_OPE1(norm(sseA_4f(vec4_1)), "                                              ");
		TEST_VECTOR_OPE1(normSq(sseA_4f(vec4_1)), "                                            ");
		TEST_VECTOR_OPE3(sseA_4f vec4_result = merge(sseA_4f(vec4_1), sseA_4f(vec4_2)), "      ", vec4_result);
		TEST_VECTOR_OPE3(sseA_4f vec4_result = difference(sseA_4f(vec4_1), sseA_4f(vec4_2)), " ", vec4_result);
		TEST_VECTOR_OPE1(length(sseA_4f(vec4_1), sseA_4f(vec4_2)), "                           ");
		TEST_VECTOR_OPE1(lengthSq(sseA_4f(vec4_1), sseA_4f(vec4_2)), "                         ");
		TEST_VECTOR_OPE3(sseA_4f vec4_result = normalize(sseA_4f(vec4_1)), "                   ", vec4_result);
		TEST_VECTOR_OPE3(sseA_4f vec4_result = mul(sseA_4f(vec4_1), 10.f), "                   ", vec4_result);
		TEST_VECTOR_OPE3(sseA_4f vec4_result = forward(sseA_4f(vec4_1), 10.f), "               ", vec4_result);
		TEST_VECTOR_OPE1(dot(sseA_4f(vec4_1), sseA_4f(vec4_2)), "                              ");
		TEST_VECTOR_OPE1(normalizedDot(sseA_4f(vec4_1), sseA_4f(vec4_2)), "                    ");
		TEST_VECTOR_OPE3(sseA_4f vec4_result = cross(sseA_4f(vec4_1), sseA_4f(vec4_2)), "      ", vec4_result);

		TEST_VECTOR_OPE1(norm(normA_4f(vec4_1)), "                                                ");
		TEST_VECTOR_OPE1(normSq(normA_4f(vec4_1)), "                                              ");
		TEST_VECTOR_OPE3(normA_4f vec4_result = merge(normA_4f(vec4_1), normA_4f(vec4_2)), "      ", vec4_result);
		TEST_VECTOR_OPE3(normA_4f vec4_result = difference(normA_4f(vec4_1), normA_4f(vec4_2)), " ", vec4_result);
		TEST_VECTOR_OPE1(length(normA_4f(vec4_1), normA_4f(vec4_2)), "                            ");
		TEST_VECTOR_OPE1(lengthSq(normA_4f(vec4_1), normA_4f(vec4_2)), "                          ");
		TEST_VECTOR_OPE3(normA_4f vec4_result = normalize(normA_4f(vec4_1)), "                    ", vec4_result);
		TEST_VECTOR_OPE3(normA_4f vec4_result = mul(normA_4f(vec4_1), 10.f), "                    ", vec4_result);
		TEST_VECTOR_OPE3(normA_4f vec4_result = forward(normA_4f(vec4_1), 10.f), "                ", vec4_result);
		TEST_VECTOR_OPE1(dot(normA_4f(vec4_1), normA_4f(vec4_2)), "                               ");
		TEST_VECTOR_OPE1(normalizedDot(normA_4f(vec4_1), normA_4f(vec4_2)), "                     ");
		TEST_VECTOR_OPE3(normA_4f vec4_result = cross(normA_4f(vec4_1), normA_4f(vec4_2)), "      ", vec4_result);

		TEST_VECTOR_OPE1(norm(dummyA_4f(vec4_1)), "                                       ");
		TEST_VECTOR_OPE1(normSq(dummyA_4f(vec4_1)), "                                     ");
		TEST_VECTOR_OPE3(merge(vec4_result, dummyA_4f(vec4_1), dummyA_4f(vec4_2)), "      ", vec4_result);
		TEST_VECTOR_OPE3(difference(vec4_result, dummyA_4f(vec4_1), dummyA_4f(vec4_2)), " ", vec4_result);
		TEST_VECTOR_OPE1(length(dummyA_4f(vec4_1), dummyA_4f(vec4_2)), "                  ");
		TEST_VECTOR_OPE1(lengthSq(dummyA_4f(vec4_1), dummyA_4f(vec4_2)), "                ");
		TEST_VECTOR_OPE3(normalize(vec4_result, dummyA_4f(vec4_1)), "                     ", vec4_result);
		TEST_VECTOR_OPE3(mul(vec4_result, dummyA_4f(vec4_1), 10.f), "                     ", vec4_result);
		TEST_VECTOR_OPE3(forward(vec4_result, dummyA_4f(vec4_1), 10.f), "                 ", vec4_result);
		TEST_VECTOR_OPE1(dot(dummyA_4f(vec4_1), dummyA_4f(vec4_2)), "                     ");
		TEST_VECTOR_OPE1(normalizedDot(dummyA_4f(vec4_1), dummyA_4f(vec4_2)), "           ");
		TEST_VECTOR_OPE3(cross(vec4_result, dummyA_4f(vec4_1), dummyA_4f(vec4_2)), "      ", vec4_result);

		printf("\n");
		printf("- 2D vector(double) -\n");
		double vec2d_1[2] = { 1., 2. };
		double vec2d_2[2] = { 3., 4. };
		double vec2d_result[2];
		printf("vec2d_1=[%.5lf][%.5lf]\n", vec2d_1[0], vec2d_1[1]);
		printf("vec2d_2=[%.5lf][%.5lf]\n", vec2d_2[0], vec2d_2[1]);
		TEST_VECTOR_OPE1(norm(vec2d_1), "                              ");
		TEST_VECTOR_OPE1(normSq(vec2d_1), "                            ");
		TEST_VECTOR_OPE2(merge(vec2d_result, vec2d_1, vec2d_2), "      ", vec2d_result);
		TEST_VECTOR_OPE2(difference(vec2d_result, vec2d_1, vec2d_2), " ", vec2d_result);
		TEST_VECTOR_OPE1(length(vec2d_1, vec2d_2), "                   ");
		TEST_VECTOR_OPE1(lengthSq(vec2d_1, vec2d_2), "                 ");
		TEST_VECTOR_OPE2(normalize(vec2d_result, vec2d_1), "           ", vec2d_result);
		TEST_VECTOR_OPE2(mul(vec2d_result, vec2d_1, 10.), "            ", vec2d_result);
		TEST_VECTOR_OPE2(forward(vec2d_result, vec2d_1, 10.), "        ", vec2d_result);
		TEST_VECTOR_OPE1(dot(vec2d_1, vec2d_2), "                      ");
		TEST_VECTOR_OPE1(normalizedDot(vec2d_1, vec2d_2), "            ");
		
		printf("\n");
		printf("- 3D vector (double) -\n");
		double vec3d_1[3] = { 1., 2., 3. };
		double vec3d_2[3] = { 4., 5., 6. };
		double vec3d_result[3];
		printf("vec3dd_1=[%.5lf][%.5lf][%.5lf]\n", vec3d_1[0], vec3d_1[1], vec3d_1[2]);
		printf("vec3dd_2=[%.5lf][%.5lf][%.5lf]\n", vec3d_2[0], vec3d_2[1], vec3d_2[2]);
		TEST_VECTOR_OPE1(norm(vec3d_1), "                              ");
		TEST_VECTOR_OPE1(normSq(vec3d_1), "                            ");
		TEST_VECTOR_OPE3(merge(vec3d_result, vec3d_1, vec3d_2), "      ", vec3d_result);
		TEST_VECTOR_OPE3(difference(vec3d_result, vec3d_1, vec3d_2), " ", vec3d_result);
		TEST_VECTOR_OPE1(length(vec3d_1, vec3d_2), "                   ");
		TEST_VECTOR_OPE1(lengthSq(vec3d_1, vec3d_2), "                 ");
		TEST_VECTOR_OPE3(normalize(vec3d_result, vec3d_1), "           ", vec3d_result);
		TEST_VECTOR_OPE3(mul(vec3d_result, vec3d_1, 10.), "            ", vec3d_result);
		TEST_VECTOR_OPE3(forward(vec3d_result, vec3d_1, 10.), "        ", vec3d_result);
		TEST_VECTOR_OPE1(dot(vec3d_1, vec3d_2), "                      ");
		TEST_VECTOR_OPE1(normalizedDot(vec3d_1, vec3d_2), "            ");
		TEST_VECTOR_OPE3(cross(vec3d_result, vec3d_1, vec3d_2), "      ", vec3d_result);
	}

	//テンプレート行列演算のテスト
	printf("\n");
	printf("--------------------------------------------------------------------------------\n");
	printf("[ Test for matrix operation ]\n");

	{
		float x1[4][4] = { { 1.f, 2.f, 3.f, 4.f }, { 5.f, 6.f, 7.f, 8.f }, { 9.f, 10.f, 11.f, 12.f }, { 13.f, 14.f, 15.f, 16.f } };
		float x2[4][4] = { { .1f, .2f, .3f, .4f }, { .5f, .6f, .7f, .8f }, { .9f, .10f, .11f, .12f }, { .13f, .14f, .15f, .16f } };
		float x3[4][4];
		add(x3, x1, x2);
		printf("add:   x3 = { {%.1f, %.1f, %.1f, %.1f}, {%.1f, %.1f, %.1f, %.1f}, {%.1f, %.1f, %.1f, %.1f}, {%.1f, %.1f, %.1f, %.1f} }\n", x3[0][0], x3[0][1], x3[0][2], x3[0][3], x3[1][0], x3[1][1], x3[1][2], x3[1][3], x3[2][0], x3[2][1], x3[2][2], x3[2][3], x3[3][0], x3[3][1], x3[3][2], x3[3][3]);
		addLU(x3, x1, x2);
		printf("addLU: x3 = { {%.1f, %.1f, %.1f, %.1f}, {%.1f, %.1f, %.1f, %.1f}, {%.1f, %.1f, %.1f, %.1f}, {%.1f, %.1f, %.1f, %.1f} }\n", x3[0][0], x3[0][1], x3[0][2], x3[0][3], x3[1][0], x3[1][1], x3[1][2], x3[1][3], x3[2][0], x3[2][1], x3[2][2], x3[2][3], x3[3][0], x3[3][1], x3[3][2], x3[3][3]);
		sub(x3, x1, x2);
		printf("sub:   x3 = { {%.1f, %.1f, %.1f, %.1f}, {%.1f, %.1f, %.1f, %.1f}, {%.1f, %.1f, %.1f, %.1f}, {%.1f, %.1f, %.1f, %.1f} }\n", x3[0][0], x3[0][1], x3[0][2], x3[0][3], x3[1][0], x3[1][1], x3[1][2], x3[1][3], x3[2][0], x3[2][1], x3[2][2], x3[2][3], x3[3][0], x3[3][1], x3[3][2], x3[3][3]);
		subLU(x3, x1, x2);
		printf("subLU: x3 = { {%.1f, %.1f, %.1f, %.1f}, {%.1f, %.1f, %.1f, %.1f}, {%.1f, %.1f, %.1f, %.1f}, {%.1f, %.1f, %.1f, %.1f} }\n", x3[0][0], x3[0][1], x3[0][2], x3[0][3], x3[1][0], x3[1][1], x3[1][2], x3[1][3], x3[2][0], x3[2][1], x3[2][2], x3[2][3], x3[3][0], x3[3][1], x3[3][2], x3[3][3]);
		mul(x3, x1, 10.f);
		printf("mul:   x3 = { {%.1f, %.1f, %.1f, %.1f}, {%.1f, %.1f, %.1f, %.1f}, {%.1f, %.1f, %.1f, %.1f}, {%.1f, %.1f, %.1f, %.1f} }\n", x3[0][0], x3[0][1], x3[0][2], x3[0][3], x3[1][0], x3[1][1], x3[1][2], x3[1][3], x3[2][0], x3[2][1], x3[2][2], x3[2][3], x3[3][0], x3[3][1], x3[3][2], x3[3][3]);
		mulLU(x3, x1, 10.f);
		printf("mulLU: x3 = { {%.1f, %.1f, %.1f, %.1f}, {%.1f, %.1f, %.1f, %.1f}, {%.1f, %.1f, %.1f, %.1f}, {%.1f, %.1f, %.1f, %.1f} }\n", x3[0][0], x3[0][1], x3[0][2], x3[0][3], x3[1][0], x3[1][1], x3[1][2], x3[1][3], x3[2][0], x3[2][1], x3[2][2], x3[2][3], x3[3][0], x3[3][1], x3[3][2], x3[3][3]);
		mul(x3, x1, x2);
		printf("mul:   x3 = { {%.1f, %.1f, %.1f, %.1f}, {%.1f, %.1f, %.1f, %.1f}, {%.1f, %.1f, %.1f, %.1f}, {%.1f, %.1f, %.1f, %.1f} }\n", x3[0][0], x3[0][1], x3[0][2], x3[0][3], x3[1][0], x3[1][1], x3[1][2], x3[1][3], x3[2][0], x3[2][1], x3[2][2], x3[2][3], x3[3][0], x3[3][1], x3[3][2], x3[3][3]);
		mulLU(x3, x1, x2);
		printf("mulLU: x3 = { {%.1f, %.1f, %.1f, %.1f}, {%.1f, %.1f, %.1f, %.1f}, {%.1f, %.1f, %.1f, %.1f}, {%.1f, %.1f, %.1f, %.1f} }\n", x3[0][0], x3[0][1], x3[0][2], x3[0][3], x3[1][0], x3[1][1], x3[1][2], x3[1][3], x3[2][0], x3[2][1], x3[2][2], x3[2][3], x3[3][0], x3[3][1], x3[3][2], x3[3][3]);
	}
{
		double x1[3][3] = { { 1., 2., 3. }, { 4., 5., 6. }, { 7., 8., 9. } };
		double x2[3][3] = { { .1, .2, .3 }, { .4, .5, .6 }, { .7, .8, .9 } };
		double x3[3][3];
		add(x3, x1, x2);
		printf("add:   x3 = { {%.1lf, %.1lf, %.1lf}, {%.1lf, %.1lf, %.1lf}, {%.1lf, %.1lf, %.1lf} }\n", x3[0][0], x3[0][1], x3[0][2], x3[1][0], x3[1][1], x3[1][2], x3[2][0], x3[2][1], x3[2][2]);
		addLU(x3, x1, x2);
		printf("addLU: x3 = { {%.1lf, %.1lf, %.1lf}, {%.1lf, %.1lf, %.1lf}, {%.1lf, %.1lf, %.1lf} }\n", x3[0][0], x3[0][1], x3[0][2], x3[1][0], x3[1][1], x3[1][2], x3[2][0], x3[2][1], x3[2][2]);
		sub(x3, x1, x2);
		printf("sub:   x3 = { {%.1lf, %.1lf, %.1lf}, {%.1lf, %.1lf, %.1lf}, {%.1lf, %.1lf, %.1lf} }\n", x3[0][0], x3[0][1], x3[0][2], x3[1][0], x3[1][1], x3[1][2], x3[2][0], x3[2][1], x3[2][2]);
		subLU(x3, x1, x2);
		printf("subLU: x3 = { {%.1lf, %.1lf, %.1lf}, {%.1lf, %.1lf, %.1lf}, {%.1lf, %.1lf, %.1lf} }\n", x3[0][0], x3[0][1], x3[0][2], x3[1][0], x3[1][1], x3[1][2], x3[2][0], x3[2][1], x3[2][2]);
		mul(x3, x1, 10.);
		printf("mul:   x3 = { {%.1lf, %.1lf, %.1lf}, {%.1lf, %.1lf, %.1lf}, {%.1lf, %.1lf, %.1lf} }\n", x3[0][0], x3[0][1], x3[0][2], x3[1][0], x3[1][1], x3[1][2], x3[2][0], x3[2][1], x3[2][2]);
		mulLU(x3, x1, 10.);
		printf("mulLU: x3 = { {%.1lf, %.1lf, %.1lf}, {%.1lf, %.1lf, %.1lf}, {%.1lf, %.1lf, %.1lf} }\n", x3[0][0], x3[0][1], x3[0][2], x3[1][0], x3[1][1], x3[1][2], x3[2][0], x3[2][1], x3[2][2]);
		mul(x3, x1, x2);
		printf("mul:   x3 = { {%.1lf, %.1lf, %.1lf}, {%.1lf, %.1lf, %.1lf}, {%.1lf, %.1lf, %.1lf} }\n", x3[0][0], x3[0][1], x3[0][2], x3[1][0], x3[1][1], x3[1][2], x3[2][0], x3[2][1], x3[2][2]);
		mulLU(x3, x1, x2);
		printf("mulLU: x3 = { {%.1lf, %.1lf, %.1lf}, {%.1lf, %.1lf, %.1lf}, {%.1lf, %.1lf, %.1lf} }\n", x3[0][0], x3[0][1], x3[0][2], x3[1][0], x3[1][1], x3[1][2], x3[2][0], x3[2][1], x3[2][2]);
	}

	//パフォーマンス計測
	printf("\n");
	
	//各種高速演算のパフォーマンス計測
	testForOperationPerformanceDirect<float>("direct:float");
	testForOperationPerformance<dummyA_f, float>("dummy:float");
	testForOperationPerformance<normA_f, float>("normal:float");
	testForOperationPerformance<sseA_f, float>("sse:float");
	testForOperationPerformance<fastA_f, float>("fast:float");
	testForOperationPerformance<semiA_f, float>("semifast:float");
	testForOperationPerformance<fastestA_f, float>("fastest:float");

	testForOperationPerformanceDirect<double>("direct:dobule");
	testForOperationPerformance<dummyA_d, double>("dummy:dobule");
	testForOperationPerformance<normA_d, double>("normal:double");
	testForOperationPerformance<sseA_d, double>("sse:double");
	testForOperationPerformance<fastA_d, double>("fast:double");
	testForOperationPerformance<semiA_d, double>("semifast:double");
	testForOperationPerformance<fastestA_d, double>("fastest:double");

	//各種高速平方根のパフォーマンス計測
	testForSqrtPerformanceDirect<float>("direct:float");
	testForSqrtPerformance<dummyA_f, float>("dummy:float");
	testForSqrtPerformance<normA_f, float>("normal:float");
	testForSqrtPerformance<sseA_f, float>("sse:float");
	testForSqrtPerformance<fastA_f, float>("fast:float");
	testForSqrtPerformance<semiA_f, float>("semifast:float");
	testForSqrtPerformance<fastestA_f, float>("fastest:float");

	testForSqrtPerformanceDirect<double>("direct:dobule");
	testForSqrtPerformance<dummyA_d, double>("dummy:dobule");
	testForSqrtPerformance<normA_d, double>("normal:double");
	testForSqrtPerformance<sseA_d, double>("sse:double");
	testForSqrtPerformance<fastA_d, double>("fast:double");
	testForSqrtPerformance<semiA_d, double>("semifast:double");
	testForSqrtPerformance<fastestA_d, double>("fastest:double");

	//ベクトル演算のパフォーマンス計測
	testForVectorPerformanceDirect<float, 2>("direct:float[2]");
	testForVectorPerformanceDummy<dummyA_2f, float, 2>("dummy:float[2]");
	testForVectorPerformance<normA_2f, float, 2>("normal:float[2]");
	testForVectorPerformance<sseA_2f, float, 2>("sse:float[2]");
	testForVectorPerformance<fastA_2f, float, 2>("fast:float[2]");
	testForVectorPerformance<semiA_2f, float, 2>("semifast:float[2]");
	testForVectorPerformance<fastestA_2f, float, 2>("fastest:float[2]");

	testForVectorPerformanceDirect<float, 3>("direct:float[3]");
	testForVectorPerformanceDummy<dummyA_3f, float, 3>("dummy:float[3]");
	testForVectorPerformance<normA_3f, float, 3>("normal:float[3]");
	testForVectorPerformance<sseA_3f, float, 3>("sse:float[3]");
	testForVectorPerformance<fastA_3f, float, 3>("fast:float[3]");
	testForVectorPerformance<semiA_3f, float, 3>("semifast:float[3]");
	testForVectorPerformance<fastestA_3f, float, 3>("fastest:float[3]");

	testForVectorPerformanceDirect<float, 4>("direct:float[4]");
	testForVectorPerformanceDummy<dummyA_4f, float, 4>("dummy:float[4]");
	testForVectorPerformance<normA_4f, float, 4>("normal:float[4]");
	testForVectorPerformance<sseA_4f, float, 4>("sse:float[4]");
	testForVectorPerformance<fastA_4f, float, 4>("fast:float[4]");
	testForVectorPerformance<semiA_4f, float, 4>("semifast:float[4]");
	testForVectorPerformance<fastestA_4f, float, 4>("fastest:float[4]");

	testForVectorPerformanceDirect<double, 2>("direct:double[2]");
	testForVectorPerformanceDummy<dummyA_2d, double, 2>("dummy:double[2]");
	testForVectorPerformance<normA_2d, double, 2>("normal:double[2]");
	testForVectorPerformance<sseA_2d, double, 2>("sse:double[2]");
	testForVectorPerformance<fastA_2d, double, 2>("fast:double[2]");
	testForVectorPerformance<semiA_2d, double, 2>("semifast:double[2]");
	testForVectorPerformance<fastestA_2d, double, 2>("fastest:double[2]");

	testForVectorPerformanceDirect<double, 3>("direct:double[3]");
	testForVectorPerformanceDummy<dummyA_3d, double, 3>("dummy:double[3]");
	testForVectorPerformance<normA_3d, double, 3>("normal:double[3]");
	testForVectorPerformance<sseA_3d, double, 3>("sse:double[3]");
	testForVectorPerformance<fastA_3d, double, 3>("fast:double[3]");
	testForVectorPerformance<semiA_3d, double, 3>("semifast:double[3]");
	testForVectorPerformance<fastestA_3d, double, 3>("fastest:double[3]");

	testForVectorPerformanceDirect<double, 4>("direct:double[4]");
	testForVectorPerformanceDummy<dummyA_4d, double, 4>("dummy:double[4]");
	testForVectorPerformance<normA_4d, double, 4>("normal:double[4]");
	testForVectorPerformance<sseA_4d, double, 4>("sse:double[4]");
	testForVectorPerformance<fastA_4d, double, 4>("fast:double[4]");
	testForVectorPerformance<semiA_4d, double, 4>("semifast:double[4]");
	testForVectorPerformance<fastestA_4d, double, 4>("fastest:double[4]");

	//行列演算のパフォーマンス計測
	testForMatrixPerformanceDirect<float, 2, 2>("diredt:float[2][2]");
	testForMatrixPerformanceDirectLU<float, 2, 2>("diredt(RU):float[2][2](RU)");
	testForMatrixPerformanceDummy<dummyA_22f, float, 2, 2>("dummy:float[2][2]");
	testForMatrixPerformance<normA_22f, float, 2, 2>("normal:float[2][2]");
	testForMatrixPerformance<sseA_22f, float, 2, 2>("sse:float[2][2]");
	testForMatrixPerformance<fastA_22f, float, 2, 2>("fast:float[2][2]");
	testForMatrixPerformance<fastestA_22f, float, 2, 2>("fastest:float[2][2]");
	testForMatrixPerformance<semiA_22f, float, 2, 2>("semifast:float[2][2]");

	testForMatrixPerformanceDirect<float, 3, 3>("diredt:float[3][3]");
	testForMatrixPerformanceDirectLU<float, 3, 3>("diredt(RU):float[3][3](RU)");
	testForMatrixPerformanceDummy<dummyA_33f, float, 3, 3>("dummy:float[3][3]");
	testForMatrixPerformance<normA_33f, float, 3, 3>("normal:float[3][3]");
	testForMatrixPerformance<sseA_33f, float, 3, 3>("sse:float[3][3]");
	testForMatrixPerformance<fastA_33f, float, 3, 3>("fast:float[3][3]");
	testForMatrixPerformance<fastestA_33f, float, 3, 3>("fastest:float[3][3]");
	testForMatrixPerformance<semiA_33f, float, 3, 3>("semifast:float[3][3]");

	testForMatrixPerformanceDirect<float, 4, 4>("diredt:float[4][4]");
	testForMatrixPerformanceDirectLU<float, 4, 4>("diredt(RU):float[4][4](RU)");
	testForMatrixPerformanceDummy<dummyA_44f, float, 4, 4>("dummy:float[4][4]");
	testForMatrixPerformance<normA_44f, float, 4, 4>("normal:float[4][4]");
	testForMatrixPerformance<sseA_44f, float, 4, 4>("sse:float[4][4]");
	testForMatrixPerformance<fastA_44f, float, 4, 4>("fast:float[4][4]");
	testForMatrixPerformance<fastestA_44f, float, 4, 4>("fastest:float[4][4]");
	testForMatrixPerformance<semiA_44f, float, 4, 4>("semifast:float[4][4]");

	testForMatrixPerformanceDirect<double, 2, 2>("diredt:double[2][2]");
	testForMatrixPerformanceDirectLU<double, 2, 2>("diredt(RU):double[2][2](RU)");
	testForMatrixPerformanceDummy<dummyA_22d, double, 2, 2>("dummy:double[2][2]");
	testForMatrixPerformance<normA_22d, double, 2, 2>("normal:double[2][2]");
	testForMatrixPerformance<sseA_22d, double, 2, 2>("sse:double[2][2]");
	testForMatrixPerformance<fastA_22d, double, 2, 2>("fast:double[2][2]");
	testForMatrixPerformance<fastestA_22d, double, 2, 2>("fastest:double[2][2]");
	testForMatrixPerformance<semiA_22d, double, 2, 2>("semifast:double[2][2]");

	testForMatrixPerformanceDirect<double, 3, 3>("diredt:double[3][3]");
	testForMatrixPerformanceDirectLU<double, 3, 3>("diredt(RU):double[3][3](RU)");
	testForMatrixPerformanceDummy<dummyA_33d, double, 3, 3>("dummy:double[3][3]");
	testForMatrixPerformance<normA_33d, double, 3, 3>("normal:double[3][3]");
	testForMatrixPerformance<sseA_33d, double, 3, 3>("sse:double[3][3]");
	testForMatrixPerformance<fastA_33d, double, 3, 3>("fast:double[3][3]");
	testForMatrixPerformance<fastestA_33d, double, 3, 3>("fastest:double[3][3]");
	testForMatrixPerformance<semiA_33d, double, 3, 3>("semifast:double[3][3]");

	testForMatrixPerformanceDirect<double, 4, 4>("diredt:double[4][4]");
	testForMatrixPerformanceDirectLU<double, 4, 4>("diredt(RU):double[4][4](RU)");
	testForMatrixPerformanceDummy<dummyA_44d, double, 4, 4>("dummy:double[4][4]");
	testForMatrixPerformance<normA_44d, double, 4, 4>("normal:double[4][4]");
	testForMatrixPerformance<sseA_44d, double, 4, 4>("sse:double[4][4]");
	testForMatrixPerformance<fastA_44d, double, 4, 4>("fast:double[4][4]");
	testForMatrixPerformance<fastestA_44d, double, 4, 4>("fastest:double[4][4]");
	testForMatrixPerformance<semiA_44d, double, 4, 4>("semifast:double[4][4]");
}

// End of file
