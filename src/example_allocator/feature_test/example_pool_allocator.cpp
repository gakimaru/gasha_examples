//--------------------------------------------------------------------------------
// exmaple_pool_allocator.cpp
// プールアロケータテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_allocator.h"//アロケータテスト

#include <gasha/pool_allocator.h>//プールアロケータ
#include <gasha/lf_pool_allocator.h>//ロックフリープールアロケータ

#include <cstdio>//std::printf()

GASHA_USING_NAMESPACE;//ネームスペース使用

//----------------------------------------
//基本テスト

//テスト用マクロ
#define EXPR_PLAIN(...) std::printf("%s\n", #__VA_ARGS__); __VA_ARGS__
#define EXPR_WITH_INFO(...) __VA_ARGS__ std::printf("> %s\tsize=%d, remain=%d, pool=%d/%d\n", #__VA_ARGS__, pool.size(), pool.remain(), pool.usingPoolSize(), pool.poolSize())
#define EXPR(p, ...) __VA_ARGS__ std::printf("> %s\t%s=%p, size=%d, remain=%d, pool=%d/%d\n", #__VA_ARGS__, #p, p, pool.size(), pool.remain(), pool.usingPoolSize(), pool.poolSize())

//プールアロケータのテスト（共通処理）
template<class ALLOCATOR>
static void testPool(ALLOCATOR& pool)
{
	std::printf("\n");
	char message[1024];
	for (int i = 0; i < 3; ++i)//ブロックの再利用を確認するために3回ループ
	{
		EXPR_PLAIN(pool.debugInfo(message, sizeof(message)); std::printf("%s\n", message););
		EXPR(p1, void* p1 = pool.alloc(1););
		EXPR(p2, void* p2 = pool.alloc(1, 1););
		EXPR(p3, void* p3 = pool.alloc(1, 1););
		EXPR(p4, void* p4 = pool.alloc(1););
		EXPR(p5, void* p5 = pool.alloc(10, 32););//アロケート失敗（アライメントによるサイズオーバー）
		EXPR(p6, int* p6 = pool.template newObj<int>(););
		EXPR(p7, double* p7 = pool.template newArray<double>(3););
		EXPR(p8, int* p8 = pool.template newObj<int>(););
		EXPR(p9, data_t* p9 = pool.template newObj<data_t>(););
		EXPR(p10, data_t* p10 = pool.template newObj<data_t>(123););
		EXPR(p11, data_t* p11 = pool.template newArray<data_t>(3););//アロケート失敗（サイズオーバー）
		EXPR(p12, data_t* p12 = pool.template newArray<data_t>(3, 456););//アロケート失敗（サイズオーバー）
		EXPR(p13, void* p13 = pool.alloc(1000););//アロケート失敗（サイズオーバー）
		EXPR(p14, void* p14 = pool.alloc(10););
		EXPR(p14, void* p15 = pool.alloc(10););
		EXPR(p16, void* p16 = pool.alloc(10););
		EXPR(p17, void* p17 = pool.alloc(10););
		EXPR(p18, void* p18 = pool.alloc(10););
		EXPR(p19, void* p19 = pool.alloc(10););
		EXPR(p20, void* p20 = pool.alloc(10););
		EXPR(p21, void* p21 = pool.alloc(10););//アロケータ失敗（プール不足）
		EXPR_PLAIN(pool.debugInfo(message, sizeof(message)); std::printf("%s\n", message););
		EXPR(p1, pool.free(p1););
		EXPR(p2, pool.free(p2););
		EXPR(p3, pool.free(p3););
		EXPR(p4, pool.free(p4););
		EXPR(p5, pool.free(p5););
		EXPR(p6, pool.deleteObj(p6););
		EXPR(p7, pool.deleteArray(p7, 3););
		EXPR(p8, pool.deleteObj(p8););
		EXPR(p9, pool.deleteObj(p9););
		EXPR(p10, pool.deleteObj(p10););
		EXPR(p11, pool.deleteArray(p11, 3););
		EXPR(p12, pool.deleteArray(p12, 3););
		EXPR(p13, pool.free(p13););
		EXPR(p14, pool.free(p14););
		EXPR(p15, pool.free(p15););
		EXPR(p16, pool.free(p16););
		EXPR(p17, pool.free(p17););
		EXPR(p18, pool.free(p18););
		EXPR(p19, pool.free(p19););
		EXPR(p20, pool.free(p20););
		EXPR(p21, pool.free(p21););
	}
	EXPR_PLAIN(pool.debugInfo(message, sizeof(message)); std::printf("%s\n", message););
}

