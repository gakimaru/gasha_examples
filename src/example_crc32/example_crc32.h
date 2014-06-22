﻿#pragma once
#ifndef __EXAMPLE_CRC32_H_
#define __EXAMPLE_CRC32_H_

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

#ifdef _DEBUG//デバッグ版
static const int TEST_REPEAT_COUNT = 10000;//CRC32計算の繰り返しテスト回数

#else//_DEBUG//リリース版
static const int TEST_REPEAT_COUNT = 1000000;//CRC32計算の繰り返しテスト回数

#endif//_DEBUG

//--------------------------------------------------------------------------------

#include <gasha/crc32.h>//gasha::crc32

//CRC32計算処理テスト
void example_crc32();

//CRC32計算処理テスト（パフォーマンス計測用処理）
extern GASHA_ crc32_t performance_test_crc32_recursive(const char* str, const int dummy);//再帰処理版
extern GASHA_ crc32_t performance_test_crc32_loop(const char* str, const int dummy);//ループ処理版
extern GASHA_ crc32_t performance_test_crc32_table(const char* str, const int dummy);//事前計算済み多項式テーブル版
extern GASHA_ crc32_t performance_test_crc32_sse(const char* str, const int dummy);//SSE命令版

#endif//__EXAMPLE_CRC32_H_

// End of file
