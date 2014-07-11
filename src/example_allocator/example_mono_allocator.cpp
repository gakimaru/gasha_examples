//--------------------------------------------------------------------------------
// exmaple_mono_allocator.cpp
// 単一アロケータテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_allocator.h"//アロケータテスト

#include <gasha/mono_allocator.h>//単一アロケータ
#include <gasha/lf_mono_allocator.h>//ロックフリー単一アロケータ

#include <stdio.h>//printf()

GASHA_USING_NAMESPACE;//ネームスペース使用

//----------------------------------------
//基本テスト

//テスト用マクロ
#define EXPR_PLAIN(...) printf("%s\n", #__VA_ARGS__); __VA_ARGS__
#define EXPR_WITH_INFO(...) __VA_ARGS__ printf("> %s\t\tsize=%d, remain=%d, isAllocated=%s\n", #__VA_ARGS__, mono.size(), mono.remain(), mono.isAllocated() ? "true" : "false")
#define EXPR(p, ...) __VA_ARGS__ printf("> %s\t%s=%p, size=%d, remain=%d, isAllocated=%s\n", #__VA_ARGS__, #p, p, mono.size(), mono.remain(), mono.isAllocated() ? "true" : "false")

//単一アロケータのテスト（共通処理）
template<class ALLOCATOR>
static void testMono(ALLOCATOR& mono)
{
	printf("\n");
	char message[1024];
	EXPR_PLAIN(mono.debugInfo(message); printf(message););
	EXPR(p1, void* p1 = mono.alloc(1););
	EXPR(p1, mono.free(p1););
	EXPR(p2, void* p2 = mono.alloc(1, 1););
	EXPR(p3, void* p3 = mono.alloc(1, 1););//確保失敗（一つしか確保できない）
	EXPR(p2, mono.free(p2););
	EXPR(p3, mono.free(p3););
	EXPR(p4, void* p4 = mono.alloc(1););
	EXPR(p4, mono.free(p4););
	EXPR(p5, void* p5 = mono.alloc(10, 32););
	EXPR(p5, mono.free(p5););
	EXPR(p6, int* p6 = mono.template newObj<int>(););
	EXPR(p6, mono.deleteObj(p6););
	EXPR(p7, double* p7 = mono.template newArray<double>(3););
	EXPR(p7, mono.deleteArray(p7, 3););
	EXPR(p8, int* p8 = mono.template newObj<int>(););
	EXPR(p8, mono.deleteObj(p8););
	EXPR(p9, data_t* p9 = mono.template newObj<data_t>(););
	EXPR(p9, mono.deleteObj(p9););
	EXPR(p10, data_t* p10 = mono.template newObj<data_t>(123););
	EXPR(p10, mono.deleteObj(p10););
	EXPR(p11, data_t* p11 = mono.template newArray<data_t>(3););
	EXPR(p11, mono.deleteArray(p11, 3););
	EXPR(p12, data_t* p12 = mono.template newArray<data_t>(3, 456););
	EXPR(p12, mono.deleteArray(p12, 3););
	EXPR(p13, void* p13 = mono.alloc(2000););//アロケート失敗（サイズオーバー）
	EXPR(p13, mono.free(p13););
	EXPR(p14, void* p14 = mono.alloc(10););
	EXPR(p14, mono.free(p14););
	EXPR_PLAIN(mono.debugInfo(message); printf(message););
}

//----------------------------------------
//単一アロケータテスト
void example_mono_allocator()
{
	printf("\n");
	printf("================================================================================\n");

	alignas(16) char buff[1024];
	char message[1024];

	{
		printf("\n");
		printf("--------------------------------------------------------------------------------\n");
		printf("[ Test for monoAllocator ]\n");
		printf("--------------------------------------------------------------------------------\n");

		//単一アロケータ
		{
			printf("\n");
			printf("----------------------------------------\n");
			EXPR_PLAIN(monoAllocator<lock_type> mono(buff););
			printf("----------------------------------------\n");
			testMono(mono);
		}

		//バッファ付き単一アロケータ
		{
			printf("\n");
			printf("----------------------------------------\n");
			EXPR_PLAIN(monoAllocator_withBuff<1024, lock_type> mono;);
			printf("----------------------------------------\n");
			printf("\n");
			EXPR_PLAIN(mono.debugInfo(message); printf(message););
		}

		//型指定バッファ付き単一アロケータ
		//※型のアラインメントサイズ分余計に領域を割り当てる
		{
			printf("\n");
			printf("----------------------------------------\n");
			EXPR_PLAIN(monoAllocator_withType<long long, 128, lock_type> mono;);
			printf("----------------------------------------\n");
			printf("\n");
			EXPR_PLAIN(mono.debugInfo(message); printf(message););
			EXPR(p, long long* p = mono.newDefault(););
			EXPR(p, mono.deleteDefault(p););
		}
	}

	{
		printf("\n");
		printf("--------------------------------------------------------------------------------\n");
		printf("[ Test for lfMonoAllocator ]\n");
		printf("--------------------------------------------------------------------------------\n");

		//ロックフリー単一アロケータ
		{
			printf("\n");
			printf("----------------------------------------\n");
			EXPR_PLAIN(lfMonoAllocator mono(buff););
			printf("----------------------------------------\n");
			testMono(mono);
		}

		//バッファ付き単一スタックアロケータ
		{
			printf("\n");
			printf("----------------------------------------\n");
			EXPR_PLAIN(lfMonoAllocator_withBuff<1024> mono;);
			printf("----------------------------------------\n");
			printf("\n");
			EXPR_PLAIN(mono.debugInfo(message); printf(message););
		}

		//型指定バッファ付き単一スタックアロケータ
		//※型のアラインメントサイズ分余計に領域を割り当てる
		{
			printf("\n");
			printf("----------------------------------------\n");
			EXPR_PLAIN(lfMonoAllocator_withType<long long, 128> mono;);
			printf("----------------------------------------\n");
			printf("\n");
			EXPR_PLAIN(mono.debugInfo(message); printf(message););
			EXPR(p, long long* p = mono.newDefault(););
			EXPR(p, mono.deleteDefault(p););
		}
	}
}

// End of file
