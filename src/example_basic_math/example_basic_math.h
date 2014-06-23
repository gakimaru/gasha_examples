#pragma once
#ifndef __EXAMPLE_BASIC_MATH_H_
#define __EXAMPLE_BASIC_MATH_H_

//--------------------------------------------------------------------------------
// exmaple_basic_math.h
// 基本算術処理テスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//基本算術処理テスト設定とコンパイラスイッチ

#ifdef _DEBUG//デバッグ版
static const int TEST_BITCOUNT_MAX = 10000000;//ビット数計測テスト回数

#else//_DEBUG//リリース版
static const int TEST_BITCOUNT_MAX = 100000000;//ビット数計測テスト回数

#endif//_DEBUG

//基本算術処理テスト
void example_basic_math();

#endif//__EXAMPLE_BASIC_MATH_H_

// End of file
