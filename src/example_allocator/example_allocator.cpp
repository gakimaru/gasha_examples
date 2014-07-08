//--------------------------------------------------------------------------------
// exmaple_thrad_id.cpp
// アロケータテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_allocator.h"//アロケータテスト

//#include <gasha/mono_allocator.h>//モノアロケータ
#include <gasha/stack_allocator.h>//スタックアロケータ
//#include <gasha/stack_allocator.h>//双方向スタックアロケータ
//#include <gasha/stack_allocator.h>//ロックフリースタックアロケータ
//#include <gasha/stack_allocator.h>//スコープスタックアロケータ
//#include <gasha/stack_allocator.h>//スコープ双方向スタックアロケータ
#include <gasha/pool_allocator.h>//プールアロケータ
#include <gasha/lf_pool_allocator.h>//ロックフリープールアロケータ
//アロケータアダプタ
//多態アロケータ
//スコープ多態アロケータ

#include <gasha/pool_allocator.cpp.h>//プールアロケータ
#include <gasha/lf_pool_allocator.cpp.h>//ロックフリープールアロケータ
#include <gasha/stack_allocator.cpp.h>//スタックアロケータ

#include <gasha/type_traits.h>//型特性ユーティリティ：extentof

#include <gasha/spin_lock.h>//スピンロック

#include <stdio.h>//printf()

//【VC++】例外を無効化した状態で <thread> をインクルードすると、warning C4530 が発生する
//  warning C4530: C++ 例外処理を使っていますが、アンワインド セマンティクスは有効にはなりません。/EHsc を指定してください。
#pragma warning(disable: 4530)//C4530を抑える

#include <thread>//C++11 std::thread
#include <new>

GASHA_USING_NAMESPACE;//ネームスペース使用

GASHA_INSTANCING_stackAllocator();
GASHA_INSTANCING_smartStackAllocator();
//GASHA_INSTANCING_stackAllocator_withLock(spinLock);
//GASHA_INSTANCING_smartStackAllocator_withLock(spinLock);
//GASHA_INSTANCING_stackAllocator_withBuff(80);
//GASHA_INSTANCING_smartStackAllocator_withBuff(80);
//GASHA_INSTANCING_stackAllocator_withBuff_withLock(80, spinLock);
//GASHA_INSTANCING_smartStackAllocator_withBuff_withLock(80, spinLock);
//GASHA_INSTANCING_stackAllocator_withType(int, 20);
//GASHA_INSTANCING_smartStackAllocator_withType(int, 20);
//GASHA_INSTANCING_stackAllocator_withType_withLock(int, 20, spinLock);
//GASHA_INSTANCING_smartStackAllocator_withType_withLock(int, 20, spinLock);

struct st_a0{ int x; st_a0(){ printf("st_a0::st_a0()\n"); } ~st_a0(){ printf("st_a0::~st_a0()\n"); } };
struct alignas(4) st_a4{ int x; st_a4(){ printf("st_a4::st_a4()\n"); } ~st_a4(){ printf("st_a4::~st_a4()\n"); } };
struct alignas(8) st_a8{ int x; st_a8(){ printf("st_a8::st_a8()\n"); } ~st_a8(){ printf("st_a8::~st_a8()\n"); } };
struct alignas(16) st_a16{ int x; st_a16(){ printf("st_a16::st_a16()\n"); } ~st_a16(){ printf("st_a16::~st_a16()\n"); } };
struct alignas(32) st_a32{ int x; st_a32(){ printf("st_a32::st_a32()\n"); } ~st_a32(){ printf("st_a32::~st_a32()\n"); } };

char buff[1024];
void* operator new(std::size_t size, std::size_t id, std::size_t& real_size) GASHA_NOEXCEPT
{
	real_size = size;
	printf("new: size=%d, id=%d, real_size=%d\n", size, id, real_size);
	return nullptr;
	//return buff;
}
void operator delete(void* p, std::size_t id, std::size_t& real_size)
{
	printf("delete: p=%p, id=%d, real_size=%d\n", p, id, real_size);
}
void* operator new[](std::size_t size, std::size_t id, std::size_t& real_size) GASHA_NOEXCEPT
{
	real_size = size;
	printf("new[]: size=%d, id=%d, real_size=%d\n", size, id, real_size);
	return nullptr;
	//return buff;
}
void operator delete[](void* p, std::size_t id, std::size_t& real_size)
{
	printf("delete[]: p=%p, id=%d, real_size=%d\n", p, id, real_size);
}

