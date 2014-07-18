//--------------------------------------------------------------------------------
// exmaple_dual_stack_allocator.cpp
// 双方向スタックアロケータテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_allocator.h"//アロケータテスト

#include <gasha/dual_stack_allocator.h>//双方向スタックアロケータ
#include <gasha/lf_dual_stack_allocator.h>//ロックフリー双方向スタックアロケータ
#include <gasha/scoped_stack_allocator.h>//スコープスタックアロケータ
#include <gasha/scoped_dual_stack_allocator.h>//スコープ双方向スタックアロケータ

#include <cstdio>//std::printf()

GASHA_USING_NAMESPACE;//ネームスペース使用

//----------------------------------------
//基本テスト

//テスト用マクロ
#define EXPR_PLAIN(...) std::printf("> %s\n", #__VA_ARGS__); __VA_ARGS__
#define EXPR_WITH_INFO(...) __VA_ARGS__ std::printf("> %s\ttsize=%d(ASC=%d,DESC=%d) remain=%d, count=%d(ASC=%d,DESC=%d)\n", #__VA_ARGS__, stack.size(), stack.sizeAsc(), stack.sizeDesc(), stack.remain(), stack.count(), stack.countAsc(), stack.countDesc())
#define EXPR(p, ...) __VA_ARGS__ std::printf("> %s\t%s=%p, size=%d(ASC=%d,DESC=%d), remain=%d, count=%d(ASC=%d,DESC=%d)\n", #__VA_ARGS__, #p, p, stack.size(), stack.sizeAsc(), stack.sizeDesc(), stack.remain(), stack.count(), stack.countAsc(), stack.countDesc())
#define EXPR_SCOPED_PLAIN(...) std::printf(">\t %s\n", #__VA_ARGS__); __VA_ARGS__
#define EXPR_SCOPED_WITH_INFO(...) __VA_ARGS__ std::printf(">\t %s\tsize=%d, remain=%d, count=%d\n", #__VA_ARGS__, scoped_stack.size(), scoped_stack.remain(), scoped_stack.count())
#define EXPR_SCOPED(p, ...) __VA_ARGS__ std::printf(">\t %s\t%s=%p, size=%d, remain=%d, count=%d\n", #__VA_ARGS__, #p, p, scoped_stack.size(), scoped_stack.remain(), scoped_stack.count())
#define EXPR_SCOPED_DUAL_PLAIN(...) std::printf(">\t %s\n", #__VA_ARGS__); __VA_ARGS__
#define EXPR_SCOPED_DUAL_WITH_INFO(...) __VA_ARGS__ std::printf(">\t %s\tsize=%d(ASC=%d,DESC=%d), remain=%d, count=%d(ASC=%d,DESC=%d)\n", #__VA_ARGS__, scoped_stack.size(), scoped_stack.sizeAsc(), scoped_stack.sizeDesc(), scoped_stack.remain(), scoped_stack.count(), scoped_stack.countAsc(), scoped_stack.countDesc())
#define EXPR_SCOPED_DUAL(p, ...) __VA_ARGS__ std::printf(">\t %s\t%s=%p, size=%d(ASC=%d,DESC=%d), remain=%d, count=%d(ASC=%d,DESC=%d)\n", #__VA_ARGS__, #p, p, scoped_stack.size(), scoped_stack.sizeAsc(), scoped_stack.sizeDesc(), scoped_stack.remain(), scoped_stack.count(), scoped_stack.countAsc(), scoped_stack.countDesc())

