//--------------------------------------------------------------------------------
// exmaple_stack_allocator.cpp
// スタックアロケータテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_allocator.h"//アロケータテスト

#include <gasha/stack_allocator.h>//スタックアロケータ
#include <gasha/lf_stack_allocator.h>//ロックフリースタックアロケータ
#include <gasha/scoped_stack_allocator.h>//スコープスタックアロケータ

#include <gasha/spin_lock.h>//スピンロック

#include <stdio.h>//printf()

GASHA_USING_NAMESPACE;//ネームスペース使用

//----------------------------------------
//基本テスト

//テスト用マクロ
#define EXPR_PLAIN(...) printf("%s\n", #__VA_ARGS__); __VA_ARGS__
#define EXPR_WITH_INFO(...) __VA_ARGS__ printf("%s\t\tsize=%d, remain=%d, count=%d\n", #__VA_ARGS__, stack.size(), stack.remain(), stack.count())
#define EXPR(P, ...) __VA_ARGS__ printf("%s\t\t%s=%p, size=%d, remain=%d, count=%d\n", #__VA_ARGS__, #P, P, stack.size(), stack.remain(), stack.count())
#define EXPR_SCOPED_PLAIN(...) printf("\t%s\n", #__VA_ARGS__); __VA_ARGS__
#define EXPR_SCOPED(P, ...) __VA_ARGS__ printf("\t%s\t\t%s=%p, size=%d, remain=%d, count=%d\n", #__VA_ARGS__, #P, P, scoped_stack.size(), scoped_stack.remain(), scoped_stack.count())
#define EXPR_SCOPED_WITH_INFO(...) __VA_ARGS__ printf("\t%s\t\tsize=%d, remain=%d, count=%d\n", #__VA_ARGS__, scoped_stack.size(), scoped_stack.remain(), scoped_stack.count())

//共通テスト
template<class ALLOCATOR>
static void test(ALLOCATOR& stack)
{
	printf("\n");
	char message[1024];
	EXPR_PLAIN(stack.debugInfo(message); printf(message););
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
	EXPR(p13, void* p13 = stack.alloc(1000););
	EXPR(p14, void* p14 = stack.alloc(10););
	EXPR_PLAIN(stack.debugInfo(message); printf(message););
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
	EXPR(p14, stack.free(p14););
	EXPR_PLAIN(stack.debugInfo(message); printf(message););
	EXPR_WITH_INFO(stack.rewind(900););
	EXPR_WITH_INFO(stack.rewind(20););
	EXPR(p100, void* p100 = stack.alloc(10););
	EXPR(p101, void* p101 = stack.alloc(20););
	EXPR_WITH_INFO(stack.clear(););
	EXPR_PLAIN(stack.debugInfo(message); printf(message););
}
template<class ALLOCATOR>
static void testScoped(ALLOCATOR& stack)
{
	printf("\n");
	char message[1024];
	EXPR_PLAIN(stack.debugInfo(message); printf(message););
	EXPR(p, void* p = stack.alloc(100););
	{
		printf("***** Begin : Scoped stack allocator *****\n");
		EXPR_SCOPED_PLAIN(auto scoped_stack = stack.scopedAllocator(););
		EXPR_SCOPED_PLAIN(scoped_stack.debugInfo(message); printf(message););
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
		EXPR_SCOPED(p13, void* p13 = scoped_stack.alloc(1000););
		EXPR_SCOPED(p14, void* p14 = scoped_stack.alloc(10););
		EXPR_SCOPED_PLAIN(scoped_stack.debugInfo(message); printf(message););
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
		EXPR_SCOPED(p14, scoped_stack.free(p14););
		EXPR_SCOPED_PLAIN(scoped_stack.debugInfo(message); printf(message););
		EXPR_SCOPED_WITH_INFO(scoped_stack.rewind(900););
		EXPR_SCOPED_WITH_INFO(scoped_stack.rewind(20););
		EXPR_SCOPED(p100, void* p100 = scoped_stack.alloc(10););
		EXPR_SCOPED(p101, void* p101 = scoped_stack.alloc(20););
		EXPR_SCOPED_WITH_INFO(scoped_stack.clear(););
		EXPR_SCOPED(p102, void* p102 = scoped_stack.alloc(30););
		EXPR_SCOPED(p103, void* p103 = scoped_stack.alloc(40););
		printf("***** End : Scoped stack allocator *****\n");
	}
	EXPR_PLAIN(stack.debugInfo(message); printf(message););
	EXPR(p, stack.free(p););
	EXPR_PLAIN(stack.debugInfo(message); printf(message););
}

