//--------------------------------------------------------------------------------
// example_crc32.cpp
// CRC32計算処理テスト
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_crc32.h"//CRC32計算処理テスト

#include <gasha/crc32.h>//CRC32計算
#include <gasha/chrono.h>//時間処理ユーティリティ：nowTime(), calcElapsedTime()
#include <gasha/simple_assert.h>//シンプルアサーション

#include <cstdio>//std::printf()

GASHA_USING_NAMESPACE;//ネームスペース使用

//CRC32計算処理テスト
void example_crc32()
{
#if 0
	//CRC多項式テーブルを作成
	{
		std::printf("\n");
		makeAndPrintPolyTable();
	}
#endif

	//【constexprによるCRC32計算テスト】
	//※constexpr未対応のコンパイラでも一応使える
	//　ただし、ランタイム時の実行となる。遅い。
	//※constexprでコンパイル時に計算されると、static_assertが正常に反応し、
	//　かつ、文字列定数 "1234567890" が実行コード上から削除される。
	//※calcStaticCRC32()の結果を const型で受けないとコンパイル時に処理されず、
	//　static_assertがコンパイルエラーになってしまう。
	{
		std::printf("\n");
		const crc32_t crc = calcStaticCRC32("1234567890");//constexprでコンパイル時に計算 ※文字列リテラルも消滅 ※const変数に代入しないとコンパイル時に計算されないので注意
	#ifdef GASHA_HAS_CONSTEXPR//constexpr使用可能時はstatic_assertで結果をコンパイル時にチェック
	#ifndef GASHA_CRC32_IS_CRC32C//標準CRC-32(IEEE 802.3)
		static_assert(crc == 0x261daee5u, "CRC32-result is not matched with expectation.");
	#else//GASHA_CRC32_IS_CRC32C//CRC-32C
		static_assert(crc == 0xf3dbd4feu, "CRC32-result is not matched with expectation.");
	#endif//GASHA_CRC32_IS_CRC32C
	#else//GASHA_HAS_CONSTEXPR
	#ifndef GASHA_CRC32_IS_CRC32C//標準CRC-32(IEEE 802.3)
			GASHA_SIMPLE_ASSERT(crc == 0x261daee5u, "CRC32-result is not matched with expectation.");
	#else//GASHA_CRC32_IS_CRC32C//CRC-32C
			GASHA_SIMPLE_ASSERT(crc == 0xf3dbd4feu, "CRC32-result is not matched with expectation.");
	#endif//GASHA_CRC32_IS_CRC32C
	#endif//GASHA_HAS_CONSTEXPR
		std::printf("Result of calculation CRC of \"1234567890\" by constexpr = 0x%08x\n", crc);
	}
	
#ifdef GASHA_HAS_USER_DEFINED_LITERAL//constexpr使用可能時はstatic_assertで結果をコンパイル時にチェック
	//【ユーザー定義リテラルによるCRC32計算テスト】
	//※ユーザー定義リテラル未対応のコンパイラでは使用不可。
	//※文字列定数の状態やstatic_assertの挙動については、上記のconstexprと同じ。
	{
		std::printf("\n");
		const crc32_t crc = "abcdefghij"_crc32;//ユーザー定義リテラルでコンパイル時に計算 ※文字列リテラルも消滅 ※const変数に代入しないとコンパイル時に計算されないので注意
	#ifndef GASHA_CRC32_IS_CRC32C//標準CRC-32(IEEE 802.3)
		static_assert(crc == 0x3981703au, "CRC32-result is not matched with expectation.");
	#else//GASHA_CRC32_IS_CRC32C//CRC-32C
		static_assert(crc == 0xe6599437u, "CRC32-result is not matched with expectation.");
	#endif//GASHA_CRC32_IS_CRC32C
		std::printf("Result of calculation CRC of \"abcdefghij\" by user-defined-literal = 0x%08x\n", crc);
	}
#endif//GASHA_HAS_USER_DEFINED_LITERAL
	
	//【ランタイム関数によるCRC32計算テスト】
	{
		std::printf("\n");
		const crc32_t crc = calcCRC32("ABCDEFGHIJ");
	#ifndef GASHA_CRC32_IS_CRC32C//標準CRC-32(IEEE 802.3)
		GASHA_SIMPLE_ASSERT(crc == 0x321e6d05u, "CRC32-result is not matched with expectation.");
	#else//GASHA_CRC32_IS_CRC32C//CRC-32C
		GASHA_SIMPLE_ASSERT(crc == 0xd599aefdu, "CRC32-result is not matched with expectation.");
	#endif//GASHA_CRC32_IS_CRC32C
		std::printf("Result of calculation CRC of \"ABCDEFGHIJ\" by runtime-function = 0x%08x\n", crc);
	}

	//パフォーマンステスト
	{
		std::printf("\n");
		static const char* str = "1234567890";
		crc32_t crc_sum;
		auto prev_time = nowTime();
		{
			std::printf("[calcCRC32_recursive() * %d times]\n", TEST_REPEAT_COUNT);
			crc_sum = 0;
			for (int loop = 0; loop < TEST_REPEAT_COUNT; ++loop)
				crc_sum += testCRC32ForPerforman_recursive(str, loop);
		}
		auto print_elapsed_time = [](const std::chrono::system_clock::time_point prev_time, crc32_t crc_sum)
		{
			const double elapsed_time = calcElapsedTime(prev_time);
			std::printf("*elapsed time = %.9lf sec (crc_sum=0x%08x)\n", elapsed_time, crc_sum);
		};
		print_elapsed_time(prev_time, crc_sum);
		prev_time = nowTime();
		{
			std::printf("[calcCRC32_loop() * %d times]\n", TEST_REPEAT_COUNT);
			crc_sum = 0;
			for (int loop = 0; loop < TEST_REPEAT_COUNT; ++loop)
				crc_sum += testCRC32ForPerforman_loop(str, loop);
		}
		print_elapsed_time(prev_time, crc_sum);
		prev_time = nowTime();
		{
			std::printf("[calcCRC32_table() * %d times]\n", TEST_REPEAT_COUNT);
			crc_sum = 0;
			for (int loop = 0; loop < TEST_REPEAT_COUNT; ++loop)
				crc_sum += testCRC32ForPerforman_table(str, loop);
		}
		print_elapsed_time(prev_time, crc_sum);
		prev_time = nowTime();
		{
			std::printf("[calcCRC32_sse() * %d times]\n", TEST_REPEAT_COUNT);
			crc_sum = 0;
			for (int loop = 0; loop < TEST_REPEAT_COUNT; ++loop)
				crc_sum += testCRC32ForPerforman_sse(str, loop);
		}
		print_elapsed_time(prev_time, crc_sum);
	}
}

// End of file
