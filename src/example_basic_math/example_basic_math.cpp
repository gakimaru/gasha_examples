//--------------------------------------------------------------------------------
// exmaple_basic_math.cpp
// 基本算術処理テスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_basic_math.h"//基本算術処理テスト

#include <gasha/basic_math.h>//基本算術
#include <gasha/memory.h>//メモリ操作：アライメント計算
#include <gasha/chrono.h>//時間処理ユーティリティ：nowTime(), calcElapsedTime()

#include <cstdio>//std::printf()
#include <cstdarg>//va_list

GASHA_USING_NAMESPACE;//ネームスペース使用

//----------------------------------------
//【ランタイム版】素数判定／算出テスト
///※isPrime(), makePrimeLT(), makePrimeLE(), makePrimeGT(), makePrimeGE() をテスト
void printPrime(const unsigned int min, const unsigned int max)
{
	if (max > min)
		printPrime(min, max - 1);
	std::printf("%6d is %s [prev=%6d(%6d), next=%6d(%6d)]\n", max, isPrime(max) ? "PRIME.    " : "NOT prime.", makePrimeLT(max), makePrimeLE(max), makePrimeGT(max), makePrimeGE(max));
}

//----------------------------------------
//【メタプログラミング版】素数判定／算出テスト
//※isStaticPrime, makeStaticPrimeLT, makeStaticPrimeLE, makeStaticPrimeGT, makeStaticPrimeGE をテスト
template<unsigned int N>
void _printPrimeCommon()
{
	std::printf("%6d is %s [prev=%6d(%6d), next=%6d(%6d)]\n", N, isStaticPrime<N>::value ? "PRIME.    " : "NOT prime.", makeStaticPrimeLT<N>::value, makeStaticPrimeLE<N>::value, makeStaticPrimeGT<N>::value, makeStaticPrimeGE<N>::value);
}
template<unsigned int MIN, unsigned int MAX>
struct printStaticPrime{
	void operator()()
	{
		printStaticPrime<MIN, MAX - 1>()();
		_printPrimeCommon<MAX>();
	}
};
template<unsigned int MIN>
struct printStaticPrime<MIN, MIN>{
	void operator()()
	{
		_printPrimeCommon<MIN>();
	}
};

//----------------------------------------
//【ランタイム版】ビット計算テスト
///※countBits(), calcMSB(), calcLSB(), calcNearPow2GE(), calcNearPow2LE(), adjustAlign() をテスト
template<unsigned int N>
void printCalcBits(const unsigned int (&values)[N])
{
	for (unsigned int i = 0; i < N; ++i)
	{
		const unsigned int value = values[i];
		std::printf("0x%08x : count=%2d, MSB=%2d, LSB=%2d, near-pow2(GE)=0x%08x, near-pow2(LE)=0x%08x, adjustAlign<ALIGN>=0x%08x,0x%08x(ALIGN=4,256)\n", value, countBits(value), calcMSB(value), calcLSB(value), calcNearPow2GE(value), calcNearPow2LE(value), adjustAlign<4>(value), adjustAlign<256>(value));
	}
}

//----------------------------------------
//【メタプログラミング版】ビット計算テスト
///※countBits(), calcMSB(), calcLSB(), calcNearPow2GE(), calcNearPow2LE(), adjustAlign() をテスト
template<unsigned int N>
void _printCalcStaticBits()
{
	std::printf("0x%08x : count=%2d, MSB=%2d, LSB=%2d, near-pow2(GE)=0x%08x, near-pow2(LE)=0x%08x, adjustAlign<ALIGN>=0x%08x,0x%08x(ALIGN=4,256)\n", N, countStaticBits<N>::value, calcStaticMSB<N>::value, calcStaticLSB<N>::value, calcStaticNearPow2GE<N>::value, calcStaticNearPow2LE<N>::value, adjustStaticAlign<N, 4>::value, adjustStaticAlign<N, 256>::value);
}
template<unsigned int N, unsigned int... Nx>
struct printCalcStaticBits{
	void operator()()
	{
		_printCalcStaticBits<N>();
		printCalcStaticBits<Nx...>()();
	}
};
template<unsigned int N>
struct printCalcStaticBits<N>{
	void operator()()
	{
		_printCalcStaticBits<N>();
	}
};

