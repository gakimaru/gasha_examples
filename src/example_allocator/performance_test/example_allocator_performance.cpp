//--------------------------------------------------------------------------------
// exmaple_allocator_performance.cpp
// アロケータパフォーマンステスト
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_allocator.h"//アロケータテスト

#include <gasha/mono_allocator.h>//単一アロケータ
#include <gasha/lf_mono_allocator.h>//ロックフリー単一アロケータ
#include <gasha/stack_allocator.h>//スタックアロケータ
#include <gasha/lf_stack_allocator.h>//ロックフリースタックアロケータ
#include <gasha/dual_stack_allocator.h>//双方向スタックアロケータ
#include <gasha/lf_dual_stack_allocator.h>//ロックフリー双方向スタックアロケータ
#include <gasha/pool_allocator.h>//プールアロケータ
#include <gasha/lf_pool_allocator.h>//ロックフリープールアロケータ
#include <gasha/std_allocator.h>//標準アロケータ

#include <gasha/scoped_stack_allocator.h>//スコープスタックアロケータ
#include <gasha/scoped_dual_stack_allocator.h>//双方向スコープスタックアロケータ

#include <gasha/poly_allocator.h>//多態アロケータ

#include <gasha/spin_lock.h>//スピンロック

#include <gasha/chrono.h>//時間系ユーティリティ：elapsedTime
#include <gasha/type_traits.h>//型特性ユーティリティ：toStr()

#include <cstdio>//std::printf()

#pragma warning(push)//【VC++】ワーニング設定を退避
#pragma warning(disable: 4530)//【VC++】C4530を抑える
#include <thread>//C++11 std::thread
#include <mutex>//C++11 std::mutex
#pragma warning(pop)//【VC++】ワーニング設定を復元

//【関数／実体定義部】のインクルード
//※明示的なインスタンス化を不要にするために最初にインクルードする
//※通常は、project_last_settings.h にて、GASHA_***_ALLOCATOR_ALLWAYS_TOGETHER_CPP_H を有効化する方が良い。
#include <gasha/mono_allocator.cpp.h>//単一アロケータ
#include <gasha/stack_allocator.cpp.h>//スタックアロケータ
#include <gasha/lf_stack_allocator.cpp.h>//ロックフリースタックアロケータ
#include <gasha/dual_stack_allocator.cpp.h>//双方向スタックアロケータ
#include <gasha/lf_dual_stack_allocator.cpp.h>//ロックフリー双方向スタックアロケータ
#include <gasha/pool_allocator.cpp.h>//プールアロケータ
#include <gasha/lf_pool_allocator.cpp.h>//ロックフリープールアロケータ
#include <gasha/std_allocator.cpp.h>//標準アロケータ

GASHA_USING_NAMESPACE;//ネームスペース使用

//----------------------------------------
//シングルスレッドテスト
template<class ALLOC, class FREE, class SHOW>
static void testSingleThread(const char* caption, ALLOC alloc_func, FREE free_func, SHOW show_func)
{
	std::printf("\n");
	std::printf("------------------------------------------------------------\n");
	std::printf("\"%s\" : %d * times allocate & free\n", caption, ST_TEST_REPEAT_COUNT1 * ST_TEST_REPEAT_COUNT2);
	elapsedTime time;
	for (int i = 0; i < ST_TEST_REPEAT_COUNT1; ++i)
	{
		void* p[1000];
		for (int j = 0; j < ST_TEST_REPEAT_COUNT2; ++j)
		{
			p[j] = alloc_func(10);
		}
		for (int j = 0; j < ST_TEST_REPEAT_COUNT2; ++j)
		{
			free_func(p[j]);
		}
	}
	std::printf("* elapsed time = %.9lf\t", time.now());
	show_func();
}

