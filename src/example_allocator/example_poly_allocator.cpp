//--------------------------------------------------------------------------------
// exmaple_poly_allocator.cpp
// 多態アロケータテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_allocator.h"//アロケータテスト

#include <gasha/mono_allocator.h>//単一アロケータ
#include <gasha/lf_mono_allocator.h>//ロックフリー単一アロケータ
#include <gasha/stack_allocator.h>//スタックアロケータ
#include <gasha/stack_allocator.h>//ロックフリースタックアロケータ
#include <gasha/dual_stack_allocator.h>//双方向スタックアロケータ
#include <gasha/lf_dual_stack_allocator.h>//ロックフリー双方向スタックアロケータ
#include <gasha/pool_allocator.h>//プールアロケータ
#include <gasha/lf_pool_allocator.h>//ロックフリープールアロケータ
#include <gasha/std_allocator.h>//標準アロケータ

#include <gasha/scoped_stack_allocator.h>//スコープスタックアロケータ
#include <gasha/scoped_dual_stack_allocator.h>//双方向スコープスタックアロケータ

#include <gasha/allocator_adapter.h>//アロケータアダプター

#include <gasha/poly_allocator.h>//多態アロケータ
#include <gasha/new.h>//new/delete操作

#include <gasha/type_traits.h>//型特性ユーティリティ：extentof
#include <gasha/spin_lock.h>//スピンロック

#include <stdio.h>//printf()

GASHA_USING_NAMESPACE;//ネームスペース使用