//----------------------------------------
//基本算術処理テスト
void example_basic_math()
{
	//--------------------
	//【ランタイム版（半メタプログラミング版）】べき乗テスト
	//※関数の再帰呼び出しを静的に展開するので、指数が小さければ関数が消滅して高速になる（はず）。
	//※引数が定数なら、コンパイル時に計算が済む可能性も高い。
	std::printf("\n");
	std::printf("----- Calculate pow for Runtime -----\n");
	{
		const int ni0 = pow<int, 0>(2);
		const int ni1 = pow<int, 1>(2);
		const int ni2 = pow<int, 2>(2);
		const int ni3 = pow<int, 3>(2);
		const int ni4 = pow<int, 4>(2);
		const float nf0 = pow<float, 0>(2.f);
		const float nf1 = pow<float, 1>(2.f);
		const float nf2 = pow<float, 2>(2.f);
		const float nf3 = pow<float, 3>(2.f);
		const float nf4 = pow<float, 4>(2.f);
		const float nfm0 = negaPow<float, 0>(2.f);
		const float nfm1 = negaPow<float, -1>(2.f);
		const float nfm2 = negaPow<float, -2>(2.f);
		const float nfm3 = negaPow<float, -3>(2.f);
		const float nfm4 = negaPow<float, -4>(2.f);
		std::printf("pow<int, e>(2) = {%d, %d, %d, %d, %d} (e=0, 1, 2, 3, 4)\n", ni0, ni1, ni2, ni3, ni4);
		std::printf("pow<float, e>(2.f) = {%.1f, %.1f, %.1f, %.1f, %.1f} (e=0, 1, 2, 3, 4)\n", nf0, nf1, nf2, nf3, nf4);
		std::printf("negaPow<float, -e>(2.f) = {%.4f, %.4f, %.4f, %.4f, %.4f} (e=0, -1, -2, -3, -4)\n", nfm0, nfm1, nfm2, nfm3, nfm4);
	}

	//--------------------
	//【メタプログラミング版】べき乗テスト
	std::printf("\n");
	std::printf("----- Calculate pow for Meta-programming -----\n");
	{
		const int n0 = staticPow<int, 2, 0>::value;
		const int n1 = staticPow<int, 2, 1>::value;
		const int n2 = staticPow<int, 2, 2>::value;
		const int n3 = staticPow<int, 2, 3>::value;
		const int n4 = staticPow<int, 2, 4>::value;
		std::printf("staticPow<2, e> = {%d, %d, %d, %d, %d} (e=0, 1, 2, 3, 4)\n", n0, n1, n2, n3, n4);
	}
	
	//--------------------
	//素数判定／算出テスト
	static const unsigned int MIN = 0;
	static const unsigned int MAX = 10;

	//【ランタイム版】素数判定／算出テスト
	///※isPrime(), makePrimeLT(), makePrimeLE(), makePrimeGT(), makePrimeGE() をテスト。
	std::printf("\n");
	std::printf("----- Check and Make Prime for Runtime -----\n");
	printPrime(MIN, MAX);

	//【メタプログラミング版】素数判定／算出テスト
	//※isStaticPrime, makeStaticPrimeLT, makeStaticPrimeLE, makeStaticPrimeGT, makeStaticPrimeGE をテスト。
	std::printf("\n");
	std::printf("----- Check and Make Prime for Meta-programming -----\n");
	printStaticPrime<MIN, MAX>()();

	//--------------------
	//【ランタイム版】ビット計算テスト
	//※countBits(), calcMSB(), calcLSB(), calcNearPow2GE(), calcNearPow2LE(), adjustAlign() をテスト。
	#define CALC_BITS_TEST_PATTERN 0x00000000, 0xffffffff, 0x80000000, 0x00000001, 0x55555555, 0xaaaaaaaa, 0xffff0000, 0x0000ffff, 0x00ffff00, 0x12345678, 0xfedcba98, 0x11111111, 0x88888888, 0x01010101, 0x8080800, 0x00010001, 0x80008000
	std::printf("\n");
	std::printf("----- Calculate bits for Runtime -----\n");
	const unsigned int values[] = { CALC_BITS_TEST_PATTERN };
	printCalcBits(values);

	//--------------------
	//【メタプログラミング版】ビット計算テスト
	//※countStaticBits(), calcStaticMSB(), calcStaticLSB(), calcStaticNearPow2GE(), calcStaticNearPow2LE(), adjustStaticAlign() をテスト。
	//※やや強引なテストのため、コンパイル時に警告が出る。
	std::printf("\n");
	std::printf("----- Calculate bits for Meta-programming -----\n");
	printCalcStaticBits<CALC_BITS_TEST_PATTERN>()();

	//--------------------
	//パフォーマンステスト
	//※ビット数計測の処理時間を計測
	{
		std::printf("\n");
		std::printf("----- Performance test for coutnBits() -----\n");
		auto prev_time = nowTime();
		unsigned int bit_count_sum;
		{
			std::printf("[countBits_custom() * %d times]\n", TEST_BITCOUNT_MAX);
			bit_count_sum = 0;
			for (int value = 0; value < TEST_BITCOUNT_MAX; ++value)
				bit_count_sum += countBits_custom(value);
		}
		auto print_elapsed_time = [](const std::chrono::system_clock::time_point prev_time, const unsigned int bit_count_sum)
		{
			const double elapsed_time = calcElapsedTime(prev_time);
			std::printf("*elapsed time = %.9lf sec (bit_count_sum=0x%08x)\n", elapsed_time, bit_count_sum);
		};
	#ifdef ENABLE_BUILTIN_POPCNT
		print_elapsed_time(prev_time, bit_count_sum);
		prev_time = nowTime();
		{
			std::printf("[countBits_builtin() * %d times]\n", TEST_BITCOUNT_MAX);
			bit_count_sum = 0;
			for (int value = 0; value < TEST_BITCOUNT_MAX; ++value)
				bit_count_sum += countBits_builtin(value);
		}
		print_elapsed_time(prev_time, bit_count_sum);
	#endif//ENABLE_BUILTIN_POPCNT
	#ifdef ENABLE_SSE_POPCNT
		prev_time = nowTime();
		{
			std::printf("[countBits_sse() * %d times]\n", TEST_BITCOUNT_MAX);
			bit_count_sum = 0;
			for (int value = 0; value < TEST_BITCOUNT_MAX; ++value)
				bit_count_sum += countBits_sse(value);
		}
		print_elapsed_time(prev_time, bit_count_sum);
	#endif//ENABLE_SSE_POPCNT
	}
}

// End of file