//----------------------------------------
//シングルスレッドテスト
static void testAllSingleThread()
{
	std::printf("\n");
	std::printf("--------------------------------------------------------------------------------\n");
	std::printf("[ Test for performance of single thread ]\n");
	std::printf("--------------------------------------------------------------------------------\n");

	char buff[1024 * 16];

	//ロックなしスマートスタックアロケータのテスト
	{
		smartStackAllocator<> stack(buff);
		auto alloc = [&stack](const std::size_t size) -> void* { return stack.alloc(size); };
		auto free = [&stack](void* p) { stack.free(p); };
		auto show = [&stack]() { std::printf("maxSize=%d, size=%d, remain=%d, count=%d\n", stack.maxSize(), stack.size(), stack.remain(), stack.count()); };
		testSingleThread("smartStackAllocator<>", alloc, free, show);
	}

	//スピンロック付きスマートスタックアロケータのテスト
	{
		smartStackAllocator<spinLock> stack(buff);
		auto alloc = [&stack](const std::size_t size) -> void* { return stack.alloc(size); };
		auto free = [&stack](void* p) { stack.free(p); };
		auto show = [&stack]() { std::printf("maxSize=%d, size=%d, remain=%d, count=%d\n", stack.maxSize(), stack.size(), stack.remain(), stack.count()); };
		testSingleThread("smartStackAllocator<spinLock>", alloc, free, show);
	}

	//ミューテックス付きスマートスタックアロケータのテスト
	{
		smartStackAllocator<std::mutex> stack(buff);
		auto alloc = [&stack](const std::size_t size) -> void* { return stack.alloc(size); };
		auto free = [&stack](void* p) { stack.free(p); };
		auto show = [&stack]() { std::printf("maxSize=%d, size=%d, remain=%d, count=%d\n", stack.maxSize(), stack.size(), stack.remain(), stack.count()); };
		testSingleThread("smartStackAllocator<std::mutex>", alloc, free, show);
	}

	//ロックフリースマートスタックアロケータのテスト
	{
		lfSmartStackAllocator stack(buff);
		auto alloc = [&stack](const std::size_t size) -> void* { return stack.alloc(size); };
		auto free = [&stack](void* p) { stack.free(p); };
		auto show = [&stack]() { std::printf("maxSize=%d, size=%d, remain=%d, count=%d\n", stack.maxSize(), stack.size(), stack.remain(), stack.count()); };
		testSingleThread("lfSmartStackAllocator", alloc, free, show);
	}

	//ロックなしスマートスタックアロケータ＋スコープスタックアロケータのテスト
	{
		smartStackAllocator<> stack(buff);
		auto scoped_allocator = stack.scopedAllocator();
		auto alloc = [&scoped_allocator](const std::size_t size) -> void* { return scoped_allocator.alloc(size); };
		auto free = [&scoped_allocator](void* p) { scoped_allocator.free(p); };
		auto show = [&scoped_allocator]() { std::printf("maxSize=%d, size=%d, remain=%d, count=%d\n", scoped_allocator.maxSize(), scoped_allocator.size(), scoped_allocator.remain(), scoped_allocator.count()); };
		testSingleThread("smartStackAllocator<>::scopedAllocator", alloc, free, show);
	}

	//ロックなしスマートスタックアロケータ＋多態アロケータのテスト
	{
		smartStackAllocator<> stack(buff);
		auto adapter = stack.adapter();
		polyAllocator poly_allocator(adapter);
		auto alloc = [](const std::size_t size) -> void* { return new char[size]; };
		auto free = [](void* p) { delete[] reinterpret_cast<char*>(p); };
		auto show = [&stack]() { std::printf("maxSize=%d, size=%d, remain=%d, count=%d\n", stack.maxSize(), stack.size(), stack.remain(), stack.count()); };
		testSingleThread("polyAllocator with smartStackAllocator<>::adapter", alloc, free, show);
	}

	//ロックなし双方向スマートスタックアロケータのテスト
	{
		smartDualStackAllocator<> stack(buff);
		auto alloc = [&stack](const std::size_t size) -> void* { stack.reversewAllocateOrder(); return stack.alloc(size); };
		auto free = [&stack](void* p) { stack.free(p); };
		auto show = [&stack]() { std::printf("maxSize=%d, size=%d(ASC=%d,DESC=%d), remain=%d, count=%d(ASC=%d,DESC=%d)\n", stack.maxSize(), stack.size(), stack.sizeAsc(), stack.sizeDesc(), stack.remain(), stack.count(), stack.countAsc(), stack.countDesc()); };
		testSingleThread("smartDualStackAllocator<>", alloc, free, show);
	}

	//スピンロック付き双方向スマートスタックアロケータのテスト
	{
		smartDualStackAllocator<spinLock> stack(buff);
		auto alloc = [&stack](const std::size_t size) -> void* { stack.reversewAllocateOrder(); return stack.alloc(size); };
		auto free = [&stack](void* p) { stack.free(p); };
		auto show = [&stack]() { std::printf("maxSize=%d, size=%d(ASC=%d,DESC=%d), remain=%d, count=%d(ASC=%d,DESC=%d)\n", stack.maxSize(), stack.size(), stack.sizeAsc(), stack.sizeDesc(), stack.remain(), stack.count(), stack.countAsc(), stack.countDesc()); };
		testSingleThread("smartDualStackAllocator<spinLock>", alloc, free, show);
	}

	//ミューテックス付き双方向スマートスタックアロケータのテスト
	{
		smartDualStackAllocator<std::mutex> stack(buff);
		auto alloc = [&stack](const std::size_t size) -> void* { stack.reversewAllocateOrder(); return stack.alloc(size); };
		auto free = [&stack](void* p) { stack.free(p); };
		auto show = [&stack]() { std::printf("maxSize=%d, size=%d(ASC=%d,DESC=%d), remain=%d, count=%d(ASC=%d,DESC=%d)\n", stack.maxSize(), stack.size(), stack.sizeAsc(), stack.sizeDesc(), stack.remain(), stack.count(), stack.countAsc(), stack.countDesc()); };
		testSingleThread("smartDualStackAllocator<std::mutex>", alloc, free, show);
	}

	//ロックフリー双方向スマートスタックアロケータのテスト
	{
		lfSmartDualStackAllocator stack(buff);
		auto alloc = [&stack](const std::size_t size) -> void* { stack.reversewAllocateOrder(); return stack.alloc(size); };
		auto free = [&stack](void* p) { stack.free(p); };
		auto show = [&stack]() { std::printf("maxSize=%d, size=%d(ASC=%d,DESC=%d), remain=%d, count=%d(ASC=%d,DESC=%d)\n", stack.maxSize(), stack.size(), stack.sizeAsc(), stack.sizeDesc(), stack.remain(), stack.count(), stack.countAsc(), stack.countDesc()); };
		testSingleThread("lfSmartDualStackAllocator", alloc, free, show);
	}

	//ロックなしスマート双方向スタックアロケータ＋スコープスタックアロケータのテスト
	{
		smartDualStackAllocator<> stack(buff);
		auto scoped_allocator = stack.scopedAllocator();
		auto alloc = [&scoped_allocator](const std::size_t size) -> void* { return scoped_allocator.alloc(size); };
		auto free = [&scoped_allocator](void* p) { scoped_allocator.free(p); };
		auto show = [&scoped_allocator]() { std::printf("maxSize=%d, size=%d, remain=%d, count=%d\n", scoped_allocator.maxSize(), scoped_allocator.size(), scoped_allocator.remain(), scoped_allocator.count()); };
		testSingleThread("smartDualStackAllocator<>::scopedAllocator", alloc, free, show);
	}

	//ロックなしスマート双方向スタックアロケータ＋スコープ双方向スタックアロケータのテスト
	{
		smartDualStackAllocator<> stack(buff);
		auto scoped_allocator = stack.scopedDualAllocator();
		auto alloc = [&scoped_allocator](const std::size_t size) -> void* { return scoped_allocator.alloc(size); };
		auto free = [&scoped_allocator](void* p) { scoped_allocator.free(p); };
		auto show = [&scoped_allocator]() { std::printf("maxSize=%d, size=%d(ASC=%d,DESC=%d), remain=%d, count=%d(ASC=%d,DESC=%d)\n", scoped_allocator.maxSize(), scoped_allocator.size(), scoped_allocator.sizeAsc(), scoped_allocator.sizeDesc(), scoped_allocator.remain(), scoped_allocator.count(), scoped_allocator.countAsc(), scoped_allocator.countDesc()); };
		testSingleThread("smartDualStackAllocator<>::scopedDualAllocator", alloc, free, show);
	}

	//ロックなしスマート双方向スタックアロケータ＋多態アロケータのテスト
	{
		smartDualStackAllocator<> stack(buff);
		auto adapter = stack.adapter();
		polyAllocator poly_allocator(adapter);
		auto alloc = [](const std::size_t size) -> void* { return new char[size]; };
		auto free = [](void* p) { delete[] reinterpret_cast<char*>(p); };
		auto show = [&stack]() { std::printf("maxSize=%d, size=%d(ASC=%d,DESC=%d), remain=%d, count=%d(ASC=%d,DESC=%d)\n", stack.maxSize(), stack.size(), stack.sizeAsc(), stack.sizeDesc(), stack.remain(), stack.count(), stack.countAsc(), stack.countDesc()); };
		testSingleThread("polyAllocator with smartDualStackAllocator<>::adapter", alloc, free, show);
	}

	//ロックなし単一アロケータのテスト
	{
		monoAllocator<> mono(buff);
		auto alloc = [&mono](const std::size_t size) -> void* { void* p = mono.alloc(size); mono.free(p); return nullptr; };
		auto free = [&mono](void* p) {};
		auto show = [&mono]() { std::printf("maxSize=%d, size=%d, remain=%d, isAllocated=%s\n", mono.maxSize(), mono.size(), mono.remain(), toStr(mono.isAllocated())); };
		testSingleThread("monoAllocator<>", alloc, free, show);
	}

	//スピンロック付き単一アロケータのテスト
	{
		monoAllocator<spinLock> mono(buff);
		auto alloc = [&mono](const std::size_t size) -> void* { void* p = mono.alloc(size); mono.free(p); return nullptr; };
		auto free = [&mono](void* p) {};
		auto show = [&mono]() { std::printf("maxSize=%d, size=%d, remain=%d, isAllocated=%s\n", mono.maxSize(), mono.size(), mono.remain(), toStr(mono.isAllocated())); };
		testSingleThread("monoAllocator<spinLock>", alloc, free, show);
	}

	//ミューテックス付き単一アロケータのテスト
	{
		monoAllocator<std::mutex> mono(buff);
		auto alloc = [&mono](const std::size_t size) -> void* { void* p = mono.alloc(size); mono.free(p); return nullptr; };
		auto free = [&mono](void* p) {};
		auto show = [&mono]() { std::printf("maxSize=%d, size=%d, remain=%d, isAllocated=%s\n", mono.maxSize(), mono.size(), mono.remain(), toStr(mono.isAllocated())); };
		testSingleThread("monoAllocator<std::mutex>", alloc, free, show);
	}

	//ロックフリー単一アロケータのテスト
	{
		lfMonoAllocator mono(buff);
		auto alloc = [&mono](const std::size_t size) -> void* { void* p = mono.alloc(size); mono.free(p); return nullptr; };
		auto free = [&mono](void* p) {};
		auto show = [&mono]() { std::printf("maxSize=%d, size=%d, remain=%d, isAllocated=%s\n", mono.maxSize(), mono.size(), mono.remain(), toStr(mono.isAllocated())); };
		testSingleThread("lfMonoAllocator", alloc, free, show);
	}

	//ロックなし単一アロケータ＋多態アロケータのテスト
	{
		monoAllocator<> mono(buff);
		auto adapter = mono.adapter();
		polyAllocator poly_allocator(adapter);
		auto alloc = [](const std::size_t size) -> void* { char* p = new char[size]; delete[] p; return nullptr; };
		auto free = [](void* p) {};
		auto show = [&mono]() { std::printf("maxSize=%d, size=%d, remain=%d, isAllocated=%s\n", mono.maxSize(), mono.size(), mono.remain(), toStr(mono.isAllocated())); };
		testSingleThread("polyAllocator with monoAllocator<>::adapter", alloc, free, show);
	}

	//ロックなしプールアロケータのテスト
	{
		poolAllocator<1024> pool(buff, sizeof(buff), 16);
		auto alloc = [&pool](const std::size_t size) -> void* { return pool.alloc(size); };
		auto free = [&pool](void* p) { pool.free(p); };
		auto show = [&pool]() { std::printf("maxSize=%d, size=%d, remain=%d, pool=%d/%d\n", pool.maxSize(), pool.size(), pool.remain(), pool.usingPoolSize(), pool.poolSize()); };
		testSingleThread("poolAllocator<1024>", alloc, free, show);
	}

	//スピンロック付きプールアロケータのテスト
	{
		poolAllocator<1024, spinLock> pool(buff, sizeof(buff), 16);
		auto alloc = [&pool](const std::size_t size) -> void* { return pool.alloc(size); };
		auto free = [&pool](void* p) { pool.free(p); };
		auto show = [&pool]() { std::printf("maxSize=%d, size=%d, remain=%d, pool=%d/%d\n", pool.maxSize(), pool.size(), pool.remain(), pool.usingPoolSize(), pool.poolSize()); };
		testSingleThread("poolAllocator<1024, spinLock>", alloc, free, show);
	}

	//ミューテックス付きプールアロケータのテスト
	{
		poolAllocator<1024, std::mutex> pool(buff, sizeof(buff), 16);
		auto alloc = [&pool](const std::size_t size) -> void* { return pool.alloc(size); };
		auto free = [&pool](void* p) { pool.free(p); };
		auto show = [&pool]() { std::printf("maxSize=%d, size=%d, remain=%d, pool=%d/%d\n", pool.maxSize(), pool.size(), pool.remain(), pool.usingPoolSize(), pool.poolSize()); };
		testSingleThread("poolAllocator<1024, std::mutex>", alloc, free, show);
	}

	//ロックフリープールアロケータのテスト
	{
		lfPoolAllocator<1024> pool(buff, sizeof(buff), 16);
		auto alloc = [&pool](const std::size_t size) -> void* { return pool.alloc(size); };
		auto free = [&pool](void* p) { pool.free(p); };
		auto show = [&pool]() { std::printf("maxSize=%d, size=%d, remain=%d, pool=%d/%d\n", pool.maxSize(), pool.size(), pool.remain(), pool.usingPoolSize(), pool.poolSize()); };
		testSingleThread("lfPoolAllocator<1024>", alloc, free, show);
	}

	//ロックなしプールアロケータ＋多態アロケータのテスト
	{
		poolAllocator<1024> pool(buff, sizeof(buff), 32);
		auto adapter = pool.adapter();
		polyAllocator poly_allocator(adapter);
		auto alloc = [](const std::size_t size) -> void* { return new char[size]; };
		auto free = [](void* p) { delete[] reinterpret_cast<char*>(p); };
		auto show = [&pool]() { std::printf("maxSize=%d, size=%d, remain=%d, pool=%d/%d\n", pool.maxSize(), pool.size(), pool.remain(), pool.usingPoolSize(), pool.poolSize()); };
		testSingleThread("polyAllocator with poolAllocator<1024>::adapter", alloc, free, show);
	}

	//ロックなし標準アロケータのテスト
	{
		stdAllocator<> std_allocator;
		auto alloc = [&std_allocator](const std::size_t size) -> void* { return std_allocator.alloc(size); };
		auto free = [&std_allocator](void* p) { std_allocator.free(p); };
		auto show = [&std_allocator]() { std::printf("maxSize=%d, size=%d, remain=%d\n", std_allocator.maxSize(), std_allocator.size(), std_allocator.remain()); };
		testSingleThread("stdAllocator<>", alloc, free, show);
	}

	//スピンロック付き標準アロケータのテスト
	{
		stdAllocator<spinLock> std_allocator;
		auto alloc = [&std_allocator](const std::size_t size) -> void* { return std_allocator.alloc(size); };
		auto free = [&std_allocator](void* p) { std_allocator.free(p); };
		auto show = [&std_allocator]() { std::printf("maxSize=%d, size=%d, remain=%d\n", std_allocator.maxSize(), std_allocator.size(), std_allocator.remain()); };
		testSingleThread("stdAllocator<spinLock>", alloc, free, show);
	}

	//ミューテックス付き標準アロケータのテスト
	{
		stdAllocator<std::mutex> std_allocator;
		auto alloc = [&std_allocator](const std::size_t size) -> void* { return std_allocator.alloc(size); };
		auto free = [&std_allocator](void* p) { std_allocator.free(p); };
		auto show = [&std_allocator]() { std::printf("maxSize=%d, size=%d, remain=%d\n", std_allocator.maxSize(), std_allocator.size(), std_allocator.remain()); };
		testSingleThread("stdAllocator<std::mutex>", alloc, free, show);
	}

	//ロックなし標準アロケータ＋多態アロケータのテスト
	{
		stdAllocator<> std_allocator;
		auto adapter = std_allocator.adapter();
		polyAllocator poly_allocator(adapter);
		auto alloc = [](const std::size_t size) -> void* { return new char[size]; };
		auto free = [](void* p) { delete[] reinterpret_cast<char*>(p); };
		auto show = [&std_allocator]() { std::printf("maxSize=%d, size=%d, remain=%d\n", std_allocator.maxSize(), std_allocator.size(), std_allocator.remain()); };
		testSingleThread("polyAllocator with stdAllocator<>::adapter", alloc, free, show);
	}

	//ロックなしアラインメント対応標準アロケータのテスト
	{
		stdAlignAllocator<> std_allocator;
		auto alloc = [&std_allocator](const std::size_t size) -> void* { return std_allocator.alloc(size); };
		auto free = [&std_allocator](void* p) { std_allocator.free(p); };
		auto show = [&std_allocator]() { std::printf("maxSize=%d, size=%d, remain=%d\n", std_allocator.maxSize(), std_allocator.size(), std_allocator.remain()); };
		testSingleThread("stdAlignAllocator<>", alloc, free, show);
	}

	//スピンロック付きアラインメント対応標準アロケータのテスト
	{
		stdAlignAllocator<spinLock> std_allocator;
		auto alloc = [&std_allocator](const std::size_t size) -> void* { return std_allocator.alloc(size); };
		auto free = [&std_allocator](void* p) { std_allocator.free(p); };
		auto show = [&std_allocator]() { std::printf("maxSize=%d, size=%d, remain=%d\n", std_allocator.maxSize(), std_allocator.size(), std_allocator.remain()); };
		testSingleThread("stdAlignAllocator<spinLock>", alloc, free, show);
	}

	//ミューテックス付きアラインメント対応標準アロケータのテスト
	{
		stdAlignAllocator<std::mutex> std_allocator;
		auto alloc = [&std_allocator](const std::size_t size) -> void* { return std_allocator.alloc(size); };
		auto free = [&std_allocator](void* p) { std_allocator.free(p); };
		auto show = [&std_allocator]() { std::printf("maxSize=%d, size=%d, remain=%d\n", std_allocator.maxSize(), std_allocator.size(), std_allocator.remain()); };
		testSingleThread("stdAlignAllocator<std::mutex>", alloc, free, show);
	}

	//ロックなしアラインメント対応標準アロケータ＋多態アロケータのテスト
	{
		stdAlignAllocator<> std_allocator;
		auto adapter = std_allocator.adapter();
		polyAllocator poly_allocator(adapter);
		auto alloc = [](const std::size_t size) -> void* { return new char[size]; };
		auto free = [](void* p) { delete[] reinterpret_cast<char*>(p); };
		auto show = [&std_allocator]() { std::printf("maxSize=%d, size=%d, remain=%d\n", std_allocator.maxSize(), std_allocator.size(), std_allocator.remain()); };
		testSingleThread("polyAllocator with stdAlignAllocator<>::adapter", alloc, free, show);
	}
}

