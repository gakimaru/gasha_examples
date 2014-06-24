//--------------------------------------------------------------------------------
// example_crc32.cpp
// CRC32計算処理テスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_crc32.h"//CRC32計算処理テスト

#include <gasha/crc32.h>//CRC32計算

#include <stdio.h>//printf()
#include <assert.h>//assert()
#include <chrono>//C++11 std::chrono

USING_NAMESPACE_GASHA;//ネームスペース使用

//CRC32計算処理テスト
void example_crc32()
{
#if 0
	//CRC多項式テーブルを作成
	{
		printf("\n");
		makeAndPrintPolyTable();
	}
#endif

	//【constexprによるCRC32計算テスト】
	//※constexpr未対応のコンパイラでも一応使える
	//　ただし、ランタイム時の実行となる。遅い。
	//※constexprでコンパイル時に計算されると、static_assertが正常に反応し、
	//　かつ、文字列定数 "1234567890" が実行コード上から削除される。
	//※calcConstCRC32()の結果を const型で受けないとコンパイル時に処理されず、
	//　static_assertがコンパイルエラーになってしまう。
	{
		printf("\n");
		const crc32_t crc = calcConstCRC32("1234567890");//constexprでコンパイル時に計算 ※文字列リテラルも消滅 ※const変数に代入しないとコンパイル時に計算されないので注意
	#ifdef HAS_CONSTEXPR//constexpr使用可能時はstatic_assertで結果をコンパイル時にチェック
	#ifndef CRC32_IS_CRC32C//標準CRC-32(IEEE 802.3)
		static_assert(crc == 0x261daee5u, "invalid crc");
	#else//CRC32_IS_CRC32C//CRC-32C
		static_assert(crc == 0xf3dbd4feu, "invalid crc");
	#endif//CRC32_IS_CRC32C
	#else//HAS_CONSTEXPR
	#ifndef CRC32_IS_CRC32C//標準CRC-32(IEEE 802.3)
			assert(crc == 0x261daee5u);
	#else//CRC32_IS_CRC32C//CRC-32C
			assert(crc == 0xf3dbd4feu);
	#endif//CRC32_IS_CRC32C
	#endif//HAS_CONSTEXPR
		printf("Result of calculation CRC of \"1234567890\" by constexpr = 0x%08x\n", crc);
	}
	
#ifdef HAS_USER_DEFINED_LITERAL//constexpr使用可能時はstatic_assertで結果をコンパイル時にチェック
	//【ユーザー定義リテラルによるCRC32計算テスト】
	//※ユーザー定義リテラル未対応のコンパイラでは使用不可。
	//※文字列定数の状態やstatic_assertの挙動については、上記のconstexprと同じ。
	{
		printf("\n");
		const crc32_t crc = "abcdefghij"_crc32;//ユーザー定義リテラルでコンパイル時に計算 ※文字列リテラルも消滅 ※const変数に代入しないとコンパイル時に計算されないので注意
	#ifndef CRC32_IS_CRC32C//標準CRC-32(IEEE 802.3)
		static_assert(crc == 0x3981703au, "invalid crc");
	#else//CRC32_IS_CRC32C//CRC-32C
		static_assert(crc == 0xe6599437u, "invalid crc");
	#endif//CRC32_IS_CRC32C
		printf("Result of calculation CRC of \"abcdefghij\" by user-defined-literal = 0x%08x\n", crc);
	}
#endif//HAS_USER_DEFINED_LITERAL
	
	//【ランタイム関数によるCRC32計算テスト】
	{
		printf("\n");
		const crc32_t crc = calcCRC32("ABCDEFGHIJ");
	#ifndef CRC32_IS_CRC32C//標準CRC-32(IEEE 802.3)
		assert(crc == 0x321e6d05u);
	#else//CRC32_IS_CRC32C//CRC-32C
		assert(crc == 0xd599aefdu);
	#endif//CRC32_IS_CRC32C
		printf("Result of calculation CRC of \"ABCDEFGHIJ\" by runtime-function = 0x%08x\n", crc);
	}

	//パフォーマンステスト
	{
		printf("\n");
		static const char* str = "1234567890";
		crc32_t crc_sum;
		auto prev_time = std::chrono::system_clock::now();
		{
			printf("[calcCRC32_recursive() * %d times]\n", TEST_REPEAT_COUNT);
			crc_sum = 0;
			for (int loop = 0; loop < TEST_REPEAT_COUNT; ++loop)
				crc_sum += testCRC32ForPerforman_recursive(str, loop);
		}
		auto print_elapsed_time = [](const std::chrono::system_clock::time_point prev_time, crc32_t crc_sum)
		{
			const auto end_time = std::chrono::system_clock::now();
			const auto duration_time = end_time - prev_time;
			const double elapsed_time = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(duration_time).count()) / 1000000000.;
			printf("*elapsed time = %.9lf sec (crc_sum=0x%08x)\n", elapsed_time, crc_sum);
		};
		print_elapsed_time(prev_time, crc_sum);
		prev_time = std::chrono::system_clock::now();
		{
			printf("[calcCRC32_loop() * %d times]\n", TEST_REPEAT_COUNT);
			crc_sum = 0;
			for (int loop = 0; loop < TEST_REPEAT_COUNT; ++loop)
				crc_sum += testCRC32ForPerforman_loop(str, loop);
		}
		print_elapsed_time(prev_time, crc_sum);
		prev_time = std::chrono::system_clock::now();
		{
			printf("[calcCRC32_table() * %d times]\n", TEST_REPEAT_COUNT);
			crc_sum = 0;
			for (int loop = 0; loop < TEST_REPEAT_COUNT; ++loop)
				crc_sum += testCRC32ForPerforman_table(str, loop);
		}
		print_elapsed_time(prev_time, crc_sum);
		prev_time = std::chrono::system_clock::now();
		{
			printf("[calcCRC32_sse() * %d times]\n", TEST_REPEAT_COUNT);
			crc_sum = 0;
			for (int loop = 0; loop < TEST_REPEAT_COUNT; ++loop)
				crc_sum += testCRC32ForPerforman_sse(str, loop);
		}
		print_elapsed_time(prev_time, crc_sum);
	}
}

// End of file
