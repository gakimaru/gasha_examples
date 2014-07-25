#pragma once
#ifndef GASHA_INCLUDED_EXAMPLE_FAST_STRING_H
#define GASHA_INCLUDED_EXAMPLE_FAST_STRING_H

//--------------------------------------------------------------------------------
// exmaple_fast_string.h
// 高速文字列操作テスト
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include <cstddef>//std::size_t

#ifdef GASHA_OPTIMIZED

static const int TEST_STRLEN_REPEAT_NUM = 10000;//strlenパフォーマンステストの実行回数
static const int TEST_STRNLEN_REPEAT_NUM = 10000;//strnlenパフォーマンステストの実行回数
static const int TEST_STRCMP_REPEAT_NUM = 10000;//strcmpパフォーマンステストの実行回数
static const int TEST_STRNCMP_REPEAT_NUM = 10000;//strncmpパフォーマンステストの実行回数
static const int TEST_STRCHR_REPEAT_NUM = 10000;//strchrパフォーマンステストの実行回数
static const int TEST_STRRCHR_REPEAT_NUM = 10000;//strrchrパフォーマンステストの実行回数
static const int TEST_STRSTR_REPEAT_NUM = 10000;//strstrパフォーマンステストの実行回数
static const int TEST_STRSTRBM_REPEAT_NUM = 10000;//strstrbmパフォーマンステストの実行回数
static const int TEST_STRSTR0_REPEAT_NUM = 10000;//strstr0パフォーマンステストの実行回数
static const int TEST_STRCPY_REPEAT_NUM = 10000;//strcpyパフォーマンステストの実行回数
static const int TEST_STRNCPY_REPEAT_NUM = 10000;//strncpyパフォーマンステストの実行回数

#else//GASHA_OPTIMIZED

static const int TEST_STRLEN_REPEAT_NUM = 100;//strlenパフォーマンステストの実行回数
static const int TEST_STRNLEN_REPEAT_NUM = 100;//strnlenパフォーマンステストの実行回数
static const int TEST_STRCMP_REPEAT_NUM = 100;//strcmpパフォーマンステストの実行回数
static const int TEST_STRNCMP_REPEAT_NUM = 100;//strncmpパフォーマンステストの実行回数
static const int TEST_STRCHR_REPEAT_NUM = 100;//strchrパフォーマンステストの実行回数
static const int TEST_STRRCHR_REPEAT_NUM = 100;//strrchrパフォーマンステストの実行回数
static const int TEST_STRSTR_REPEAT_NUM = 100;//strstrパフォーマンステストの実行回数
static const int TEST_STRSTRBM_REPEAT_NUM = 100;//strstrbmパフォーマンステストの実行回数
static const int TEST_STRSTR0_REPEAT_NUM = 100;//strstr0パフォーマンステストの実行回数
static const int TEST_STRCPY_REPEAT_NUM = 100;//strcpyパフォーマンステストの実行回数
static const int TEST_STRNCPY_REPEAT_NUM = 100;//strncpyパフォーマンステストの実行回数

#endif//GASHA_OPTIMIZED

//----------------------------------------
//高速文字列操作テスト
void example_fast_string();

//----------------------------------------
//テスト用データ初期化
void initStrSet();
//テスト用データ数取得
std::size_t getStrSetNum();
//テスト用データ取得
const char* getStr(char* buff, const std::size_t no, const int dummy);
//テスト用データ長取得
const std::size_t getStrLen(const std::size_t no, const int dummy);

#endif//GASHA_INCLUDED_EXAMPLE_FAST_STRING_H

// End of file