//双方向スタックアロケータのテスト（共通処理）
template<class ALLOCATOR>
static void testDualStack(ALLOCATOR& stack)
{
	std::printf("\n");
	char message[1024];
	for (int i = 0; i < 4; ++i)
	{
		std::printf("--------------------------------------------------\n");
		if (i == 0)
		{
			EXPR_PLAIN(stack.setAllocateOrder(ALLOC_ASC));
		}
		else if (i == 1)
		{
			EXPR_PLAIN(stack.setAllocateOrder(ALLOC_DESC));
		}
		else
		{
			EXPR_PLAIN(stack.reversewAllocateOrder());
		}
		EXPR_PLAIN(stack.debugInfo(message, sizeof(message)); std::printf(message););
		EXPR(p1, void* p1 = stack.alloc(1););
		EXPR(p2, void* p2 = stack.alloc(1, 1););
		EXPR(p3, void* p3 = stack.alloc(1, 1););
		EXPR(p4, void* p4 = stack.alloc(1););
		EXPR(p5, void* p5 = stack.alloc(10, 32););
		EXPR(p6, int* p6 = stack.template newObj<int>(););
		EXPR(p7, double* p7 = stack.template newArray<double>(3););
		EXPR(p8, int* p8 = stack.template newObj<int>(););
		EXPR(p9, data_t* p9 = stack.template newObj<data_t>(););
		EXPR(p10, data_t* p10 = stack.template newObj<data_t>(123););
		EXPR(p11, data_t* p11 = stack.template newArray<data_t>(3););
		EXPR(p12, data_t* p12 = stack.template newArray<data_t>(3, 456););
		EXPR(p13, void* p13 = stack.alloc(1000););//アロケート失敗（サイズオーバー）
		EXPR(p14, void* p14 = stack.alloc(10););
		EXPR_PLAIN(stack.debugInfo(message, sizeof(message)); std::printf(message););
		EXPR(p1, stack.free(p1););
		EXPR(p2, stack.free(p2););
		EXPR(p3, stack.free(p3););
		EXPR(p4, stack.free(p4););
		EXPR(p5, stack.free(p5););
		EXPR(p6, stack.deleteObj(p6););
		EXPR(p7, stack.deleteArray(p7, 3););
		EXPR(p8, stack.deleteObj(p8););
		EXPR(p9, stack.deleteObj(p9););
		EXPR(p10, stack.deleteObj(p10););
		EXPR(p11, stack.deleteArray(p11, 3););
		EXPR(p12, stack.deleteArray(p12, 3););
		EXPR(p13, stack.free(p13););
		EXPR(p14, stack.free(p14););//スマートスタックならここで自動クリア
		EXPR_PLAIN(stack.debugInfo(message, sizeof(message)); std::printf(message););
		EXPR_WITH_INFO(stack.clearAll(););
		EXPR(p100, void* p100 = stack.alloc(10););
		EXPR(p101, void* p101 = stack.alloc(20););
		EXPR(p102, void* p102 = stack.alloc(30););
		EXPR_WITH_INFO(stack.rewind(100););//誤ったリワインド（影響なし）
		EXPR_WITH_INFO(stack.rewind(20););
		EXPR_WITH_INFO(stack.rewind(p101););
		EXPR_WITH_INFO(stack.rewind(p100););
		EXPR_WITH_INFO(stack.clearAll(););
		EXPR_PLAIN(stack.debugInfo(message, sizeof(message)); std::printf(message););
		EXPR(p200, void* p200 = stack.allocOrd(ALLOC_ASC, 10););
		EXPR(p201, void* p201 = stack.allocOrd(ALLOC_DESC, 10););
		EXPR(p202, int* p202 = stack.template newObjOrd<int>(ALLOC_ASC););
		EXPR(p203, int* p203 = stack.template newObjOrd<int>(ALLOC_DESC););
		EXPR(p204, double* p204 = stack.template newArrayOrd<double>(ALLOC_ASC, 3););
		EXPR(p205, double* p205 = stack.template newArrayOrd<double>(ALLOC_DESC, 3););
		EXPR_WITH_INFO(stack.rewindOrd(ALLOC_ASC, 100););//誤ったリワインド（影響なし）
		EXPR_WITH_INFO(stack.rewindOrd(ALLOC_DESC, 100););//誤ったリワインド（影響なし）
		EXPR_WITH_INFO(stack.rewindOrd(ALLOC_ASC, 15););
		EXPR_WITH_INFO(stack.rewindOrd(ALLOC_DESC, 15););
		EXPR_WITH_INFO(stack.rewind(p202););
		EXPR_WITH_INFO(stack.rewind(p201););
		EXPR_WITH_INFO(stack.clearOrd(ALLOC_ASC););
		EXPR_WITH_INFO(stack.clearOrd(ALLOC_DESC););
		EXPR(p300, void* p300 = stack.allocOrd(ALLOC_ASC, 10););
		EXPR(p301, void* p301 = stack.allocOrd(ALLOC_DESC, 10););
		EXPR_WITH_INFO(stack.clear(););
		EXPR_PLAIN(stack.debugInfo(message, sizeof(message)); std::printf(message););
	}
}

