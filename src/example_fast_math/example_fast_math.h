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

static const int TEST_OPERATION_REPEAT_NUM = 10000000;//四則演算パフォーマンステストの実行回数
static const int TEST_SQRT_REPEAT_NUM = 1000000;//平方根パフォーマンステストの実行回数
static const int TEST_VECTOR_REPEAT_NUM = 100000;//ベクトル演算パフォーマンステストの実行回数
static const int TEST_MATRIX_REPEAT_NUM = 100000;//行列演算パフォーマンステストの実行回数

#else//GASHA_OPTIMIZED

static const int TEST_OPERATION_REPEAT_NUM = 100;//四則演算パフォーマンステストの実行回数
static const int TEST_SQRT_REPEAT_NUM = 100;//平方根パフォーマンステストの実行回数
static const int TEST_VECTOR_REPEAT_NUM = 100;//ベクトル演算パフォーマンステストの実行回数
static const int TEST_MATRIX_REPEAT_NUM = 100;//行列演算パフォーマンステストの実行回数

#endif//GASHA_OPTIMIZED

//----------------------------------------
//高速除算テスト
void example_fast_math();

#endif//GASHA_INCLUDED_EXAMPLE_FAST_MATH_H

// End of file
