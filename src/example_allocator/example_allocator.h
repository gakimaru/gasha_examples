#pragma once
#ifndef GASHA_INCLUDED_EXAMPLE_ALLOCATOR_H
#define GASHA_INCLUDED_EXAMPLE_ALLOCATOR_H

//--------------------------------------------------------------------------------
// exmaple_allocator.h
// アロケータテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

//----------------------------------------
//テスト用構造体
struct alignas(16) data_t
{
	int m_val;//値

	data_t();//デフォルトコンストラクタ
	data_t(const int val);//コンストラクタ
	data_t(data_t&& obj);//ムーブコンストラクタ
	data_t(const data_t& obj);//コピーコンストラクタ
	~data_t();//デストラクタ
};

//----------------------------------------
//アロケータテスト
void example_stack_allocator();//スタックアロケータテスト
void example_dual_stack_allocator();//双方向スタックアロケータテスト
void example_pool_allocator();//プールアロケータテスト
void example_mono_allocator();//単一アロケータテスト
void example_std_allocator();//標準アロケータテスト
void example_poly_allocator();//多態アロケータテスト
void example_allocator_performance();//アロケータパフォーマンステスト

#endif//GASHA_INCLUDED_EXAMPLE_ALLOCATOR_H

// End of file
