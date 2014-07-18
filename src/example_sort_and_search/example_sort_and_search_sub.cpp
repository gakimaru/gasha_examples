//--------------------------------------------------------------------------------
// exmaple_sort_and_search_sub.cpp
// ソート＆探索処理テスト（補助処理）
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_sort_and_search.h"//ソート＆探索処理テスト

#include <gasha/memory.h>//メモリ操作：_aligned_malloc(), _aligned_free()

//----------------------------------------
//テスト用構造体

#ifdef TEST_DATA_WATCH_CONSTRUCTOR
#include <cstdio>//std::printf()
#include <cstring>//std::memcpy()
#endif//TEST_DATA_WATCH_CONSTRUCTOR
#ifdef TEST_DATA_WATCH_CONSTRUCTOR
data_t& data_t::operator=(data_t&& rhs)
{
	std::memcpy(this, &rhs, sizeof(*this));
	std::printf("data_t::move_assignment_operator\n");
	return *this;
}
data_t& data_t::operator=(const data_t& rhs)
{
	std::memcpy(this, &rhs, sizeof(*this));
	std::printf("data_t::copy_assignment_operator\n");
	return *this;
}
data_t::data_t(data_t&& src)
{
	std::memcpy(this, &src, sizeof(*this));
	std::printf("data_t::move_constructor\n");
}
data_t::data_t(const data_t& src)
{
	std::memcpy(this, &src, sizeof(*this));
	std::printf("data_t::copy_constructor\n");
}
data_t::data_t()
{
	std::printf("data_t::constructor\n");
}
data_t::~data_t()
{
	std::printf("data_t::destructor\n");
}
#endif//TEST_DATA_WATCH_CONSTRUCTOR

//----------------------------------------
//テスト用構造体の配列型

void*  array_t::operator new(const size_t size)
{
	return _aligned_malloc(sizeof(array_t), alignof(array_t));
}
void  array_t::operator delete (void* p)
{
	_aligned_free(p);
}

//----------------------------------------
//テスト用の比較処理

//※通常関数
bool predicate_func(const data_t& lhs, const data_t& rhs)
{
	return lhs.m_key < rhs.m_key;
}

// End of file