//----------------------------------------
//プールアロケータテスト
void example_pool_allocator()
{
	std::printf("\n");
	std::printf("================================================================================\n");

	char buff[1024];
	char message[1024];

	{
		std::printf("\n");
		std::printf("--------------------------------------------------------------------------------\n");
		std::printf("[ Test for poolAllocator ]\n");
		std::printf("--------------------------------------------------------------------------------\n");

		//プールアロケータ
		{
			std::printf("\n");
			std::printf("----------------------------------------\n");
			EXPR_PLAIN(poolAllocator<16, lock_type> pool(buff, sizeof(buff), 32, 8););//最大プール数=16, ブロックサイズ=32, ブロックアラインメント=8
			std::printf("----------------------------------------\n");
			testPool(pool);
		}

		//バッファ付きプールアロケータ
		{
			std::printf("\n");
			std::printf("----------------------------------------\n");
			EXPR_PLAIN(poolAllocator_withBuff<32, 16, 8, lock_type> pool;);
			std::printf("----------------------------------------\n");
			std::printf("\n");
			EXPR_PLAIN(pool.debugInfo(message, sizeof(message)); std::printf("%s\n", message););
		}

		//型指定バッファ付きプールアロケータ
		//※型のアラインメントサイズ分余計に領域を割り当てる
		{
			std::printf("\n");
			struct alignas(8) block_type
			{
				char buff[32];
			};
			std::printf("----------------------------------------\n");
			EXPR_PLAIN(poolAllocator_withType<block_type, 16, lock_type> pool;);
			std::printf("----------------------------------------\n");
			std::printf("\n");
			EXPR_PLAIN(pool.debugInfo(message, sizeof(message)); std::printf("%s\n", message););
			EXPR(p, block_type* p = pool.newDefault(););
			EXPR(p, pool.deleteDefault(p););
		}
	}

	{
		std::printf("\n");
		std::printf("--------------------------------------------------------------------------------\n");
		std::printf("[ Test for lfPoolAllocator ]\n");
		std::printf("--------------------------------------------------------------------------------\n");

		//ロックフリープールアロケータ
		{
			std::printf("\n");
			std::printf("----------------------------------------\n");
			EXPR_PLAIN(lfPoolAllocator<16> pool(buff, sizeof(buff), 32, 8););//最大プール数=16, ブロックサイズ=32, ブロックアラインメント=8
			std::printf("----------------------------------------\n");
			testPool(pool);
		}

		//バッファ付きロックフリープールアロケータ
		{
			std::printf("\n");
			std::printf("----------------------------------------\n");
			EXPR_PLAIN(lfPoolAllocator_withBuff<32, 16, 8> pool;);
			std::printf("----------------------------------------\n");
			std::printf("\n");
			EXPR_PLAIN(pool.debugInfo(message, sizeof(message)); std::printf("%s\n", message););
		}

		//型指定バッファ付きロックフリープールアロケータ
		//※型のアラインメントサイズ分余計に領域を割り当てる
		{
			std::printf("\n");
			struct alignas(8) block_type
			{
				char buff[32];
			};
			std::printf("----------------------------------------\n");
			EXPR_PLAIN(lfPoolAllocator_withType<block_type, 16> pool;);
			std::printf("----------------------------------------\n");
			std::printf("\n");
			EXPR_PLAIN(pool.debugInfo(message, sizeof(message)); std::printf("%s\n", message););
			EXPR(p, block_type* p = pool.newDefault(););
			EXPR(p, pool.deleteDefault(p););
		}
	}
}

// End of file
