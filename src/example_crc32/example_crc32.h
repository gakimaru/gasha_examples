#pragma once
#ifndef GASHA_INCLUDED_EXAMPLE_CRC32_H
#define GASHA_INCLUDED_EXAMPLE_CRC32_H

//--------------------------------------------------------------------------------
// example_crc32.h
// CRC32計算処理テスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//CRC32計算処理テスト設定とコンパイラスイッチ

#ifdef GASHA_OPTIMIZED
static const int TEST_REPEAT_COUNT = 1000000;//CRC32計算の繰り返しテスト回数

#else//GASHA_OPTIMIZED
static const int TEST_REPEAT_COUNT = 10000;//CRC32計算の繰り返しテスト回数

#endif//GASHA_OPTIMIZED

//--------------------------------------------------------------------------------

#include <gasha/crc32.h>//CRC32計算

//CRC32計算処理テスト
void example_crc32();

//CRC32計算処理テスト（パフォーマンス計測用処理）
extern GASHA_ crc32_t testCRC32ForPerforman_recursive(const char* str, const int dummy);//再帰処理版
extern GASHA_ crc32_t testCRC32ForPerforman_loop(const char* str, const int dummy);//ループ処理版
extern GASHA_ crc32_t testCRC32ForPerforman_table(const char* str, const int dummy);//事前計算済み多項式テーブル版
extern GASHA_ crc32_t testCRC32ForPerforman_sse(const char* str, const int dummy);//SSE命令版

#endif//GASHA_INCLUDED_EXAMPLE_CRC32_H

// End of file