//スコープスタックアロケータのテスト（共通処理）
//※双方向スタックアロケータを（片方向の）スコープアロケータとして使用する
template<class ALLOCATOR>
static void testScopedStack(ALLOCATOR& stack)
{
	std::printf("\n");
	char message[1024];
	EXPR_PLAIN(stack.debugInfo(message, sizeof(message)); std::printf(message););
	for (int i = 0; i < 2; ++i)
	{
		std::printf("--------------------------------------------------\n");
		void* p = nullptr;
		if (i == 0)
		{
			EXPR(p, p = stack.alloc(100););
		}
		{
			std::printf("***** Begin : Scoped stack allocator *****\n");
			EXPR_SCOPED_PLAIN(auto scoped_stack = stack.scopedAllocator(););//スコープスタックアロケータを取得（元になるスタックアロケータを使用してメモリ操作し、スコープを抜ける時に元の状態に戻す）
			EXPR_SCOPED_PLAIN(scoped_stack.debugInfo(message, sizeof(message)); std::printf(message););
			EXPR_SCOPED(p1, void* p1 = scoped_stack.alloc(1););
			EXPR_SCOPED(p2, void* p2 = scoped_stack.alloc(1, 1););
			EXPR_SCOPED(p3, void* p3 = scoped_stack.alloc(1, 1););
			EXPR_SCOPED(p4, void* p4 = scoped_stack.alloc(1););
			EXPR_SCOPED(p5, void* p5 = scoped_stack.alloc(10, 32););
			EXPR_SCOPED(p6, int* p6 = scoped_stack.template newObj<int>(););
			EXPR_SCOPED(p7, double* p7 = scoped_stack.template newArray<double>(3););
			EXPR_SCOPED(p8, int* p8 = scoped_stack.template newObj<int>(););
			EXPR_SCOPED(p9, data_t* p9 = scoped_stack.template newObj<data_t>(););
			EXPR_SCOPED(p10, data_t* p10 = scoped_stack.template newObj<data_t>(123););
			EXPR_SCOPED(p11, data_t* p11 = scoped_stack.template newArray<data_t>(3););
			EXPR_SCOPED(p12, data_t* p12 = scoped_stack.template newArray<data_t>(3, 456););
			EXPR_SCOPED(p13, void* p13 = scoped_stack.alloc(1000););//アロケート失敗（サイズオーバー）
			EXPR_SCOPED(p14, void* p14 = scoped_stack.alloc(10););
			EXPR_SCOPED_PLAIN(scoped_stack.debugInfo(message, sizeof(message)); std::printf(message););
			EXPR_SCOPED(p1, scoped_stack.free(p1););
			EXPR_SCOPED(p2, scoped_stack.free(p2););
			EXPR_SCOPED(p3, scoped_stack.free(p3););
			EXPR_SCOPED(p4, scoped_stack.free(p4););
			EXPR_SCOPED(p5, scoped_stack.free(p5););
			EXPR_SCOPED(p6, scoped_stack.deleteObj(p6););
			EXPR_SCOPED(p7, scoped_stack.deleteArray(p7, 3););
			EXPR_SCOPED(p8, scoped_stack.deleteObj(p8););
			EXPR_SCOPED(p9, scoped_stack.deleteObj(p9););
			EXPR_SCOPED(p10, scoped_stack.deleteObj(p10););
			EXPR_SCOPED(p11, scoped_stack.deleteArray(p11, 3););
			EXPR_SCOPED(p12, scoped_stack.deleteArray(p12, 3););
			EXPR_SCOPED(p13, scoped_stack.free(p13););
			EXPR_SCOPED(p14, scoped_stack.free(p14););//スマートスタックでも自動クリアはしない
			EXPR_SCOPED_PLAIN(scoped_stack.debugInfo(message, sizeof(message)); std::printf(message););
			EXPR_SCOPED_WITH_INFO(scoped_stack.clear(););
			EXPR_SCOPED(p100, void* p100 = scoped_stack.alloc(10););
			EXPR_SCOPED(p101, void* p101 = scoped_stack.alloc(20););
			EXPR_SCOPED(p102, void* p102 = scoped_stack.alloc(30););
			EXPR_SCOPED_WITH_INFO(scoped_stack.rewind(100););//誤ったリワインド（影響なし）
			EXPR_SCOPED_WITH_INFO(scoped_stack.rewind(20););
			EXPR_SCOPED_WITH_INFO(scoped_stack.rewind(p101););
			EXPR_SCOPED_WITH_INFO(scoped_stack.rewind(p100););
			EXPR_SCOPED_WITH_INFO(scoped_stack.clear(););
			EXPR_SCOPED_PLAIN(scoped_stack.debugInfo(message, sizeof(message)); std::printf(message););
			EXPR_SCOPED(p200, void* p200 = scoped_stack.alloc(30););
			EXPR_SCOPED(p201, void* p201 = scoped_stack.alloc(40););
			EXPR_SCOPED_PLAIN(scoped_stack.debugInfo(message, sizeof(message)); std::printf(message););
			std::printf("***** End : Scoped stack allocator *****\n");
		}
		EXPR_PLAIN(stack.debugInfo(message, sizeof(message)); std::printf(message););
		EXPR_PLAIN(stack.clear(););
		EXPR_PLAIN(stack.debugInfo(message, sizeof(message)); std::printf(message););
	}
}

