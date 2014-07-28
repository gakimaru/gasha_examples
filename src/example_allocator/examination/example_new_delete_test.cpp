//--------------------------------------------------------------------------------
// exmaple_new_delete_test.cpp
// new/delete演算子の挙動確認用テスト
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_allocator.h"//アロケータテスト

#include <cstddef>//std::size_t
#include <cstdio>//std::printf()

#pragma warning(push)//【VC++】ワーニング設定を退避
#pragma warning(disable: 4530)//【VC++】C4530を抑える
#include <new>//配置new/配置delete用
#pragma warning(pop)//【VC++】ワーニング設定を復元

GASHA_USING_NAMESPACE;//ネームスペース使用

//----------------------------------------
//調査用
struct forTest_t{};
const forTest_t forTest;
void* operator new(const std::size_t size, const forTest_t&, std::size_t& real_size) throw()
{
	real_size = size;
	return nullptr;
}
void* operator new[](const std::size_t size, const forTest_t&, std::size_t& real_size) throw()
{
	real_size = size;
	return nullptr;
}
void operator delete(void* p, const forTest_t&, std::size_t&) throw()
{
}
void operator delete[](void* p, const forTest_t&, std::size_t&) throw()
{
}

//----------------------------------------
//new/delete演算子の挙動確認用テスト
void examination_new_delete()
{
	std::printf("\n");
	std::printf("================================================================================\n");
	
	std::printf("\n");
	std::printf("--------------------------------------------------------------------------------\n");
	std::printf("[ Examination for new/delete operators ]\n");
	std::printf("--------------------------------------------------------------------------------\n");

	std::size_t real_size;

#define CHECK(T) \
	{ new(forTest, real_size)T; std::printf("new " #T ": sizeof(" #T ")=%d, alignof(" #T ")=%d, real_size=%d\n", static_cast<int>(sizeof(T)), static_cast<int>(alignof(T)), static_cast<int>(real_size)); } \
	{ new(forTest, real_size)T[2]; std::printf("new " #T "[2]: sizeof(" #T ")=%d, alignof(" #T ")=%d, real_size=%d\n", static_cast<int>(sizeof(T)), static_cast<int>(alignof(T)), static_cast<int>(real_size)); }

	struct st_n{ char m_var[1]; st_n(){} ~st_n(){} };
	CHECK(st_n);
	
	struct alignas(1) st_a1{ char m_var[1]; st_a1(){} ~st_a1(){} };
	CHECK(st_a1);
	
	struct alignas(2) st_a2{ char m_var[1]; st_a2(){} ~st_a2(){} };
	CHECK(st_a2);
	
	struct alignas(4) st_a4{ char m_var[1]; st_a4(){} ~st_a4(){} };
	CHECK(st_a4);
	
	struct alignas(8) st_a8{ char m_var[1]; st_a8(){} ~st_a8(){} };
	CHECK(st_a8);

#if GASHA_ALIGNAS_LIMIT >= 16
	struct alignas(16) st_a16{ char m_var[1]; st_a16(){} ~st_a16(){} };
	CHECK(st_a16);
#endif//GASHA_ALIGNAS_LIMIT

	//【注意】（この調査で判明すること）
	//　　　　VC++は32バイト以上のアライメントサイズだと扱いが難しくなる。
	//　　　　デストラクタのある型の配列new時に配列要素数格納領域が16で頭打ちとなるため、
	//　　　　アラインメントを揃えるには、16バイト分ずらす必要がある。
	//　　　　16バイト以下、およびGCCの場合は、配列要素数格納領域がアラインメントサイズになるため、
	//　　　　問題がない。
	//　　　　なお、本ライブラリでは、この特殊な状態に対応していない。
	//　　　　つまり、16バイトアラインメント以上のデストラクタ付きの型の配列newには対応していない。

#if GASHA_ALIGNAS_LIMIT >= 32
	struct alignas(32) st_a32{ char m_var[1]; st_a32(){} ~st_a32(){} };
	CHECK(st_a32);
#endif//GASHA_ALIGNAS_LIMIT

#if GASHA_ALIGNAS_LIMIT >= 64
	struct alignas(64) st_a64{ char m_var[1]; st_a64(){} ~st_a64(){} };
	CHECK(st_a64);
#endif//GASHA_ALIGNAS_LIMIT

#if GASHA_ALIGNAS_LIMIT >= 128
	struct alignas(128) st_a128{ char m_var[1]; st_a128(){} ~st_a128(){} };
	CHECK(st_a128);
#endif//GASHA_ALIGNAS_LIMIT

#if GASHA_ALIGNAS_LIMIT >= 256
	struct alignas(256) st_a256{ char m_var[1]; st_a256(){} ~st_a256(){} };//VC++不可
	CHECK(st_a256);
#endif//GASHA_ALIGNAS_LIMIT

#if GASHA_ALIGNAS_LIMIT >= 512
	struct alignas(512) st_a512{ char m_var[1]; st_a512(){} ~st_a512(){} };//GCC不可
	CHECK(st_a512);
#endif//GASHA_ALIGNAS_LIMIT

#if GASHA_ALIGNAS_LIMIT >= 1024
	struct alignas(1024) st_a1024{ char m_var[1]; st_a1024(){} ~st_a1024(){} };//GCC不可
	CHECK(st_a1024);
#endif//GASHA_ALIGNAS_LIMIT
}

// End of file
