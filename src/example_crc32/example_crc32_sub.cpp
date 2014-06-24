//--------------------------------------------------------------------------------
// example_crc32_sub.cpp
// CRC32計算処理テスト（パフォーマンス計測用処理）
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_crc32.h"//CRC32計算処理テスト

#include <gasha/crc32.h>//CRC32計算

GASHA_USING_NAMESPACE;//ネームスペース使用

//再帰処理版の関数で計算
crc32_t testCRC32ForPerforman_recursive(const char* str, const int dummy)
{
	return calcCRC32_recursive(str);
}

//ループ処理版の関数で計算
crc32_t testCRC32ForPerforman_loop(const char* str, const int dummy)
{
	return calcCRC32_loop(str);
}

//事前計算済み多項式テーブル版の関数で計算
crc32_t testCRC32ForPerforman_table(const char* str, const int dummy)
{
#ifdef GASHA_CRC32_USE_STATIC_TABLE
	return calcCRC32_table(str);
#else//GASHA_CRC32_USE_STATIC_TABLE
	return 0;
#endif//GASHA_CRC32_USE_STATIC_TABLE
}

//SSE命令版の関数で計算
crc32_t testCRC32ForPerforman_sse(const char* str, const int dummy)
{
#ifdef GASHA_USE_SSE4_2
	return calcCRC32_sse(str);
#else//GASHA_USE_SSE4_2
	return 0;
#endif//GASHA_USE_SSE4_2
}

// End of file
