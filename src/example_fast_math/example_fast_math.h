#pragma once
#ifndef GASHA_INCLUDED_EXAMPLE_FAST_MATH_H
#define GASHA_INCLUDED_EXAMPLE_FAST_MATH_H

//--------------------------------------------------------------------------------
// exmaple_fast_math.h
// 高速算術テスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//赤黒木テスト用設定とコンパイラスイッチ

#ifdef GASHA_OPTIMIZED

static const int TEST_REPEAT_NUM = 10000000;//パフォーマンステストの実行回数

#else//GASHA_OPTIMIZED

static const int TEST_REPEAT_NUM = 1000;//パフォーマンステストの実行回数

#endif//GASHA_OPTIMIZED

//----------------------------------------
//高速除算テスト
void example_fast_math();

#endif//GASHA_INCLUDED_EXAMPLE_FAST_MATH_H

// End of file