//----------------------------------------
//アロケータテスト
void example_allocator()
{
	printf("----- Test for allocator -----\n");

	//仮：配置newテスト
	{
		printf("sizeof(st_a0)=%d, alignof(st_a0)=%d\n", sizeof(st_a0), alignof(st_a0));
		printf("sizeof(st_a4)=%d, alignof(st_a4)=%d\n", sizeof(st_a4), alignof(st_a4));
		printf("sizeof(st_a8)=%d, alignof(st_a8)=%d\n", sizeof(st_a8), alignof(st_a8));
		printf("sizeof(st_a16)=%d, alignof(st_a16)=%d\n", sizeof(st_a16), alignof(st_a16));
		printf("sizeof(st_a32)=%d, alignof(st_a32)=%d\n", sizeof(st_a32), alignof(st_a32));
		std::size_t real_size;
		new(1, real_size)st_a0;
		new(2, real_size)st_a4;
		new(3, real_size)st_a8;
		new(4, real_size)st_a16;
		new(5, real_size)st_a32;
		new(6, real_size)st_a0[2];
		new(7, real_size)st_a4[2];
		new(8, real_size)st_a8[2];
		new(9, real_size)st_a16[2];
		new(10, real_size)st_a32[2];
	}

	//スタックアロケータテスト
	{
		char message[2048];
		smartStackAllocator_withType<int, 9, spinLock> allocator; allocator.debugInfo(message); printf(message);
		void* p1 = allocator.alloc(1, 1);                         allocator.debugInfo(message); printf(message);
		void* p2 = allocator.alloc(1, 1);                         allocator.debugInfo(message); printf(message);
		int* i = allocator.template newObj<int>();                allocator.debugInfo(message); printf(message);
		double* d3 = allocator.template newArray<double>(3);      allocator.debugInfo(message); printf(message);
		int* i2 = allocator.newDefault();                         allocator.debugInfo(message); printf(message);
		double* d = allocator.template newObj<double>();          allocator.debugInfo(message); printf(message);
		void* p3 = allocator.alloc(1, 1);                         allocator.debugInfo(message); printf(message);
		st_a0* a0 = allocator.template newObj<st_a0>();           allocator.debugInfo(message); printf(message);
		st_a4* a4 = allocator.template newObj<st_a4>();           allocator.debugInfo(message); printf(message);
		st_a8* a8 = allocator.template newObj<st_a8>();           allocator.debugInfo(message); printf(message);
		st_a16* a16 = allocator.template newObj<st_a16>();        allocator.debugInfo(message); printf(message);
		st_a32* a32 = allocator.template newObj<st_a32>();        allocator.debugInfo(message); printf(message);
		void* p4 = allocator.alloc(1, 1);                         allocator.debugInfo(message); printf(message);
		allocator.free(p1);                                       allocator.debugInfo(message); printf(message);
		allocator.free(p2);                                       allocator.debugInfo(message); printf(message);
		allocator.free(p3);                                       allocator.debugInfo(message); printf(message);
		allocator.free(p4);                                       allocator.debugInfo(message); printf(message);
		allocator.deleteObj(a0);                                  allocator.debugInfo(message); printf(message);
		allocator.deleteObj(a4);                                  allocator.debugInfo(message); printf(message);
		allocator.deleteObj(a8);                                  allocator.debugInfo(message); printf(message);
		allocator.deleteObj(a16);                                 allocator.debugInfo(message); printf(message);
		allocator.deleteObj(a32);                                 allocator.debugInfo(message); printf(message);
		allocator.deleteObj(i);                                   allocator.debugInfo(message); printf(message);
		allocator.deleteObj(i2);                                  allocator.debugInfo(message); printf(message);
		allocator.deleteObj(d);                                   allocator.debugInfo(message); printf(message);
		allocator.deleteArray(d3, 3);                             allocator.debugInfo(message); printf(message);
	}

	//仮：プールアロケータ
	{
		struct st{
			char m[35]; st(){ m[0] = 'a'; m[1] = 'b'; m[2] = 'c'; printf("st::st()\n"); }
			~st(){ printf("st::~st()\n"); }
		};
		st x[10];
		struct alignas(16) st2a{ int m; st2a(int n) :m(n){ printf("st2a::st2a()\n"); } ~st2a(){ printf("st2a::~st2a()\n"); } };
		struct alignas(32) st2b{ int m; st2b(int n) :m(n){ printf("st2b::st2b()\n"); } ~st2b(){ printf("st2b::~st2b()\n"); } };
		struct st3{
			int m;
		st3(int n) :m(n){ printf("st3::st3()\n"); }
		~st3(){ printf("st3::~st3()\n"); }
		};
		
		char message[2048];
		
		poolAllocator_withType<st, 10> x1;
		lfPoolAllocator<10> x2(x);
		st* s1 = x1.newDefault();
		x1.deleteDefault(s1);
		st2a* s2a = x1.template newObj<st2a>(99);
		x1.deleteObj(s2a);
		st2b* s2b = x1.template newObj<st2b>(99);
		x1.deleteObj(s2b);
		st3* s3 = x1.template newArray<st3>(3, 99);
		x1.debugInfo(message);
		printf(message);
		x1.deleteArray(s3, 3);

		poolAllocator_withType<st, 10> x3;
		lfPoolAllocator<10> x4(x, extentof(x));
		s1 = x3.newDefault();
		x3.deleteDefault(s1);
		s2a = x3.template newObj<st2a>(99);
		x3.deleteObj(s2a);
		s2b = x3.template newObj<st2b>(99);
		x3.deleteObj(s2b);
		s3 = x3.template newArray<st3>(3, 99);
		x3.debugInfo(message);
		printf(message);
		x3.deleteArray(s3, 3);
	}

	printf("- end -\n");
}

// End of file
