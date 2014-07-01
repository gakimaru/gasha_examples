#pragma once
#ifndef GASHA_INCLUDED_EXAMPLE_EXAMPLE_BASIC_MATH_H
#define GASHA_INCLUDED_EXAMPLE_EXAMPLE_BASIC_MATH_H

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

#ifdef GASHA_OPTIMIZED
static const int TEST_BITCOUNT_MAX = 100000000;//ビット数計測テスト回数

#else//GASHA_OPTIMIZED
static const int TEST_BITCOUNT_MAX = 10000000;//ビット数計測テスト回数

#endif//GASHA_OPTIMIZED

//基本算術処理テスト
void example_basic_math();

#endif//GASHA_INCLUDED_EXAMPLE_EXAMPLE_BASIC_MATH_H

// End of file
