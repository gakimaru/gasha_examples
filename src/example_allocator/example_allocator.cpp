//--------------------------------------------------------------------------------
// exmaple_allocator.cpp
// アロケータテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_allocator.h"//アロケータテスト

#include <cstdio>//printf()

//----------------------------------------
//テスト用構造体

//デフォルトコンストラクタ
data_t::data_t() :
	m_val(0)
{
	printf("data_t::data_t(): m_val=%d\n", m_val);
}

//コンストラクタ
data_t::data_t(const int val) :
	m_val(val)
{
	printf("data_t::data_t(%d): m_val=%d\n", val, m_val);
}

//ムーブコンストラクタ
data_t::data_t(data_t&& obj) :
	m_val(obj.m_val)
{
	printf("data_t::data_t(data_t&&): m_val=%d\n", m_val);
}

//コピーコンストラクタ
data_t::data_t(const data_t& obj) :
	m_val(obj.m_val)
{
	printf("data_t::data_t(const data_t&): m_val=%d\n", m_val);
}

//デストラクタ
data_t::~data_t()
{
	printf("data_t::~data_t(): m_val=%d\n", m_val);
}

//----------------------------------------
//アロケータテスト
void example_allocator()
{
#ifdef ENABLE_TEST_FOR_STACK_ALLOATOR
	example_stack_allocator();//スタックアロケータテスト
#endif//ENABLE_TEST_FOR_STACK_ALLOATOR

#ifdef ENABLE_TEST_FOR_DUAL_STACK_ALLOATOR
	example_dual_stack_allocator();//双方向スタックアロケータテスト
#endif//ENABLE_TEST_FOR_DUAL_STACK_ALLOATOR

#ifdef ENABLE_TEST_FOR_MONO_ALLOATOR
	example_mono_allocator();//単一アロケータテスト
#endif//ENABLE_TEST_FOR_MONO_ALLOATOR

#ifdef ENABLE_TEST_FOR_POOL_ALLOATOR
	example_pool_allocator();//プールアロケータテスト
#endif//ENABLE_TEST_FOR_POOL_ALLOATOR

#ifdef ENABLE_TEST_FOR_STD_ALLOATOR
	example_std_allocator();//標準アロケータテスト
#endif//ENABLE_TEST_FOR_STD_ALLOATOR

#ifdef ENABLE_TEST_FOR_POLY_ALLOATOR
	example_poly_allocator();//多態アロケータテスト
#endif//ENABLE_TEST_FOR_POLY_ALLOATOR

#ifdef ENABLE_TEST_FOR_ALLOATOR_PERFORMANCE
	example_allocator_performance();//アロケータパフォーマンステスト
#endif//ENABLE_TEST_FOR_ALLOATOR_PERFORMANCE

#ifdef ENABLE_TEST_FOR_NEW_DELETE
	example_new_delete_test();//new/delete演算子の挙動確認用テスト
#endif//ENABLE_TEST_FOR_NEW_DELETE

	printf("- end -\n");
}

// End of file