//双方向スコープスタックアロケータのテスト（共通処理）
template<class ALLOCATOR>
static void testScopedDualStack(ALLOCATOR& stack)
{
	std::printf("\n");
	std::printf("================================================================================\n");

	std::printf("\n");
	char message[1024];
	EXPR_PLAIN(stack.debugInfo(message, sizeof(message)); std::printf(message););
	for (int i = 0; i < 2; ++i)
	{
		std::printf("--------------------------------------------------\n");
		void* p1 = nullptr;
		void* p2 = nullptr;
		if (i == 0)
		{
			EXPR(p1, p1 = stack.allocOrd(ALLOC_ASC, 100););
			EXPR(p2, p2 = stack.allocOrd(ALLOC_DESC, 100););
		}
		for (int j = 0; j < 4; ++j)
		{
			{
				std::printf("***** Begin : Dual Scoped stack allocator *****\n");
				EXPR_SCOPED_DUAL_PLAIN(auto scoped_stack = stack.scopedDualAllocator(););//スコープスタックアロケータを取得（元になるスタックアロケータを使用してメモリ操作し、スコープを抜ける時に元の状態に戻す）
				if (j == 0)
				{
					EXPR_SCOPED_DUAL_PLAIN(scoped_stack.setAllocateOrder(ALLOC_ASC));
				}
				else if (j == 1)
				{
					EXPR_SCOPED_DUAL_PLAIN(scoped_stack.setAllocateOrder(ALLOC_DESC));
				}
				else
				{
					EXPR_SCOPED_DUAL_PLAIN(scoped_stack.reversewAllocateOrder());
				}
				EXPR_SCOPED_DUAL_PLAIN(scoped_stack.debugInfo(message, sizeof(message)); std::printf(message););
				EXPR_SCOPED_DUAL(p1, void* p1 = scoped_stack.alloc(1););
				EXPR_SCOPED_DUAL(p2, void* p2 = scoped_stack.alloc(1, 1););
				EXPR_SCOPED_DUAL(p3, void* p3 = scoped_stack.alloc(1, 1););
				EXPR_SCOPED_DUAL(p4, void* p4 = scoped_stack.alloc(1););
				EXPR_SCOPED_DUAL(p5, void* p5 = scoped_stack.alloc(10, 32););
				EXPR_SCOPED_DUAL(p6, int* p6 = scoped_stack.template newObj<int>(););
				EXPR_SCOPED_DUAL(p7, double* p7 = scoped_stack.template newArray<double>(3););
				EXPR_SCOPED_DUAL(p8, int* p8 = scoped_stack.template newObj<int>(););
				EXPR_SCOPED_DUAL(p9, data_t* p9 = scoped_stack.template newObj<data_t>(););
				EXPR_SCOPED_DUAL(p10, data_t* p10 = scoped_stack.template newObj<data_t>(123););
				EXPR_SCOPED_DUAL(p11, data_t* p11 = scoped_stack.template newArray<data_t>(3););
				EXPR_SCOPED_DUAL(p12, data_t* p12 = scoped_stack.template newArray<data_t>(3, 456););
				EXPR_SCOPED_DUAL(p13, void* p13 = scoped_stack.alloc(1000););//アロケート失敗（サイズオーバー）
				EXPR_SCOPED_DUAL(p14, void* p14 = scoped_stack.alloc(10););
				EXPR_SCOPED_DUAL_PLAIN(scoped_stack.debugInfo(message, sizeof(message)); std::printf(message););
				EXPR_SCOPED_DUAL(p1, scoped_stack.free(p1););
				EXPR_SCOPED_DUAL(p2, scoped_stack.free(p2););
				EXPR_SCOPED_DUAL(p3, scoped_stack.free(p3););
				EXPR_SCOPED_DUAL(p4, scoped_stack.free(p4););
				EXPR_SCOPED_DUAL(p5, scoped_stack.free(p5););
				EXPR_SCOPED_DUAL(p6, scoped_stack.deleteObj(p6););
				EXPR_SCOPED_DUAL(p7, scoped_stack.deleteArray(p7, 3););
				EXPR_SCOPED_DUAL(p8, scoped_stack.deleteObj(p8););
				EXPR_SCOPED_DUAL(p9, scoped_stack.deleteObj(p9););
				EXPR_SCOPED_DUAL(p10, scoped_stack.deleteObj(p10););
				EXPR_SCOPED_DUAL(p11, scoped_stack.deleteArray(p11, 3););
				EXPR_SCOPED_DUAL(p12, scoped_stack.deleteArray(p12, 3););
				EXPR_SCOPED_DUAL(p13, scoped_stack.free(p13););
				EXPR_SCOPED_DUAL(p14, scoped_stack.free(p14););//スマートスタックでも自動クリアはしない
				EXPR_SCOPED_DUAL_PLAIN(scoped_stack.debugInfo(message, sizeof(message)); std::printf(message););
				EXPR_SCOPED_DUAL_WITH_INFO(scoped_stack.clearAll(););
				EXPR_SCOPED_DUAL(p100, void* p100 = scoped_stack.alloc(10););
				EXPR_SCOPED_DUAL(p101, void* p101 = scoped_stack.alloc(20););
				EXPR_SCOPED_DUAL(p102, void* p102 = scoped_stack.alloc(30););
				EXPR_SCOPED_DUAL_WITH_INFO(scoped_stack.rewind(100););//誤ったリワインド（影響なし）
				EXPR_SCOPED_DUAL_WITH_INFO(scoped_stack.rewind(20););
				EXPR_SCOPED_DUAL_WITH_INFO(scoped_stack.rewind(p101););
				EXPR_SCOPED_DUAL_WITH_INFO(scoped_stack.rewind(p100););
				EXPR_SCOPED_DUAL_WITH_INFO(scoped_stack.clearAll(););
				EXPR_SCOPED_DUAL_PLAIN(scoped_stack.debugInfo(message, sizeof(message)); std::printf(message););
				EXPR_SCOPED_DUAL(p200, void* p200 = scoped_stack.allocOrd(ALLOC_ASC, 10););
				EXPR_SCOPED_DUAL(p201, void* p201 = scoped_stack.allocOrd(ALLOC_DESC, 10););
				EXPR_SCOPED_DUAL(p202, int* p202 = scoped_stack.template newObjOrd<int>(ALLOC_ASC););
				EXPR_SCOPED_DUAL(p203, int* p203 = scoped_stack.template newObjOrd<int>(ALLOC_DESC););
				EXPR_SCOPED_DUAL(p204, double* p204 = scoped_stack.template newArrayOrd<double>(ALLOC_ASC, 3););
				EXPR_SCOPED_DUAL(p205, double* p205 = scoped_stack.template newArrayOrd<double>(ALLOC_DESC, 3););
				EXPR_SCOPED_DUAL_WITH_INFO(scoped_stack.rewindOrd(ALLOC_ASC, 100););//誤ったリワインド（影響なし）
				EXPR_SCOPED_DUAL_WITH_INFO(scoped_stack.rewindOrd(ALLOC_DESC, 100););//誤ったリワインド（影響なし）
				EXPR_SCOPED_DUAL_WITH_INFO(scoped_stack.rewindOrd(ALLOC_ASC, 15););
				EXPR_SCOPED_DUAL_WITH_INFO(scoped_stack.rewindOrd(ALLOC_DESC, 15););
				EXPR_SCOPED_DUAL_WITH_INFO(scoped_stack.rewind(p202););
				EXPR_SCOPED_DUAL_WITH_INFO(scoped_stack.rewind(p201););
				EXPR_SCOPED_DUAL_WITH_INFO(scoped_stack.clearOrd(ALLOC_ASC););
				EXPR_SCOPED_DUAL_WITH_INFO(scoped_stack.clearOrd(ALLOC_DESC););
				EXPR_SCOPED_DUAL_PLAIN(scoped_stack.debugInfo(message, sizeof(message)); std::printf(message););
				EXPR_SCOPED_DUAL(p300, void* p300 = scoped_stack.allocOrd(ALLOC_ASC, 10););
				EXPR_SCOPED_DUAL(p301, void* p301 = scoped_stack.allocOrd(ALLOC_DESC, 10););
				EXPR_SCOPED_DUAL_WITH_INFO(scoped_stack.clear(););
				EXPR_SCOPED_DUAL(p302, void* p302 = scoped_stack.allocOrd(ALLOC_ASC, 10););
				EXPR_SCOPED_DUAL(p303, void* p303 = scoped_stack.allocOrd(ALLOC_DESC, 10););
				EXPR_SCOPED_DUAL_PLAIN(scoped_stack.debugInfo(message, sizeof(message)); std::printf(message););
				std::printf("***** End : Scoped stack allocator *****\n");
			}
			EXPR_PLAIN(stack.debugInfo(message, sizeof(message)); std::printf(message););
			EXPR_PLAIN(stack.clearAll(););
			EXPR_PLAIN(stack.debugInfo(message, sizeof(message)); std::printf(message););
		}
	}
}

