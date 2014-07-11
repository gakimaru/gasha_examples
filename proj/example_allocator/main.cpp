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

int main(const int argc, const char* argv[])
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

	example_allocator_performance();//アロケータパフォーマンステスト

	return EXIT_SUCCESS;
}

// End of file
