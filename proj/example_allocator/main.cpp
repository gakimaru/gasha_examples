//--------------------------------------------------------------------------------
// main.cpp
// アロケータテスト - メイン
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_allocator.h"//スタックアロケータテスト

#include <stdio.h>
#include <stdlib.h>

#if 0

#include <gasha/new.h>

struct st{ alignas(16) int a; st(int b) :a(b){ printf("constructor\n"); } st() :a(0){ printf("constructor\n"); } ~st(){ printf("destructor\n"); } };
auto at_new = [](const gasha::IAllocatorAdapter& adapter, const void* p, const std::size_t size, const std::size_t align, const gasha::newMethod_t method, const gasha::debugAllocationInfo* info)
{
#ifdef GASHA_HAS_DEBUG_FEATURE
	printf("new%s(%d,%d)=%p allocator=\"%s:%s\"\n", method == gasha::methodOfNewArrays ? "[]" : "", size, align, p, adapter.name(), adapter.mode());
	if (info)
		printf(" %s[%d], file=\"%s\", func=\"%s\", time=%lf\n", info->m_typeName, info->m_arrayNum, info->m_fileName, info->m_funcName, info->m_time);
#endif//GASHA_HAS_DEBUG_FEATURE
};
auto at_delete = [](const gasha::IAllocatorAdapter& adapter, const void* p, const gasha::deleteMethod_t method, const gasha::debugAllocationInfo* info)
{
#ifdef GASHA_HAS_DEBUG_FEATURE
	printf("delete%s(%p) allocator=\"%s:%s\"\n", method == gasha::methodOfDeleteArrays ? "[]" : "", p, adapter.name(), adapter.mode());
	if (info)
		printf(" %s%s, file=\"%s\", func=\"%s\", time=%lf\n", info->m_typeName, method == gasha::methodOfDeleteArrays ? "[]" : "", info->m_fileName, info->m_funcName, info->m_time);
#endif//GASHA_HAS_DEBUG_FEATURE
};
gasha::polyAllocator poly_allocator;
gasha::debugAllocationObserver observer;
observer.m_atNew = at_new;
observer.m_atDelete = at_delete;
poly_allocator.setDebugObserver(observer);

{
	gasha::stdAlignAllocator<> std_allocator;
	auto adapter = std_allocator.adapter();
	gasha::polyAllocator poly_allocator(adapter);
	poly_allocator.setDebugObserver(observer);

	st* i = GASHA_NEW(st, 99);
	st* i2 = GASHA_NEW(st[2]);
	st* i3 = GASHA_NEW_ARRAY(st, 3);
	GASHA_DELETE(i);
	GASHA_DELETE_ARRAY(i2);
	GASHA_DELETE_ARRAY(i3);
}

#endif

int main(const int argc, const char* argv[])
{
#ifdef ENABLE_TEST_FOR_STACK_ALLOATOR
	example_stack_allocator();//スタックアロケータテスト
#endif//ENABLE_TEST_FOR_STACK_ALLOATOR

#ifdef ENABLE_TEST_FOR_DUAL_STACK_ALLOATOR
	example_dual_stack_allocator();//双方向スタックアロケータテスト
#endif//ENABLE_TEST_FOR_DUAL_STACK_ALLOATOR

	example_mono_allocator();//単一アロケータテスト
	example_pool_allocator();//プールアロケータテスト
	example_std_allocator();//標準アロケータテスト
	example_poly_allocator();//多態アロケータテスト
	example_allocator_performance();//アロケータパフォーマンステスト

	return EXIT_SUCCESS;
}

// End of file