//----------------------------------------
//マルチスレッドテスト
template<class ALLOC, class FREE, class SHOW>
static void testMultiThreads(const char* caption, ALLOC alloc_func, FREE free_func, SHOW show_func)
{
	std::printf("\n");
	std::printf("------------------------------------------------------------\n");
	std::printf("\"%s\" : %d * times allocate & free\n", caption, MT_TEST_REPEAT_COUNT1 * MT_TEST_REPEAT_COUNT2 * MT_TEST_REPEAT_COUNT3);
	elapsedTime time;
	auto test_thread = [&alloc_func, &free_func]()
	{
		for (int i = 0; i < MT_TEST_REPEAT_COUNT2; ++i)
		{
			std::this_thread::sleep_for(std::chrono::microseconds(1));
			void* p[1000];
			for (int j = 0; j < MT_TEST_REPEAT_COUNT3; ++j)
			{
				p[j] = alloc_func(10);
			}
			std::this_thread::sleep_for(std::chrono::microseconds(1));
			for (int j = 0; j < MT_TEST_REPEAT_COUNT3; ++j)
			{
				free_func(p[j]);
			}
		}
	};
	std::thread* th[MT_TEST_REPEAT_COUNT1];
	for (int i = 0; i < MT_TEST_REPEAT_COUNT1; ++i)
		th[i] = new std::thread(test_thread);
	for (int i = 0; i < MT_TEST_REPEAT_COUNT1; ++i)
	{
		th[i]->join();
		delete th[i];
		th[i] = nullptr;
	}
	std::printf("* elapsed time = %.9lf\t", time.now());
	show_func();
}

