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
//#include <gasha/stack_allocator.h>//スタックアロケータ
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

#include <gasha/spin_lock.h>//スピンロック

#include <stdio.h>//printf()

//【VC++】例外を無効化した状態で <thread> をインクルードすると、warning C4530 が発生する
//  warning C4530: C++ 例外処理を使っていますが、アンワインド セマンティクスは有効にはなりません。/EHsc を指定してください。
#pragma warning(disable: 4530)//C4530を抑える

#include <thread>//C++11 std::thread
#include <new>

GASHA_USING_NAMESPACE;//ネームスペース使用

struct a{ int x; a(){ printf("a::a()\n"); } };
struct b{ int x; b(){ printf("b::b()\n"); }~b(){} };

char buff[1024];
void* operator new[](std::size_t size, std::size_t align, std::size_t& real_size) GASHA_NOEXCEPT
{
	real_size = size;
	printf("size=%d, align=%d, real_size=%d\n", size, align, real_size);
	return nullptr;
	//return buff;
}
void operator delete(void* p, std::size_t align, std::size_t& real_size)
{
	printf("p=%p, align=%d, real_size=%d\n", p, align, real_size);
}

//----------------------------------------
//アロケータテスト
void example_allocator()
{
	printf("----- Test for allocator -----\n");

	//スタックアロケータ
	//smartStackAllocator_withType<int, 20, spinLock> x;
	printf("a[10]=%d\n", sizeof(a[10]));
	printf("b[10]=%d\n", sizeof(b[10]));
	std::size_t real_size;
	new(1, real_size)a[10];
	new(2, real_size)b[10];

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
		poolAllocator_withType<st, 10> x1;
		lfPoolAllocator<10> x2(x);
		st* s1 = x1.newDefault();
		x1.deleteDefault(s1);
		st2a* s2a = x1.template newObj<st2a>(99);
		x1.deleteObj(s2a);
		st2b* s2b = x1.template newObj<st2b>(99);
		x1.deleteObj(s2b);
		st3* s3 = x1.template newArray<st3>(3, 99);
		x1.deleteArray(s3, 3);

		poolAllocator_withType<st, 10> x3;
		lfPoolAllocator<10> x4(x, sizeof(x));
		s1 = x3.newDefault();
		x3.deleteDefault(s1);
		s2a = x3.template newObj<st2a>(99);
		x3.deleteObj(s2a);
		s2b = x3.template newObj<st2b>(99);
		x3.deleteObj(s2b);
		s3 = x3.template newArray<st3>(3, 99);
		x3.deleteArray(s3, 3);
	}

	printf("- end -\n");
}

// End of file
