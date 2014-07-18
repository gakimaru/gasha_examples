//--------------------------------------------------------------------------------
// exmaple_std_allocator.cpp
// 標準アロケータテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_allocator.h"//アロケータテスト

#include <gasha/std_allocator.h>//標準アロケータ

#include <cstdio>//std::printf()

GASHA_USING_NAMESPACE;//ネームスペース使用

//----------------------------------------
//基本テスト

//テスト用マクロ
#define EXPR_PLAIN(...) std::printf("%s\n", #__VA_ARGS__); __VA_ARGS__
#define EXPR_WITH_INFO(...) __VA_ARGS__ std::printf("> %s\t\size=%d, remain=%d\n", #__VA_ARGS__, std_allocator.size(), std_allocator.remain())
#define EXPR(p, ...) __VA_ARGS__ std::printf("> %s\t%s=%p, size=%d, remain=%d\n", #__VA_ARGS__, #p, p, std_allocator.size(), std_allocator.remain())

//標準アロケータのテスト（共通処理）
template<class ALLOCATOR>
static void testStdAllocator(ALLOCATOR& std_allocator)
{
	std::printf("\n");
	char message[1024];
	EXPR_PLAIN(std_allocator.debugInfo(message); std::printf(message););
	EXPR(p1, void* p1 = std_allocator.alloc(1););
	EXPR(p2, void* p2 = std_allocator.alloc(1, 1););
	EXPR(p3, void* p3 = std_allocator.alloc(1, 1););
	EXPR(p4, void* p4 = std_allocator.alloc(1););
	EXPR(p5, void* p5 = std_allocator.alloc(10, 32););
	EXPR(p6, int* p6 = std_allocator.template newObj<int>(););
	EXPR(p7, double* p7 = std_allocator.template newArray<double>(3););
	EXPR(p8, int* p8 = std_allocator.template newObj<int>(););
	EXPR(p9, data_t* p9 = std_allocator.template newObj<data_t>(););//アラインメント対応版の時にはアラインメントが保証される
	EXPR(p10, int* p10 = std_allocator.template newObj<int>(););
	EXPR(p11, data_t* p11 = std_allocator.template newObj<data_t>(123););//アラインメント対応版の時にはアラインメントが保証される
	EXPR(p12, int* p12 = std_allocator.template newObj<int>(););
	EXPR(p13, data_t* p13 = std_allocator.template newArray<data_t>(3););//アラインメント対応版の時にはアラインメントが保証される
	EXPR(p14, data_t* p14 = std_allocator.template newArray<data_t>(3, 456););
	EXPR(p15, void* p15 = std_allocator.alloc(2000););
	EXPR(p16, void* p16 = std_allocator.alloc(10););
	EXPR(p17, data2_t* p17 = std_allocator.template newObj<data2_t>(););//アラインメント対応版の時にはアラインメントが保証される
	EXPR(p18, int* p18 = std_allocator.template newObj<int>(););
	EXPR(p19, data2_t* p19 = std_allocator.template newObj<data2_t>(););//アラインメント対応版の時にはアラインメントが保証される
	EXPR(p20, int* p20 = std_allocator.template newObj<int>(););
	EXPR(p21, data2_t* p21 = std_allocator.template newObj<data2_t>(););//アラインメント対応版の時にはアラインメントが保証される
	EXPR_PLAIN(std_allocator.debugInfo(message); std::printf(message););
	EXPR(p1, std_allocator.free(p1););
	EXPR(p2, std_allocator.free(p2););
	EXPR(p3, std_allocator.free(p3););
	EXPR(p4, std_allocator.free(p4););
	EXPR(p5, std_allocator.free(p5););
	EXPR(p6, std_allocator.deleteObj(p6););
	EXPR(p7, std_allocator.deleteArray(p7, 3););
	EXPR(p8, std_allocator.deleteObj(p8););
	EXPR(p9, std_allocator.deleteObj(p9););
	EXPR(p10, std_allocator.deleteObj(p10););
	EXPR(p11, std_allocator.deleteArray(p11, 3););
	EXPR(p12, std_allocator.deleteArray(p12, 3););
	EXPR(p13, std_allocator.free(p13););
	EXPR(p14, std_allocator.free(p14););
	EXPR(p15, std_allocator.free(p15););
	EXPR(p16, std_allocator.free(p16););
	EXPR(p17, std_allocator.free(p17););
	EXPR(p18, std_allocator.free(p18););
	EXPR(p19, std_allocator.free(p19););
	EXPR(p20, std_allocator.free(p20););
	EXPR(p21, std_allocator.free(p21););
	EXPR_PLAIN(std_allocator.debugInfo(message); std::printf(message););
}

//----------------------------------------
//標準アロケータテスト
void example_std_allocator()
{
	std::printf("\n");
	std::printf("================================================================================\n");

	{
		std::printf("\n");
		std::printf("--------------------------------------------------------------------------------\n");
		std::printf("[ Test for stdAllocator ]\n");
		std::printf("--------------------------------------------------------------------------------\n");

		//標準アロケータ
		{
			std::printf("\n");
			std::printf("----------------------------------------\n");
			EXPR_PLAIN(stdAllocator<lock_type> std_allocator;);
			std::printf("----------------------------------------\n");
			testStdAllocator(std_allocator);
		}

		//アライメント対応標準アロケータ
		{
			std::printf("\n");
			std::printf("----------------------------------------\n");
			EXPR_PLAIN(stdAlignAllocator<lock_type> std_allocator;);
			std::printf("----------------------------------------\n");
			testStdAllocator(std_allocator);
		}
	}
}

// End of file