//----------------------------------------
//マルチスレッドテスト
static void testAllMultiThreads()
{
	std::printf("\n");
	std::printf("--------------------------------------------------------------------------------\n");
	std::printf("[ Test for performance of multi threada ]\n");
	std::printf("--------------------------------------------------------------------------------\n");

	static char buff[1024 * 16];

	//スピンロック付きスマートスタックアロケータのテスト
	{
		static smartStackAllocator<spinLock> stack(buff);
		auto alloc = [](const std::size_t size) -> void* { return stack.alloc(size); };
		auto free = [](void* p) { stack.free(p); };
		auto show = []() { std::printf("maxSize=%d, size=%d, remain=%d, count=%d\n", stack.maxSize(), stack.size(), stack.remain(), stack.count()); };
		testMultiThreads("smartStackAllocator<spinLock>", alloc, free, show);
	}

	//ミューテックス付きスマートスタックアロケータのテスト
	{
		static smartStackAllocator<std::mutex> stack(buff);
		auto alloc = [](const std::size_t size) -> void* { return stack.alloc(size); };
		auto free = [](void* p) { stack.free(p); };
		auto show = []() { std::printf("maxSize=%d, size=%d, remain=%d, count=%d\n", stack.maxSize(), stack.size(), stack.remain(), stack.count()); };
		testMultiThreads("smartStackAllocator<std::mutex>", alloc, free, show);
	}

	//ロックフリースマートスタックアロケータのテスト
	{
		static lfSmartStackAllocator stack(buff);
		auto alloc = [](const std::size_t size) -> void* { return stack.alloc(size); };
		auto free = [](void* p) { stack.free(p); };
		auto show = []() { std::printf("maxSize=%d, size=%d, remain=%d, count=%d\n", stack.maxSize(), stack.size(), stack.remain(), stack.count()); };
		testMultiThreads("lfSmartStackAllocator", alloc, free, show);
	}

	//スピンロック付き双方向スマートスタックアロケータのテスト
	{
		static smartDualStackAllocator<spinLock> stack(buff);
		auto alloc = [](const std::size_t size) -> void* { stack.reversewAllocateOrder(); return stack.alloc(size); };
		auto free = [](void* p) { stack.free(p); };
		auto show = []() { std::printf("maxSize=%d, size=%d(ASC=%d,DESC=%d), remain=%d, count=%d(ASC=%d,DESC=%d)\n", stack.maxSize(), stack.size(), stack.sizeAsc(), stack.sizeDesc(), stack.remain(), stack.count(), stack.countAsc(), stack.countDesc()); };
		testMultiThreads("smartDualStackAllocator<spinLock>", alloc, free, show);
	}

	//ミューテックス付き双方向スマートスタックアロケータのテスト
	{
		static smartDualStackAllocator<std::mutex> stack(buff);
		auto alloc = [](const std::size_t size) -> void* { stack.reversewAllocateOrder(); return stack.alloc(size); };
		auto free = [](void* p) { stack.free(p); };
		auto show = []() { std::printf("maxSize=%d, size=%d(ASC=%d,DESC=%d), remain=%d, count=%d(ASC=%d,DESC=%d)\n", stack.maxSize(), stack.size(), stack.sizeAsc(), stack.sizeDesc(), stack.remain(), stack.count(), stack.countAsc(), stack.countDesc()); };
		testMultiThreads("smartDualStackAllocator<std::mutex>", alloc, free, show);
	}

	//ロックフリー双方向スマートスタックアロケータのテスト
	{
		static lfSmartDualStackAllocator stack(buff);
		auto alloc = [](const std::size_t size) -> void* { stack.reversewAllocateOrder(); return stack.alloc(size); };
		auto free = [](void* p) { stack.free(p); };
		auto show = []() { std::printf("maxSize=%d, size=%d(ASC=%d,DESC=%d), remain=%d, count=%d(ASC=%d,DESC=%d)\n", stack.maxSize(), stack.size(), stack.sizeAsc(), stack.sizeDesc(), stack.remain(), stack.count(), stack.countAsc(), stack.countDesc()); };
		testMultiThreads("lfSmartDualStackAllocator", alloc, free, show);
	}

	//スピンロック付き単一アロケータのテスト
	{
		static monoAllocator<spinLock> mono(buff);
		auto alloc = [](const std::size_t size) -> void* { void* p = mono.alloc(size); mono.free(p); return nullptr; };
		auto free = [](void* p) {};
		auto show = []() { std::printf("maxSize=%d, size=%d, remain=%d, isAllocated=%s\n", mono.maxSize(), mono.size(), mono.remain(), toStr(mono.isAllocated())); };
		testMultiThreads("monoAllocator<spinLock>", alloc, free, show);
	}

	//ミューテックス付き単一アロケータのテスト
	{
		static monoAllocator<std::mutex> mono(buff);
		auto alloc = [](const std::size_t size) -> void* { void* p = mono.alloc(size); mono.free(p); return nullptr; };
		auto free = [](void* p) {};
		auto show = []() { std::printf("maxSize=%d, size=%d, remain=%d, isAllocated=%s\n", mono.maxSize(), mono.size(), mono.remain(), toStr(mono.isAllocated())); };
		testMultiThreads("monoAllocator<std::mutex>", alloc, free, show);
	}

	//ロックフリー単一アロケータのテスト
	{
		static lfMonoAllocator mono(buff);
		auto alloc = [](const std::size_t size) -> void* { void* p = mono.alloc(size); mono.free(p); return nullptr; };
		auto free = [](void* p) {};
		auto show = []() { std::printf("maxSize=%d, size=%d, remain=%d, isAllocated=%s\n", mono.maxSize(), mono.size(), mono.remain(), toStr(mono.isAllocated())); };
		testMultiThreads("lfMonoAllocator", alloc, free, show);
	}

	//スピンロック付きプールアロケータのテスト
	{
		static poolAllocator<1024, spinLock> pool(buff, sizeof(buff), 16);
		auto alloc = [](const std::size_t size) -> void* { return pool.alloc(size); };
		auto free = [](void* p) { pool.free(p); };
		auto show = []() { std::printf("maxSize=%d, size=%d, remain=%d, pool=%d/%d\n", pool.maxSize(), pool.size(), pool.remain(), pool.usingPoolSize(), pool.poolSize()); };
		testMultiThreads("poolAllocator<1024, spinLock>", alloc, free, show);
	}

	//ミューテックス付きプールアロケータのテスト
	{
		static poolAllocator<1024, std::mutex> pool(buff, sizeof(buff), 16);
		auto alloc = [](const std::size_t size) -> void* { return pool.alloc(size); };
		auto free = [](void* p) { pool.free(p); };
		auto show = []() { std::printf("maxSize=%d, size=%d, remain=%d, pool=%d/%d\n", pool.maxSize(), pool.size(), pool.remain(), pool.usingPoolSize(), pool.poolSize()); };
		testMultiThreads("poolAllocator<1024, std::mutex>", alloc, free, show);
	}

	//ロックフリープールアロケータのテスト
	{
		static lfPoolAllocator<1024> pool(buff, sizeof(buff), 16);
		auto alloc = [](const std::size_t size) -> void* { return pool.alloc(size); };
		auto free = [](void* p) { pool.free(p); };
		auto show = []() { std::printf("maxSize=%d, size=%d, remain=%d, pool=%d/%d\n", pool.maxSize(), pool.size(), pool.remain(), pool.usingPoolSize(), pool.poolSize()); };
		testMultiThreads("lfPoolAllocator<1024>", alloc, free, show);
	}

	//ロックなし標準アロケータのテスト
	{
		static stdAllocator<> std_allocator;
		auto alloc = [](const std::size_t size) -> void* { return std_allocator.alloc(size); };
		auto free = [](void* p) { std_allocator.free(p); };
		auto show = []() { std::printf("maxSize=%d, size=%d, remain=%d\n", std_allocator.maxSize(), std_allocator.size(), std_allocator.remain()); };
		testMultiThreads("stdAllocator<>", alloc, free, show);
	}

	//スピンロック付き標準アロケータのテスト
	{
		static stdAllocator<spinLock> std_allocator;
		auto alloc = [](const std::size_t size) -> void* { return std_allocator.alloc(size); };
		auto free = [](void* p) { std_allocator.free(p); };
		auto show = []() { std::printf("maxSize=%d, size=%d, remain=%d\n", std_allocator.maxSize(), std_allocator.size(), std_allocator.remain()); };
		testMultiThreads("stdAllocator<spinLock>", alloc, free, show);
	}

	//ミューテックス付き標準アロケータのテスト
	{
		static stdAllocator<std::mutex> std_allocator;
		auto alloc = [](const std::size_t size) -> void* { return std_allocator.alloc(size); };
		auto free = [](void* p) { std_allocator.free(p); };
		auto show = []() { std::printf("maxSize=%d, size=%d, remain=%d\n", std_allocator.maxSize(), std_allocator.size(), std_allocator.remain()); };
		testMultiThreads("stdAllocator<std::mutex>", alloc, free, show);
	}

	//ロックなしアラインメント対応標準アロケータのテスト
	{
		static stdAlignAllocator<> std_allocator;
		auto alloc = [](const std::size_t size) -> void* { return std_allocator.alloc(size); };
		auto free = [](void* p) { std_allocator.free(p); };
		auto show = []() { std::printf("maxSize=%d, size=%d, remain=%d\n", std_allocator.maxSize(), std_allocator.size(), std_allocator.remain()); };
		testMultiThreads("stdAlignAllocator<>", alloc, free, show);
	}

	//スピンロック付きアラインメント対応標準アロケータのテスト
	{
		static stdAlignAllocator<spinLock> std_allocator;
		auto alloc = [](const std::size_t size) -> void* { return std_allocator.alloc(size); };
		auto free = [](void* p) { std_allocator.free(p); };
		auto show = []() { std::printf("maxSize=%d, size=%d, remain=%d\n", std_allocator.maxSize(), std_allocator.size(), std_allocator.remain()); };
		testMultiThreads("stdAlignAllocator<spinLock>", alloc, free, show);
	}

	//ミューテックス付きアラインメント対応標準アロケータのテスト
	{
		static stdAlignAllocator<std::mutex> std_allocator;
		auto alloc = [](const std::size_t size) -> void* { return std_allocator.alloc(size); };
		auto free = [](void* p) { std_allocator.free(p); };
		auto show = []() { std::printf("maxSize=%d, size=%d, remain=%d\n", std_allocator.maxSize(), std_allocator.size(), std_allocator.remain()); };
		testMultiThreads("stdAlignAllocator<std::mutex>", alloc, free, show);
	}

	//ロックなしアラインメント対応標準アロケータのテスト
	{
		static stdAlignAllocator<> std_allocator;
		auto alloc = [](const std::size_t size) -> void* { return std_allocator.alloc(size); };
		auto free = [](void* p) { std_allocator.free(p); };
		auto show = []() { std::printf("maxSize=%d, size=%d, remain=%d\n", std_allocator.maxSize(), std_allocator.size(), std_allocator.remain()); };
		testMultiThreads("stdAlignAllocator<>", alloc, free, show);
	}
}

//----------------------------------------
//アロケータパフォーマンステスト
void example_allocator_performance()
{
	std::printf("\n");
	std::printf("================================================================================\n");

#ifdef ENABLE_TEST_FOR_ALLOATOR_PERFORMANCE_ST
	//シングルスレッドテスト
	testAllSingleThread();
#endif//ENABLE_TEST_FOR_ALLOATOR_PERFORMANCE_ST

#ifdef ENABLE_TEST_FOR_ALLOATOR_PERFORMANCE_MT
	//マルチスレッドテスト
	testAllMultiThreads();
#endif//ENABLE_TEST_FOR_ALLOATOR_PERFORMANCE_MT
}

// End of file