//----------------------------------------
//スタックアロケータテスト
void example_stack_allocator()
{
	alignas(16) char buff[1024];
	char message[1024];

	{
		printf("\n");
		printf("--------------------------------------------------------------------------------\n");
		printf("[ Test for stackAllocator ]\n");
		printf("--------------------------------------------------------------------------------\n");

		{
			printf("\n");
			printf("----------------------------------------\n");
			EXPR_PLAIN(stackAllocator<> stack(buff););
			printf("----------------------------------------\n");
			test(stack);
		}
		{
			printf("\n");
			printf("----------------------------------------\n");
			EXPR_PLAIN(smartStackAllocator<> stack(buff););
			printf("----------------------------------------\n");
			test(stack);
		}
		{
			printf("\n");
			printf("----------------------------------------\n");
			EXPR_PLAIN(stackAllocator<spinLock> stack(buff););
			printf("----------------------------------------\n");
			test(stack);
		}
		{
			printf("\n");
			printf("----------------------------------------\n");
			EXPR_PLAIN(smartStackAllocator<spinLock> stack(buff););
			printf("----------------------------------------\n");
			test(stack);
		}
		{
			printf("\n");
			printf("----------------------------------------\n");
			EXPR_PLAIN(stackAllocator_withBuff<1024> stack;);
			printf("----------------------------------------\n");
			printf("\n");
			EXPR_PLAIN(stack.debugInfo(message); printf(message););
		}
		{
			printf("\n");
			printf("----------------------------------------\n");
			EXPR_PLAIN(stackAllocator_withType<long long, 128> stack;);
			printf("----------------------------------------\n");
			printf("\n");
			EXPR_PLAIN(stack.debugInfo(message); printf(message););
			EXPR(p, long long* p = stack.newDefault(););
			EXPR(p, stack.deleteDefault(p););
		}
	}

	{
		printf("\n");
		printf("--------------------------------------------------------------------------------\n");
		printf("[ Test for lfStackAllocator ]\n");
		printf("--------------------------------------------------------------------------------\n");

		{
			printf("\n");
			printf("----------------------------------------\n");
			EXPR_PLAIN(lfStackAllocator<> stack(buff););
			printf("----------------------------------------\n");
			test(stack);
		}
		{
			printf("\n");
			printf("----------------------------------------\n");
			EXPR_PLAIN(lfSmartStackAllocator stack(buff););
			printf("----------------------------------------\n");
			test(stack);
		}
		{
			printf("\n");
			printf("----------------------------------------\n");
			EXPR_PLAIN(lfStackAllocator_withBuff<1024> stack;);
			printf("----------------------------------------\n");
			printf("\n");
			EXPR_PLAIN(stack.debugInfo(message); printf(message););
		}
		{
			printf("\n");
			printf("----------------------------------------\n");
			EXPR_PLAIN(lfStackAllocator_withType<long long, 128> stack;);
			printf("----------------------------------------\n");
			printf("\n");
			EXPR_PLAIN(stack.debugInfo(message); printf(message););
			EXPR(p, long long* p = stack.newDefault(););
			EXPR(p, stack.deleteDefault(p););
		}
	}


	{
		printf("\n");
		printf("--------------------------------------------------------------------------------\n");
		printf("[ Test for scoepdStackAllocator ]\n");
		printf("--------------------------------------------------------------------------------\n");

		{
			printf("\n");
			printf("----------------------------------------\n");
			EXPR_PLAIN(stackAllocator<> stack(buff););
			printf("----------------------------------------\n");
			testScoped(stack);
		}

		{
			printf("\n");
			printf("----------------------------------------\n");
			EXPR_PLAIN(lfStackAllocator<> stack(buff););
			printf("----------------------------------------\n");
			testScoped(stack);
		}

		{
			printf("\n");
			printf("----------------------------------------\n");
			EXPR_PLAIN(smartStackAllocator<> stack(buff););
			printf("----------------------------------------\n");
			testScoped(stack);
		}

		{
			printf("\n");
			printf("----------------------------------------\n");
			EXPR_PLAIN(lfSmartStackAllocator stack(buff););
			printf("----------------------------------------\n");
			testScoped(stack);
		}
	}
}

// End of file