//----------------------------------------
//多態アロケータテスト
void example_poly_allocator()
{
#if 0
	char message[2048];
	
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
		//stackAllocator_withType<int, 9>                           allocator; allocator.debugInfo(message); printf(message);
		//lfStackAllocator_withType<int, 9>                         allocator; allocator.debugInfo(message); printf(message);
		//smartStackAllocator_withType<int, 9>                      allocator; allocator.debugInfo(message); printf(message);
		lfSmartStackAllocator_withType<int, 9>                    allocator; allocator.debugInfo(message); printf(message);
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
		void* p5 = allocator.alloc(0, 16);                        allocator.debugInfo(message); printf(message);
		allocator.free(p1);                                       allocator.debugInfo(message); printf(message);
		allocator.free(p2);                                       allocator.debugInfo(message); printf(message);
		allocator.free(p3);                                       allocator.debugInfo(message); printf(message);
		allocator.free(p4);                                       allocator.debugInfo(message); printf(message);
		allocator.free(p5);                                       allocator.debugInfo(message); printf(message);
		allocator.deleteObj(a0);                                  allocator.debugInfo(message); printf(message);
		allocator.deleteObj(a4);                                  allocator.debugInfo(message); printf(message);
		allocator.deleteObj(a8);                                  allocator.debugInfo(message); printf(message);
		allocator.deleteObj(a16);                                 allocator.debugInfo(message); printf(message);
		allocator.deleteObj(a32);                                 allocator.debugInfo(message); printf(message);
		allocator.deleteObj(i);                                   allocator.debugInfo(message); printf(message);
		allocator.deleteObj(i2);                                  allocator.debugInfo(message); printf(message);
		allocator.deleteObj(d);                                   allocator.debugInfo(message); printf(message);
		allocator.deleteArray(d3, 3);                             allocator.debugInfo(message); printf(message);//自動クリア
		p1 = allocator.alloc(1, 1);                               allocator.debugInfo(message); printf(message);
		allocator.free(p5);                                       allocator.debugInfo(message); printf(message);//二重解放
		allocator.free(p1);                                       allocator.debugInfo(message); printf(message);
		allocator.clear();                                        allocator.debugInfo(message); printf(message);
		a16 = allocator.template newObj<st_a16>();                allocator.debugInfo(message); printf(message);
		allocator.rewind(8);                                      allocator.debugInfo(message); printf(message);
		allocator.rewind(16);                                     allocator.debugInfo(message); printf(message);
		allocator.clear();                                        allocator.debugInfo(message); printf(message);

		i = allocator.template newArray<int>(2);  allocator.debugInfo(message); printf(message);
		{
			auto sub_allocator = allocator.scopedAllocator();
			sub_allocator.debugInfo(message); printf(message);
			void* p1 = sub_allocator.alloc(1); sub_allocator.debugInfo(message); printf(message);
			void* p2 = sub_allocator.alloc(30, 1); sub_allocator.debugInfo(message); printf(message);
			sub_allocator.free(p1); sub_allocator.debugInfo(message); printf(message);
			sub_allocator.free(p2); sub_allocator.debugInfo(message); printf(message);
		}
		allocator.debugInfo(message); printf(message);
		allocator.deleteArray(i, 2);  allocator.debugInfo(message); printf(message);
	}

	//双方向スタックアロケータテスト
	{
		//dualStackAllocator_withType<int, 9>                       allocator; allocator.debugInfo(message); printf(message);
		//lfDualStackAllocator_withType<int, 9>                     allocator; allocator.debugInfo(message); printf(message);
		//smartDualStackAllocator_withType<int, 9>                  allocator; allocator.debugInfo(message); printf(message);
		lfSmartDualStackAllocator_withType<int, 9>                allocator; allocator.debugInfo(message); printf(message);
		//allocator.reversewAllocateOrder();
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
		void* p5 = allocator.alloc(0, 16);                        allocator.debugInfo(message); printf(message);
		allocator.free(p1);                                       allocator.debugInfo(message); printf(message);
		allocator.free(p2);                                       allocator.debugInfo(message); printf(message);
		allocator.free(p3);                                       allocator.debugInfo(message); printf(message);
		allocator.free(p4);                                       allocator.debugInfo(message); printf(message);
		allocator.free(p5);                                       allocator.debugInfo(message); printf(message);
		allocator.deleteObj(a0);                                  allocator.debugInfo(message); printf(message);
		allocator.deleteObj(a4);                                  allocator.debugInfo(message); printf(message);
		allocator.deleteObj(a8);                                  allocator.debugInfo(message); printf(message);
		allocator.deleteObj(a16);                                 allocator.debugInfo(message); printf(message);
		allocator.deleteObj(a32);                                 allocator.debugInfo(message); printf(message);
		allocator.deleteObj(i);                                   allocator.debugInfo(message); printf(message);
		allocator.deleteObj(i2);                                  allocator.debugInfo(message); printf(message);
		allocator.deleteObj(d);                                   allocator.debugInfo(message); printf(message);
		allocator.deleteArray(d3, 3);                             allocator.debugInfo(message); printf(message);//自動クリア
		allocator.reversewAllocateOrder();
		p1 = allocator.alloc(1, 1);                               allocator.debugInfo(message); printf(message);
		allocator.free(p5);                                       allocator.debugInfo(message); printf(message);//二重解放
		allocator.free(p1);                                       allocator.debugInfo(message); printf(message);
		allocator.clear();                                        allocator.debugInfo(message); printf(message);
		a16 = allocator.template newObj<st_a16>();                allocator.debugInfo(message); printf(message);
		allocator.rewind(8);                                      allocator.debugInfo(message); printf(message);
		allocator.rewind(16);                                     allocator.debugInfo(message); printf(message);
		allocator.clear();                                        allocator.debugInfo(message); printf(message);
		p1 = allocator.allocOrd(ALLOC_ASC, 1, 1);             allocator.debugInfo(message); printf(message);
		p2 = allocator.allocOrd(ALLOC_DESC, 1, 1);            allocator.debugInfo(message); printf(message);
		i = allocator.template newObjOrd<int>(ALLOC_ASC);     allocator.debugInfo(message); printf(message);
		i2 = allocator.template newObjOrd<int>(ALLOC_DESC);   allocator.debugInfo(message); printf(message);
		short* i3 = allocator.template newArrayOrd<short>(ALLOC_ASC, 2);  allocator.debugInfo(message); printf(message);
		short* i4 = allocator.template newArrayOrd<short>(ALLOC_DESC, 2); allocator.debugInfo(message); printf(message);
		int* i5 = allocator.newDefaultOrd(ALLOC_ASC);         allocator.debugInfo(message); printf(message);
		int* i6 = allocator.newDefaultOrd(ALLOC_DESC);        allocator.debugInfo(message); printf(message);
		allocator.free(p1);                                       allocator.debugInfo(message); printf(message);
		allocator.free(p2);                                       allocator.debugInfo(message); printf(message);
		allocator.deleteObj(i);                                   allocator.debugInfo(message); printf(message);
		allocator.deleteObj(i2);                                  allocator.debugInfo(message); printf(message);
		allocator.deleteArray(i3, 2);                             allocator.debugInfo(message); printf(message);
		allocator.deleteArray(i4, 2);                             allocator.debugInfo(message); printf(message);
		allocator.deleteObj(i5);                                  allocator.debugInfo(message); printf(message);
		allocator.deleteObj(i6);                                  allocator.debugInfo(message); printf(message);

		//allocator.reversewAllocateOrder();
		i = allocator.template newArray<int>(2);  allocator.debugInfo(message); printf(message);
		{
			auto sub_allocator = allocator.scopedAllocator();
			sub_allocator.debugInfo(message); printf(message);
			void* p1 = sub_allocator.alloc(1); sub_allocator.debugInfo(message); printf(message);
			void* p2 = sub_allocator.alloc(30, 1); sub_allocator.debugInfo(message); printf(message);
			sub_allocator.free(p1); sub_allocator.debugInfo(message); printf(message);
			sub_allocator.free(p2); sub_allocator.debugInfo(message); printf(message);
			sub_allocator.clear(); sub_allocator.debugInfo(message); printf(message);
			p1 = sub_allocator.alloc(1); sub_allocator.debugInfo(message); printf(message);
			sub_allocator.free(p1); sub_allocator.debugInfo(message); printf(message);
		}
		allocator.debugInfo(message); printf(message);
		allocator.deleteArray(i, 2);  allocator.debugInfo(message); printf(message);

		//allocator.reversewAllocateOrder();
		i = allocator.template newArray<int>(2);  allocator.debugInfo(message); printf(message);
		allocator.reversewAllocateOrder();
		i2 = allocator.template newArray<int>(1);  allocator.debugInfo(message); printf(message);
		{
			auto sub_allocator = allocator.scopedDualAllocator();
			sub_allocator.debugInfo(message); printf(message);
			void* p1 = sub_allocator.alloc(1); sub_allocator.debugInfo(message); printf(message);
			void* p2 = sub_allocator.alloc(4, 4); sub_allocator.debugInfo(message); printf(message);
			sub_allocator.reversewAllocateOrder();
			void* p3 = sub_allocator.alloc(1); sub_allocator.debugInfo(message); printf(message);
			void* p4 = sub_allocator.alloc(4, 4); sub_allocator.debugInfo(message); printf(message);
			sub_allocator.free(p1); sub_allocator.debugInfo(message); printf(message);
			sub_allocator.free(p2); sub_allocator.debugInfo(message); printf(message);
			sub_allocator.free(p3); sub_allocator.debugInfo(message); printf(message);
			sub_allocator.free(p4); sub_allocator.debugInfo(message); printf(message);
			sub_allocator.clear(); sub_allocator.debugInfo(message); printf(message);
			sub_allocator.clearOrd(ALLOC_DESC); sub_allocator.debugInfo(message); printf(message);
			sub_allocator.clearOrd(ALLOC_ASC); sub_allocator.debugInfo(message); printf(message);
			p1 = sub_allocator.alloc(1); sub_allocator.debugInfo(message); printf(message);
			sub_allocator.reversewAllocateOrder();
			p3 = sub_allocator.alloc(1); sub_allocator.debugInfo(message); printf(message);
			sub_allocator.free(p1); sub_allocator.debugInfo(message); printf(message);
			sub_allocator.free(p3); sub_allocator.debugInfo(message); printf(message);
		}
		allocator.debugInfo(message); printf(message);
		allocator.deleteArray(i, 2);  allocator.debugInfo(message); printf(message);
		allocator.deleteArray(i2, 1);  allocator.debugInfo(message); printf(message);
	}

	//単一アロケータテスト
	{
		//monoAllocator_withType<int, 9>                            allocator; allocator.debugInfo(message); printf(message);
		lfMonoAllocator_withType<int, 9>                         allocator; allocator.debugInfo(message); printf(message);
		void* p1 = allocator.alloc(1, 1);                         allocator.debugInfo(message); printf(message);
		int* i = allocator.template newObj<int>();                allocator.debugInfo(message); printf(message);
		double* d3 = allocator.template newArray<double>(3);      allocator.debugInfo(message); printf(message);
		int* i2 = allocator.newDefault();                         allocator.debugInfo(message); printf(message);
		st_a0* a0 = allocator.template newObj<st_a0>();           allocator.debugInfo(message); printf(message);
		st_a4* a4 = allocator.template newObj<st_a4>();           allocator.debugInfo(message); printf(message);
		st_a8* a8 = allocator.template newObj<st_a8>();           allocator.debugInfo(message); printf(message);
		st_a16* a16 = allocator.template newObj<st_a16>();        allocator.debugInfo(message); printf(message);
		st_a32* a32 = allocator.template newObj<st_a32>();        allocator.debugInfo(message); printf(message);
		allocator.free(p1);                                       allocator.debugInfo(message); printf(message);
		i = allocator.template newObj<int>();                     allocator.debugInfo(message); printf(message);
		allocator.deleteObj(i);                                   allocator.debugInfo(message); printf(message);
		d3 = allocator.template newArray<double>(3);              allocator.debugInfo(message); printf(message);
		allocator.deleteArray(d3, 3);                             allocator.debugInfo(message); printf(message);
		i2 = allocator.newDefault();                              allocator.debugInfo(message); printf(message);
		allocator.deleteObj(i2);                                  allocator.debugInfo(message); printf(message);
		a0 = allocator.template newObj<st_a0>();                  allocator.debugInfo(message); printf(message);
		allocator.deleteObj(a0);                                  allocator.debugInfo(message); printf(message);
		a4 = allocator.template newObj<st_a4>();                  allocator.debugInfo(message); printf(message);
		allocator.deleteObj(a4);                                  allocator.debugInfo(message); printf(message);
		a8 = allocator.template newObj<st_a8>();                  allocator.debugInfo(message); printf(message);
		allocator.deleteObj(a8);                                  allocator.debugInfo(message); printf(message);
		a16 = allocator.template newObj<st_a16>();                allocator.debugInfo(message); printf(message);
		allocator.deleteObj(a16);                                 allocator.debugInfo(message); printf(message);
		a32 = allocator.template newObj<st_a32>();                allocator.debugInfo(message); printf(message);
		allocator.deleteObj(a32);                                 allocator.debugInfo(message); printf(message);
	}
	
	//プールアロケータテスト
	{
		struct st{
			char m[35];
			st(){ m[0] = 'a'; m[1] = 'b'; m[2] = 'c'; printf("st::st()\n"); }
			~st(){ printf("st::~st()\n"); }
			st(const st&){ printf("(copy consstructor)\n"); }
			st(st&&){ printf("(move consstructor)\n"); }
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
		st* s1c = x1.newDefault(*s1);
		st* s1m = x1.newDefault(std::move(*s1));
		x1.deleteDefault(s1);
		x1.deleteDefault(s1c);
		x1.deleteDefault(s1m);
		st2a* s2a = x1.template newObj<st2a>(99);
		x1.deleteObj(s2a);
		st2b* s2b = x1.template newObj<st2b>(99);
		x1.deleteObj(s2b);
		st3* s3 = x1.template newArray<st3>(3, 99);
		x1.debugInfo(message, false); printf(message);
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
		x3.debugInfo(message, false); printf(message);
		x3.deleteArray(s3, 3);

		struct st_p1 { int a; st_p1() :a(1){ printf("st_p1::st_p1():a=%d\n", a); } ~st_p1(){ printf("st_p1::~st_p1():a=%d\n", a); } };
		struct st_p2 { int b; st_p2() :b(2){ printf("st_p2::st_p2():b=%d\n", b); } ~st_p2(){ printf("st_p2::~st_p2():b=%d\n", b); } };
		struct st_c : st_p1, st_p2 { int c; st_c() :c(3){ printf("st_c::st_c():c=%d\n", c); } ~st_c(){ printf("st_c::~st_c():c=%d\n", c); } };

		poolAllocator_withType<st_c, 10> x5;
		st_c* p1 = x5.newDefault();
		st_p1* p2 = x5.newDefault();
		st_p2* p3 = x5.newDefault();
		st_p1* p1_1 = p1;
		st_p2* p1_2 = p1;
		st_c* p2_c = static_cast<st_c*>(p2);
		st_c* p3_c = static_cast<st_c*>(p3);
		x5.debugInfo(message, false); printf(message);
		x5.deleteObj(static_cast<st_c*>(p1_2));
		x5.deleteObj(static_cast<st_c*>(p1_1));
		x5.deleteObj(p1);
		x5.deleteObj(p2_c);
		x5.deleteObj(p3_c);
		x5.debugInfo(message, false); printf(message);
	}

	//標準アロケータテスト
	{
		stdAllocator<>                                           allocator; allocator.debugInfo(message); printf(message);
		void* p1 = allocator.alloc(1, 1);                         allocator.debugInfo(message); printf(message);
		int* i = allocator.template newObj<int>();                allocator.debugInfo(message); printf(message);
		double* d3 = allocator.template newArray<double>(3);      allocator.debugInfo(message); printf(message);
		int* i2 = allocator.template newObj<int>();               allocator.debugInfo(message); printf(message);
		st_a0* a0 = allocator.template newObj<st_a0>();           allocator.debugInfo(message); printf(message);
		st_a4* a4 = allocator.template newObj<st_a4>();           allocator.debugInfo(message); printf(message);
		st_a8* a8 = allocator.template newObj<st_a8>();           allocator.debugInfo(message); printf(message);
		st_a16* a16 = allocator.template newObj<st_a16>();        allocator.debugInfo(message); printf(message);
		st_a32* a32 = allocator.template newObj<st_a32>();        allocator.debugInfo(message); printf(message);
		allocator.free(p1);                                       allocator.debugInfo(message); printf(message);
		i = allocator.template newObj<int>();                     allocator.debugInfo(message); printf(message);
		allocator.deleteObj(i);                                   allocator.debugInfo(message); printf(message);
		d3 = allocator.template newArray<double>(3);              allocator.debugInfo(message); printf(message);
		allocator.deleteArray(d3, 3);                             allocator.debugInfo(message); printf(message);
		i2 = allocator.template newObj<int>();                    allocator.debugInfo(message); printf(message);
		allocator.deleteObj(i2);                                  allocator.debugInfo(message); printf(message);
		a0 = allocator.template newObj<st_a0>();                  allocator.debugInfo(message); printf(message);
		allocator.deleteObj(a0);                                  allocator.debugInfo(message); printf(message);
		a4 = allocator.template newObj<st_a4>();                  allocator.debugInfo(message); printf(message);
		allocator.deleteObj(a4);                                  allocator.debugInfo(message); printf(message);
		a8 = allocator.template newObj<st_a8>();                  allocator.debugInfo(message); printf(message);
		allocator.deleteObj(a8);                                  allocator.debugInfo(message); printf(message);
		a16 = allocator.template newObj<st_a16>();                allocator.debugInfo(message); printf(message);
		allocator.deleteObj(a16);                                 allocator.debugInfo(message); printf(message);
		a32 = allocator.template newObj<st_a32>();                allocator.debugInfo(message); printf(message);
		allocator.deleteObj(a32);                                 allocator.debugInfo(message); printf(message);
	}

	//マルチスレッドテスト
	{
		static const std::size_t alloc_size = 4;
		static const std::size_t align_size = 4;
		static const std::size_t alloc_num = 100;
		static const std::size_t alloc_error_num = 0;
		static const std::size_t thread_num = 10;
		static const std::size_t repeat_num = 100;
		static const std::size_t pool_size = alloc_num * thread_num;
		static const std::size_t buff_size = alloc_size * pool_size;
		auto alloc_thread = [](std::function<void*(const std::size_t, const std::size_t)> alloc_func, std::function<void(void*)> free_func)
		{
			for (std::size_t i = 0; i < repeat_num; ++i)
			{
				void* ptr[alloc_num + alloc_error_num] = { nullptr };
				std::this_thread::sleep_for(std::chrono::microseconds(1));
				for (std::size_t i = 0; i < extentof(ptr); ++i)
					ptr[i] = alloc_func(i % (alloc_size + 1), align_size);
				std::this_thread::sleep_for(std::chrono::microseconds(1));
				for (std::size_t i = 0; i < extentof(ptr); ++i)
					free_func(ptr[i]);
			}
		};
		auto thread_test = [&alloc_thread](std::function<void*(const std::size_t, const std::size_t)> alloc_func, std::function<void(void*)> free_func, std::function<void()> print_func)
		{
			std::thread* th[thread_num];
			for (std::size_t i = 0; i < thread_num; ++i)
				th[i] = new std::thread(alloc_thread, alloc_func, free_func);
			for (std::size_t i = 0; i < thread_num; ++i)
			{
				th[i]->join();
				delete th[i];
				th[i] = nullptr;
			}
			print_func();
		};
		char buff[buff_size];
		smartStackAllocator<spinLock> stack_allocator(buff);
		lfSmartStackAllocator lf_stack_allocator(buff);
		smartDualStackAllocator<spinLock> dual_stack_allocator(buff);
		lfSmartDualStackAllocator<spinLock> lf_dual_stack_allocator(buff);
		monoAllocator<spinLock> mono_allocator(buff);
		lfMonoAllocator lf_mono_allocator(buff);
		poolAllocator<pool_size, spinLock> pool_allocator(buff, buff_size, alloc_size);
		lfPoolAllocator<pool_size> lf_pool_allocator(buff, buff_size, alloc_size);
		stdAllocator<> std_allocator;
		//stdAllocator<spinLock> std_allocator;
		auto allocator_adapter_impl1 = stack_allocator.adapter();
		auto allocator_adapter_impl2 = lf_stack_allocator.adapter();
		auto allocator_adapter_impl3 = dual_stack_allocator.adapter();
		auto allocator_adapter_impl4 = lf_dual_stack_allocator.adapter();
		auto allocator_adapter_impl5 = mono_allocator.adapter();
		auto allocator_adapter_impl6 = lf_mono_allocator.adapter();
		auto allocator_adapter_impl7 = pool_allocator.adapter();
		auto allocator_adapter_impl8 = std_allocator.adapter();
		IAllocatorAdapter* allocator_adapter = &allocator_adapter_impl8;
		auto alloc_stack = [&stack_allocator](const std::size_t size, const std::size_t align) -> void*
		{
			return stack_allocator.alloc(size, align);
		};
		auto free_stack = [&stack_allocator](void* p)
		{
			stack_allocator.free(p);
		};
		auto print_stack = [&stack_allocator]()
		{
			char message[1024];
			stack_allocator.debugInfo(message);
			printf(message);
		};
		auto alloc_lfstack = [&lf_stack_allocator](const std::size_t size, const std::size_t align) -> void*
		{
			return lf_stack_allocator.alloc(size, align);
		};
		auto free_lfstack = [&lf_stack_allocator](void* p)
		{
			lf_stack_allocator.free(p);
		};
		auto print_lfstack = [&lf_stack_allocator]()
		{
			char message[1024];
			lf_stack_allocator.debugInfo(message);
			printf(message);
		};
		auto alloc_dstack = [&dual_stack_allocator](const std::size_t size, const std::size_t align) -> void*
		{
			dual_stack_allocator.reversewAllocateOrder();
			return dual_stack_allocator.alloc(size, align);
		};
		auto free_dstack = [&dual_stack_allocator](void* p)
		{
			dual_stack_allocator.free(p);
		};
		auto print_dstack = [&dual_stack_allocator]()
		{
			char message[1024];
			dual_stack_allocator.debugInfo(message);
			printf(message);
		};
		auto alloc_lfdstack = [&lf_dual_stack_allocator](const std::size_t size, const std::size_t align) -> void*
		{
			lf_dual_stack_allocator.reversewAllocateOrder();
			return lf_dual_stack_allocator.alloc(size, align);
		};
		auto free_lfdstack = [&lf_dual_stack_allocator](void* p)
		{
			lf_dual_stack_allocator.free(p);
		};
		auto print_lfdstack = [&lf_dual_stack_allocator]()
		{
			char message[1024];
			lf_dual_stack_allocator.debugInfo(message);
			printf(message);
		};
		auto alloc_mono = [&mono_allocator](const std::size_t size, const std::size_t align) -> void*
		{
			return mono_allocator.alloc(size, align);
		};
		auto free_mono = [&mono_allocator](void* p)
		{
			mono_allocator.free(p);
		};
		auto print_mono = [&mono_allocator]()
		{
			char message[1024];
			mono_allocator.debugInfo(message);
			printf(message);
		};
		auto alloc_lfmono = [&lf_mono_allocator](const std::size_t size, const std::size_t align) -> void*
		{
			return lf_mono_allocator.alloc(size, align);
		};
		auto free_lfmono = [&lf_mono_allocator](void* p)
		{
			lf_mono_allocator.free(p);
		};
		auto print_lfmono = [&lf_mono_allocator]()
		{
			char message[1024];
			lf_mono_allocator.debugInfo(message);
			printf(message);
		};
		auto alloc_pool = [&pool_allocator](const std::size_t size, const std::size_t align) -> void*
		{
			return pool_allocator.alloc(size, align);
		};
		auto free_pool = [&pool_allocator](void* p)
		{
			pool_allocator.free(p);
		};
		auto print_pool = [&pool_allocator]()
		{
			char message[1024];
			pool_allocator.debugInfo(message, false);
			printf(message);
		};
		auto alloc_lfpool = [&lf_pool_allocator](const std::size_t size, const std::size_t align) -> void*
		{
			return lf_pool_allocator.alloc(size, align);
		};
		auto free_lfpool = [&lf_pool_allocator](void* p)
		{
			lf_pool_allocator.free(p);
		};
		auto print_lfpool = [&lf_pool_allocator]()
		{
			char message[1024];
			lf_pool_allocator.debugInfo(message, false);
			printf(message);
		};
		auto alloc_global = [&std_allocator](const std::size_t size, const std::size_t align) -> void*
		{
			return std_allocator.alloc(size, align);
		};
		auto free_global = [&std_allocator](void* p)
		{
			std_allocator.free(p);
		};
		auto print_global = [&std_allocator]()
		{
			char message[1024];
			std_allocator.debugInfo(message);
			printf(message);
		};
		auto alloc_adapter = [&allocator_adapter](const std::size_t size, const std::size_t align) -> void*
		{
			return allocator_adapter->alloc(size, align);
		};
		auto free_adapter = [&allocator_adapter](void* p)
		{
			allocator_adapter->free(p);
		};
		auto print_adapter = [&allocator_adapter]()
		{
			printf("[ allocator adapter : %s ]\n", allocator_adapter->name());
			char message[1024];
			allocator_adapter->debugInfo(message);
			printf(message);
		};
		thread_test(alloc_stack, free_stack, print_stack);
		thread_test(alloc_lfstack, free_lfstack, print_lfstack);
		thread_test(alloc_dstack, free_dstack, print_dstack);
		thread_test(alloc_lfdstack, free_lfdstack, print_lfdstack);
		thread_test(alloc_mono, free_mono, print_mono);
		thread_test(alloc_lfmono, free_lfmono, print_lfmono);
		thread_test(alloc_pool, free_pool, print_pool);
		thread_test(alloc_lfpool, free_lfpool, print_lfpool);
		thread_test(alloc_global, free_global, print_global);
		thread_test(alloc_adapter, free_adapter, print_adapter);
	}
#if 0
	lfSmartDualStackAllocator_withBuff<2> al;
	void* pp1 = al.allocOrd(ALLOC_ASC, 1, 1); al.debugInfo(message); printf(message);
	void* pp2 = al.allocOrd(ALLOC_DESC, 1, 1); al.debugInfo(message); printf(message);
	al.free(pp2); al.debugInfo(message); printf(message);
	al.free(pp1); al.debugInfo(message); printf(message);
#endif

	printf("- end -\n");
#endif
}

// End of file