//----------------------------------------
//双方向スタックアロケータテスト
void example_dual_stack_allocator()
{
	alignas(16) char buff[1024];
	char message[1024];

	{
		std::printf("\n");
		std::printf("--------------------------------------------------------------------------------\n");
		std::printf("[ Test for dualStackAllocator ]\n");
		std::printf("--------------------------------------------------------------------------------\n");

		//双方向スタックアロケータ
		{
			std::printf("\n");
			std::printf("----------------------------------------\n");
			EXPR_PLAIN(dualStackAllocator<lock_type> stack(buff););
			std::printf("----------------------------------------\n");
			testDualStack(stack);
		}

		//スマート双方向スタックアロケータ
		//※正順か逆順のメモリ確保数が0になったら、対象方向のメモリを自動リセット
		{
			std::printf("\n");
			std::printf("----------------------------------------\n");
			EXPR_PLAIN(smartDualStackAllocator<lock_type> stack(buff););
			std::printf("----------------------------------------\n");
			testDualStack(stack);
		}

		//バッファ付き双方向スタックアロケータ
		{
			std::printf("\n");
			std::printf("----------------------------------------\n");
			EXPR_PLAIN(dualStackAllocator_withBuff<1024, lock_type> stack;);
			std::printf("----------------------------------------\n");
			std::printf("\n");
			EXPR_PLAIN(stack.debugInfo(message, sizeof(message)); std::printf(message););
		}

		//バッファ付きスマート双方向スタックアロケータ
		{
			std::printf("\n");
			std::printf("----------------------------------------\n");
			EXPR_PLAIN(smartDualStackAllocator_withBuff<1024, lock_type> stack;);
			std::printf("----------------------------------------\n");
			std::printf("\n");
			EXPR_PLAIN(stack.debugInfo(message, sizeof(message)); std::printf(message););
		}

		//型指定バッファ付き双方向スタックアロケータ
		//※型のアラインメントサイズ分余計に領域を割り当てる
		{
			std::printf("\n");
			std::printf("----------------------------------------\n");
			EXPR_PLAIN(dualStackAllocator_withType<long long, 128, lock_type> stack;);
			std::printf("----------------------------------------\n");
			std::printf("\n");
			EXPR_PLAIN(stack.debugInfo(message, sizeof(message)); std::printf(message););
			EXPR(p1, long long* p1 = stack.newDefault(););
			EXPR_PLAIN(stack.reversewAllocateOrder(););
			EXPR(p1, long long* p2 = stack.newDefault(););
			EXPR(p2, long long* p3 = stack.newDefaultOrd(ALLOC_ASC););
			EXPR(p3, long long* p4 = stack.newDefaultOrd(ALLOC_DESC););
			EXPR(p1, stack.deleteDefault(p1););
			EXPR(p2, stack.deleteDefault(p2););
			EXPR(p3, stack.deleteDefault(p3););
			EXPR(p4, stack.deleteDefault(p4););
		}

		//型指定バッファ付きスマート双方向スタックアロケータ
		//※型のアラインメントサイズ分余計に領域を割り当てる
		{
			std::printf("\n");
			std::printf("----------------------------------------\n");
			EXPR_PLAIN(smartDualStackAllocator_withType<long long, 128, lock_type> stack;);
			std::printf("----------------------------------------\n");
			std::printf("\n");
			EXPR_PLAIN(stack.debugInfo(message, sizeof(message)); std::printf(message););
			EXPR(p1, long long* p1 = stack.newDefault(););
			EXPR_PLAIN(stack.reversewAllocateOrder(););
			EXPR(p1, long long* p2 = stack.newDefault(););
			EXPR(p2, long long* p3 = stack.newDefaultOrd(ALLOC_ASC););
			EXPR(p3, long long* p4 = stack.newDefaultOrd(ALLOC_DESC););
			EXPR(p1, stack.deleteDefault(p1););
			EXPR(p2, stack.deleteDefault(p2););
			EXPR(p3, stack.deleteDefault(p3););
			EXPR(p4, stack.deleteDefault(p4););
		}
	}

	{
		std::printf("\n");
		std::printf("--------------------------------------------------------------------------------\n");
		std::printf("[ Test for lfDualStackAllocator ]\n");
		std::printf("--------------------------------------------------------------------------------\n");

		//ロックフリー双方向スタックアロケータ
		{
			std::printf("\n");
			std::printf("----------------------------------------\n");
			EXPR_PLAIN(lfDualStackAllocator<> stack(buff););
			std::printf("----------------------------------------\n");
			testDualStack(stack);
		}

		//ロックフリースマート双方向スタックアロケータ
		//※正順か逆順のメモリ確保数が0になったら、対象方向のメモリを自動リセット
		{
			std::printf("\n");
			std::printf("----------------------------------------\n");
			EXPR_PLAIN(lfSmartDualStackAllocator stack(buff););
			std::printf("----------------------------------------\n");
			testDualStack(stack);
		}

		//バッファ付きロックフリー双方向スタックアロケータ
		{
			std::printf("\n");
			std::printf("----------------------------------------\n");
			EXPR_PLAIN(lfDualStackAllocator_withBuff<1024> stack;);
			std::printf("----------------------------------------\n");
			std::printf("\n");
			EXPR_PLAIN(stack.debugInfo(message, sizeof(message)); std::printf(message););
		}

		//バッファ付きロックフリースマート双方向スタックアロケータ
		{
			std::printf("\n");
			std::printf("----------------------------------------\n");
			EXPR_PLAIN(lfSmartDualStackAllocator_withBuff<1024> stack;);
			std::printf("----------------------------------------\n");
			std::printf("\n");
			EXPR_PLAIN(stack.debugInfo(message, sizeof(message)); std::printf(message););
		}

		//型指定バッファ付きロックフリー双方向スタックアロケータ
		//※型のアラインメントサイズ分余計に領域を割り当てる
		{
			std::printf("\n");
			std::printf("----------------------------------------\n");
			EXPR_PLAIN(lfDualStackAllocator_withType<long long, 128> stack;);
			std::printf("----------------------------------------\n");
			std::printf("\n");
			EXPR_PLAIN(stack.debugInfo(message, sizeof(message)); std::printf(message););
			EXPR(p1, long long* p1 = stack.newDefault(););
			EXPR_PLAIN(stack.reversewAllocateOrder(););
			EXPR(p1, long long* p2 = stack.newDefault(););
			EXPR(p2, long long* p3 = stack.newDefaultOrd(ALLOC_ASC););
			EXPR(p3, long long* p4 = stack.newDefaultOrd(ALLOC_DESC););
			EXPR(p1, stack.deleteDefault(p1););
			EXPR(p2, stack.deleteDefault(p2););
			EXPR(p3, stack.deleteDefault(p3););
			EXPR(p4, stack.deleteDefault(p4););
		}

		//型指定バッファ付きロックフリースマート双方向スタックアロケータ
		//※型のアラインメントサイズ分余計に領域を割り当てる
		{
			std::printf("\n");
			std::printf("----------------------------------------\n");
			EXPR_PLAIN(lfSmartDualStackAllocator_withType<long long, 128> stack;);
			std::printf("----------------------------------------\n");
			std::printf("\n");
			EXPR_PLAIN(stack.debugInfo(message, sizeof(message)); std::printf(message););
			EXPR(p1, long long* p1 = stack.newDefault(););
			EXPR_PLAIN(stack.reversewAllocateOrder(););
			EXPR(p1, long long* p2 = stack.newDefault(););
			EXPR(p2, long long* p3 = stack.newDefaultOrd(ALLOC_ASC););
			EXPR(p3, long long* p4 = stack.newDefaultOrd(ALLOC_DESC););
			EXPR(p1, stack.deleteDefault(p1););
			EXPR(p2, stack.deleteDefault(p2););
			EXPR(p3, stack.deleteDefault(p3););
			EXPR(p4, stack.deleteDefault(p4););
		}
	}
	
	{
		std::printf("\n");
		std::printf("--------------------------------------------------------------------------------\n");
		std::printf("[ Test for scoepdStackAllocator / scoepdDualStackAllocator ]\n");
		std::printf("--------------------------------------------------------------------------------\n");

		//双方向スタックアロケータ
		//※スコープスタックアロケータのテスト用
		{
			std::printf("\n");
			std::printf("----------------------------------------\n");
			EXPR_PLAIN(dualStackAllocator<lock_type> stack(buff););
			std::printf("----------------------------------------\n");
			testScopedStack(stack);
			EXPR_PLAIN(stack.clearAll(););
			testScopedDualStack(stack);
		}

		//スマート双方向スタックアロケータ
		//※スコープスタックアロケータのテスト用
		{
			std::printf("\n");
			std::printf("----------------------------------------\n");
			EXPR_PLAIN(smartDualStackAllocator<lock_type> stack(buff););
			std::printf("----------------------------------------\n");
			testDualStack(stack);
			EXPR_PLAIN(stack.clearAll(););
			testScopedDualStack(stack);
		}

		//ロックフリー双方向スタックアロケータ
		//※スコープスタックアロケータのテスト用
		{
			std::printf("\n");
			std::printf("----------------------------------------\n");
			EXPR_PLAIN(lfDualStackAllocator<> stack(buff););
			std::printf("----------------------------------------\n");
			testDualStack(stack);
			EXPR_PLAIN(stack.clearAll(););
			testScopedDualStack(stack);
		}

		//ロックフリースマート双方向スタックアロケータ
		//※スコープスタックアロケータのテスト用
		{
			std::printf("\n");
			std::printf("----------------------------------------\n");
			EXPR_PLAIN(lfSmartDualStackAllocator stack(buff););
			std::printf("----------------------------------------\n");
			testDualStack(stack);
			EXPR_PLAIN(stack.clearAll(););
			testScopedDualStack(stack);
		}
	}
}

// End of file
