//--------------------------------------------------------------------------------
// exmaple_shared_data.cpp
// マルチスレッド共有データテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_shared_data.h"//マルチスレッド共有データテスト

#include <gasha/shared_pool_allocator.inl>//マルチスレッド共有プールアロケータ【インライン関数／テンプレート関数定義部】
#include <gasha/shared_stack.inl>//マルチスレッド共有スタック【インライン関数／テンプレート関数定義部】
#include <gasha/shared_queue.inl>//マルチスレッド共有キュー【インライン関数／テンプレート関数定義部】

#include <gasha/lf_pool_allocator.inl>//ロックフリープールアロケータ【インライン関数／テンプレート関数定義部】
#include <gasha/lf_stack.inl>//ロックフリースタック【インライン関数／テンプレート関数定義部】
#include <gasha/lf_queue.inl>//ロックフリーキュー【インライン関数／テンプレート関数定義部】

#include <gasha/spin_lock.h>//スピンロック
#include <gasha/lw_spin_lock.h>//サイズ計量スピンロック
#include <gasha/dummy_lock.h>//ダミーロック
#include <gasha/shared_spin_lock.h>//共有スピンロック
#include <gasha/simple_shared_spin_lock.h>//単純共有スピンロック
#include <gasha/unshared_spin_lock.h>//非共有スピンロック
#include <gasha/dummy_shared_lock.h>//ダミー共有ロック

#include <gasha/utility.h>//汎用ユーティリティ：nowTime(), calcElapsedTime()
#include <gasha/type_traits.h>//型特性ユーティリティ：extentof

#include <utility>//C++11 std::move
#include <condition_variable>//C++11 std::condition_variable
#include <chrono>//C++11 std::chrono
#include <stdio.h>//printf()

//【VC++】例外を無効化した状態で <mutex> <thread> <function >をインクルードすると、もしくは、new 演算子を使用すると warning C4530 が出る
//  warning C4530: C++ 例外処理を使っていますが、アンワインド セマンティクスは有効にはなりません。/EHsc を指定してください。
#pragma warning(disable: 4530)//C4530を抑える

#include <mutex>//C++11 std::mutex
#include <thread>//C++11 std::thread
#include <functional>//C++11 std::function

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------------------------------------------------------------------
//マルチスレッド共有プールアロケータテスト

static shared_pool_allocator_t s_poolAllocator;//マルチスレッド共有プールアロケータ

//マルチスレッド共有プールアロケータからアロケート
data_t* allocNormal()
{
	return s_poolAllocator.newObj();
}
//マルチスレッド共有プールアロケータからフリー
bool freeNormal(data_t* value)
{
	return s_poolAllocator.deleteObj(value);
}

//--------------------------------------------------------------------------------
//ロックフリープールアロケータテスト

static lf_pool_allocator_t s_lfPoolAllocator;//ロックフリープールアロケータ

//ロックフリープールアロケータからアロケート
data_t* allocLockFree()
{
	return s_lfPoolAllocator.newObj();
}
//ロックフリープールアロケータからフリー
bool freeLockFree(data_t* value)
{
	return s_lfPoolAllocator.deleteObj(value);
}

//--------------------------------------------------------------------------------
//マルチスレッド共有スタックテスト

static shared_stack_t s_stack;//マルチスレッド共有スタック

//マルチスレッド共有スタックにプッシュ
bool pushNormal(data_t&& data)
{
	return s_stack.push(std::move(data));//スタックにプッシュ
}
bool pushNormal(data_t& data)
{
	return s_stack.push(data);//スタックにプッシュ
}

//マルチスレッド共有スタックからポップ
bool popNormal(data_t& data)
{
	return s_stack.pop(data);//スタックからポップ
}

//--------------------------------------------------------------------------------
//ロックフリースタックテスト

static lf_stack_t s_lfStack;//ロックフリースタック

//ロックフリースタックにプッシュ
bool pushLockFree(data_t&& data)
{
	return s_lfStack.push(std::move(data));//スタックにプッシュ
}
bool pushLockFree(data_t& data)
{
	return s_lfStack.push(data);//スタックにプッシュ
}

//ロックフリースタックからポップ
bool popLockFree(data_t& data)
{
	return s_lfStack.pop(data);//スタックからポップ
}

//--------------------------------------------------------------------------------
//マルチスレッド共有キューテスト

static shared_queue_t s_queue;//マルチスレッド共有キュー

//マルチスレッド共有キューにエンキュー
bool enqueueNormal(data_t&& value)
{
	return s_queue.enqueue(std::move(value));
}
bool enqueueNormal(data_t& value)
{
	return s_queue.enqueue(value);
}

//マルチスレッド共有キューからデキュー
bool dequeueNormal(data_t& value)
{
	return s_queue.dequeue(value);
}

//--------------------------------------------------------------------------------
//ロックフリーキューテスト

static lf_queue_t s_lfQueue;//ロックフリーキュー

//ロックフリーキューにエンキュー
bool enqueueLockFree(data_t&& value)
{
	return s_lfQueue.enqueue(std::move(value));
}
bool enqueueLockFree(data_t& value)
{
	return s_lfQueue.enqueue(value);
}

//ロックフリーキューからデキュー
bool dequeueLockFree(data_t& value)
{
	return	s_lfQueue.dequeue(value);
}

#ifdef ENABLE_TAGGED_PTRTEST
//----------------------------------------
//タグ付きポインタテスト
void taggedPtrTest()
{
	printf("================================================================================\n");
	printf("[Test for taggedPtr]\n");

	int i = 0;
	int* p = &i;
	std::uint32_t tag = 65535;
	GASHA_ taggedPtr<int, 32, 32> a;//タグ=上位32ビット
	GASHA_ taggedPtr<int,  2,  0> b;//タグ=下位2ビット
	GASHA_ taggedPtr<int,  8, -8> c;//タグ=上位8ビット
	a.set(p, tag);
	b.set(p, tag);
	c.set(p, tag);
	printf("p=0x%p, tag=%d\n", p, tag);
	printf("taggedPtr<int, 32, 32>: .value()=0x%016llx, .ptr()=0x%p, .tag()=%d\n", a.value(), a.ptr(), a.tag());
	printf("taggedPtr<int,  2,  0>: .value()=0x%016llx, .ptr()=0x%p, .tag()=%d\n", b.value(), b.ptr(), b.tag());
	printf("taggedPtr<int,  8, -8>: .value()=0x%016llx, .ptr()=0x%p, .tag()=%d\n", c.value(), c.ptr(), c.tag());
}
#endif//ENABLE_TAGGED_PTRTEST

#ifdef ENABLE_EASY_TEST
//----------------------------------------
//簡易テスト
void easyTest()
{
	printf("================================================================================\n");
	printf("[Test for Lock-free pool-allocator/sharedStack/sharedQueue]\n");
	
#if defined(ENABLE_TEST_FOR_SHARED_POOL_ALLOCATOR) || defined(ENABLE_TEST_FOR_LF_POOL_ALLOCATOR)
	//プールアロケータのテスト（共通処理）
	auto test_pool_allocator = [](const char* caption, std::function<data_t*()> alloc, std::function<bool(data_t*)> free)
	{
		printf("--------------------------------------------------------------------------------\n");
		printf("[%s:START]\n", caption);
		printf("*Test count                     = %d\n", TEST_COUNT);
		printf("*Allocate and free test threads = %d\n", 1);
		printf("*Memory pool size               = %d\n", TEST_POOL_SIZE);
		const auto begin_time = nowTime();
		sharedPoolAllocator<data_t, TEST_POOL_SIZE> allocator;
		data_t* data[TEST_POOL_SIZE + 1] = { 0 };
		int count = 0;
		while(true)
		{
			int num = 0;
			while (true)
			{
				data_t*& data_p = data[num++];
				data_p = alloc();
				if (!data_p)
					break;
				data_p->m_temp = 0;
				data_p->m_value = count++;
				if (count > TEST_COUNT)
					break;
			}
			for (int i = 0; i < num; ++i)
			{
				data_t* data_p = data[i];
				if (data_p)
					free(data_p);
			}
			if (count > TEST_COUNT)
				break;
		}
		const double elapsed_time = calcElapsedTime(begin_time);
		printf("[%s:END] elapsed_time=%.9lf sec\n", caption, elapsed_time);
		printf("--------------------------------------------------------------------------------\n");
	};
#endif//defined(ENABLE_TEST_FOR_SHARED_POOL_ALLOCATOR) || defined(ENABLE_TEST_FOR_LF_POOL_ALLOCATOR)

#if defined(ENABLE_TEST_FOR_SHARED_STACK) || defined(ENABLE_TEST_FOR_LF_STACK) || defined(ENABLE_TEST_FOR_SHARED_QUEUE) || defined(ENABLE_TEST_FOR_LF_QUEUE)
	//スタック／キューのテスト（共通処理）
	auto test_stack_queue = [](const char* caption, std::function<bool(data_t&&)> push, std::function<bool(data_t&)> pop)
	{
		printf("--------------------------------------------------------------------------------\n");
		printf("[%s:START]\n", caption);
		printf("*Test count                = %d\n", TEST_COUNT);
		printf("*Push/Enqueue test threads = %d\n", 1);
		printf("*Pop/Dequeue  test threads = %d\n", 1);
		printf("*Memory pool size          = %d\n", TEST_POOL_SIZE);
		const auto begin_time = nowTime();
		int count = 0;
		while (true)
		{
			while (true)
			{
				const bool result = push({ count++ });
				if (!result)
				{
					--count;
					break;
				}
				if (count > TEST_COUNT)
					break;
			}
			while (true)
			{
				data_t data;
				const bool result = pop(data);
				if (!result)
					break;
			}
			if (count > TEST_COUNT)
				break;
		}
		const double elapsed_time = calcElapsedTime(begin_time);
		printf("[%s:END] elapsed_time=%.9lf sec\n", caption, elapsed_time);
		printf("--------------------------------------------------------------------------------\n");
	};
#endif//defined(ENABLE_TEST_FOR_SHARED_STACK) || defined(ENABLE_TEST_FOR_LF_STACK) || defined(ENABLE_TEST_FOR_SHARED_QUEUE) || defined(ENABLE_TEST_FOR_LF_QUEUE)

#ifdef ENABLE_TEST_PRINT_DEBUG_INFO
	//デバッグ情報表示用処理
	auto debug_print_info = [](const data_t& data)
	{
		printf("temp=%d, value=%d", data.m_temp, data.m_value);
	};
#endif//ENABLE_TEST_PRINT_DEBUG_INFO

#ifdef ENABLE_TEST_FOR_SHARED_POOL_ALLOCATOR
	//マルチスレッド共有プールアロケータのテスト
	{
		auto alloc = []() -> data_t*
		{
			return allocNormal();
		};
		auto free = [](data_t* data) -> bool
		{
			return freeNormal(data);
		};
		test_pool_allocator("Shared Pool-allocator", alloc, free);

	#ifdef ENABLE_TEST_PRINT_DEBUG_INFO
		s_poolAllocator.printDebugInfo(debug_print_info);
	#endif//ENABLE_TEST_PRINT_DEBUG_INFO
	}
#endif//ENABLE_TEST_FOR_SHARED_POOL_ALLOCATOR

#ifdef ENABLE_TEST_FOR_LF_POOL_ALLOCATOR
	//ロックフリープールアロケータのテスト
	{
		auto alloc = []() -> data_t*
		{
			return allocLockFree();
		};
		auto free = [](data_t* data) -> bool
		{
			return freeLockFree(data);
		};
		test_pool_allocator("Lock-Free Pool-allocator", alloc, free);

	#ifdef ENABLE_TEST_PRINT_DEBUG_INFO
		s_lfPoolAllocator.printDebugInfo(debug_print_info);
	#endif//ENABLE_TEST_PRINT_DEBUG_INFO
	}
#endif//ENABLE_TEST_FOR_LF_POOL_ALLOCATOR

#ifdef ENABLE_TEST_FOR_SHARED_STACK
	//マルチスレッド共有スタックのテスト
	{
		auto push = [](data_t&& data) -> bool
		{
			return pushNormal(std::move(data));
		};
		auto pop = [](data_t& data) -> bool
		{
			return popNormal(data);
		};
		test_stack_queue("Shared Stack", push, pop);

	#ifdef ENABLE_TEST_PRINT_DEBUG_INFO
		s_stack.printDebugInfo(debug_print_info);
	#endif//ENABLE_TEST_PRINT_DEBUG_INFO
	}
#endif//ENABLE_TEST_FOR_SHARED_STACK

#ifdef ENABLE_TEST_FOR_LF_STACK
	//ロックフリースタックのテスト
	{
		auto push = [](data_t&& data) -> bool
		{
			return pushLockFree(std::move(data));
		};
		auto pop = [](data_t& data) -> bool
		{
			return popLockFree(data);
		};
		test_stack_queue("Lock-Free Stack", push, pop);

	#ifdef ENABLE_TEST_PRINT_DEBUG_INFO
		s_lfStack.printDebugInfo(debug_print_info);
	#endif//ENABLE_TEST_PRINT_DEBUG_INFO
	}
#endif//ENABLE_TEST_FOR_LF_STACK

#ifdef ENABLE_TEST_FOR_SHARED_QUEUE
	//マルチスレッド共有キューのテスト
	{
		auto push = [](data_t&& data) -> bool
		{
			return enqueueNormal(std::move(data));
		};
		auto pop = [](data_t& data) -> bool
		{
			return dequeueNormal(data);
		};
		test_stack_queue("Shared Queue", push, pop);

	#ifdef ENABLE_TEST_PRINT_DEBUG_INFO
		s_queue.printDebugInfo(debug_print_info);
	#endif//ENABLE_TEST_PRINT_DEBUG_INFO
	}
#endif//ENABLE_TEST_FOR_SHARED_QUEUE

#ifdef ENABLE_TEST_FOR_LF_QUEUE
	//ロックフリーキューのテスト
	{
		auto push = [](data_t&& data) -> bool
		{
			return enqueueLockFree(std::move(data));
		};
		auto pop = [](data_t& data) -> bool
		{
			return dequeueLockFree(data);
		};
		test_stack_queue("Lock-Free Queue", push, pop);

	#ifdef ENABLE_TEST_PRINT_DEBUG_INFO
		s_lfQueue.printDebugInfo(debug_print_info);
	#endif//ENABLE_TEST_PRINT_DEBUG_INFO
	}
#endif//ENABLE_TEST_FOR_LF_QUEUE
}
#endif//ENABLE_EASY_TEST

#ifdef ENABLE_THREAD_TEST
//----------------------------------------
//スレッドを使ったテスト
void thread_test()
{
	printf("================================================================================\n");
	printf("[Test for Lock-free pool-allocator/sharedStack/sharedQueue with threads]\n");

#if defined(ENABLE_TEST_FOR_SHARED_POOL_ALLOCATOR) || defined(ENABLE_TEST_FOR_LF_POOL_ALLOCATOR)
	//プールアロケータのテスト（共通処理）
	auto test_pool_allocator = [](const char* caption, std::function<data_t*()> alloc, std::function<bool(data_t*)> free)
	{
		printf("--------------------------------------------------------------------------------\n");
		printf("[%s:START]\n", caption);
		printf("*Test count                     = %d\n", TEST_COUNT);
		printf("*Allocate and free test threads = %d\n", TEST_ALLOC_THREADS);
		printf("*Memory pool size               = %d\n", TEST_POOL_SIZE);

		std::condition_variable cond;
		std::mutex mutex;
		std::atomic<bool> is_ready;
		is_ready.store(false);
		auto isReady = [&is_ready]() -> bool
		{
			return is_ready.load();
		};
		std::atomic<int> wait_count;
		std::atomic<int> start_count;
		std::atomic<int> end_count;
		std::atomic<int> alloc_count;
		wait_count.store(0);
		start_count.store(0);
		end_count.store(0);
		alloc_count.store(0);
		auto threadAlloc = [&alloc, &free, &cond, &mutex, &isReady, &wait_count, &start_count, &end_count, &alloc_count](const int thread_no)
		{
			wait_count.fetch_add(1);
		#ifdef ENABLE_TEST_PRINT
			printf("[Alloc:%2d]:Waiting...\n", thread_no);
		#endif//ENABLE_TEST_PRINT
			{
				std::unique_lock<std::mutex> lock(mutex);
				cond.wait(lock, isReady);
			}

			start_count.fetch_add(1);
		#ifdef ENABLE_TEST_PRINT
			printf("[Alloc:%2d]:Start\n", thread_no);
		#endif//ENABLE_TEST_PRINT

			int loop_count = 0;
			int my_alloc_count = 0;
			while (true)
			{
				if (alloc_count.load() > TEST_COUNT)
					break;
				++loop_count;
				if (loop_count % 1000 == 0)
					std::this_thread::sleep_for(std::chrono::microseconds(0));
				data_t* obj = alloc();
				if (!obj)
					continue;
			#ifdef ENABLE_TEST_PRINT
				const int count = 
			#endif//ENABLE_TEST_PRINT
					alloc_count.fetch_add(1);
				std::this_thread::sleep_for(std::chrono::microseconds(0));
				free(obj);
				++my_alloc_count;
				if (my_alloc_count % 1000 == 0)
					std::this_thread::sleep_for(std::chrono::microseconds(1));
			#ifdef ENABLE_TEST_PRINT
				if (count % TEST_PRINT_STEP == 0)
					printf("[Alloc:%2d] alloc=%d\n", thread_no, count);
			#endif//ENABLE_TEST_PRINT
				//std::this_thread::yield();
				std::this_thread::sleep_for(std::chrono::microseconds(0));
			}

			end_count.fetch_add(1);
		#ifdef ENABLE_TEST_PRINT
			printf("[Alloc:%2d]:End\n", thread_no);
		#endif//ENABLE_TEST_PRINT
		};
		std::thread* th[TEST_ALLOC_THREADS];
		int thread_count = 0;
		{
			std::lock_guard<std::mutex> lock(mutex);
			for (int i = 0; i < static_cast<int>(extentof(th)); ++i)
			{
				th[i] = new std::thread(threadAlloc, i);
				++thread_count;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		const auto begin_time = nowTime();
		{
			is_ready = true;
			while (true)
			{
				cond.notify_all();//きちんと全スレッドに通知できない場合があるのでリトライする
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				if (start_count.load() == thread_count)
					break;
			}
		}
		{
			for (int i = 0; i < static_cast<int>(extentof(th)); ++i)
			{
				th[i]->join();
				delete th[i];
				th[i] = nullptr;
			}
		}
		const double elapsed_time = calcElapsedTime(begin_time);
		printf("[%s:END] elapsed_time=%.9lf sec\n", caption, elapsed_time);
		printf("--------------------------------------------------------------------------------\n");
	};
#endif//defined(ENABLE_TEST_FOR_SHARED_POOL_ALLOCATOR) || defined(ENABLE_TEST_FOR_LF_POOL_ALLOCATOR)

#if defined(ENABLE_TEST_FOR_SHARED_STACK) || defined(ENABLE_TEST_FOR_LF_STACK) || defined(ENABLE_TEST_FOR_SHARED_QUEUE) || defined(ENABLE_TEST_FOR_LF_QUEUE)
	//スタック／キューのテスト（共通処理）
	auto test_stack_queue = [](const char* caption, const char* push_name, const char* pop_name, std::function<bool(data_t&&)> push, std::function<bool(data_t&)> pop)
	{
		printf("--------------------------------------------------------------------------------\n");
		printf("[%s:START]\n", caption);
		printf("*Test count                = %d\n", TEST_COUNT);
		printf("*Push/Enqueue test threads = %d\n", TEST_PUSH_THREADS);
		printf("*Pop/Dequeue  test threads = %d\n", TEST_POP_THREADS);
		printf("*Memory pool size          = %d\n", TEST_POOL_SIZE);

		std::condition_variable cond;
		std::mutex mutex;
		std::atomic<bool> is_ready;
		is_ready.store(false);
		auto isReady = [&is_ready]() -> bool
		{
			return is_ready.load();
		};
		std::atomic<int> wait_count;
		std::atomic<int> start_count;
		std::atomic<int> end_count;
		std::atomic<int> push_count;
		std::atomic<int> pop_count;
		std::atomic<int> seq_no;
		wait_count.store(0);
		start_count.store(0);
		end_count.store(0);
		push_count.store(0);
		pop_count.store(0);
		seq_no.store(0);
		auto threadPush = [&push_name, &push, &cond, &mutex, &isReady, &wait_count, &start_count, &end_count, &push_count, &seq_no](const int thread_no)
		{
			wait_count.fetch_add(1);
		#ifdef ENABLE_TEST_PRINT
			printf("[%s:%2d]:Waiting...\n", push_name, thread_no);
		#endif//ENABLE_TEST_PRINT
			{
				std::unique_lock<std::mutex> lock(mutex);
				cond.wait(lock, isReady);
			}
			
			start_count.fetch_add(1);
		#ifdef ENABLE_TEST_PRINT
			printf("[%s:%2d]:Start\n", push_name, thread_no);
		#endif//ENABLE_TEST_PRINT

			int loop_count = 0;
			int my_push_count = 0;
			while (true)
			{
				if (push_count.load() > TEST_COUNT)
					break;
				++loop_count;
				if (loop_count % 1000 == 0)
					std::this_thread::sleep_for(std::chrono::microseconds(0));
				const int no = seq_no.fetch_add(1);
				if (no > TEST_COUNT)
				{
					seq_no.fetch_sub(1);
					continue;
				}
				const bool result = push({ 0, no });
				if (!result)
				{
					seq_no.fetch_sub(1);
					continue;
				}
			#ifdef ENABLE_TEST_PRINT
				const int count = 
			#endif//ENABLE_TEST_PRINT
					push_count.fetch_add(1);
				++my_push_count;
				if (my_push_count % 1000 == 0)
					std::this_thread::sleep_for(std::chrono::microseconds(1));
			#ifdef ENABLE_TEST_PRINT
				if (count % TEST_PRINT_STEP == 0)
					printf("[%s:%2d] count=%d\n", push_name, thread_no, count);
			#endif//ENABLE_TEST_PRINT
				//std::this_thread::yield();
				std::this_thread::sleep_for(std::chrono::microseconds(0));
			}
			
			end_count.fetch_add(1);
		#ifdef ENABLE_TEST_PRINT
			printf("[%s:%2d]:End\n", push_name, thread_no);
		#endif//ENABLE_TEST_PRINT
		};
		auto threadPop = [&pop_name, &pop, &cond, &mutex, &isReady, &wait_count, &start_count, &end_count, &pop_count](const int thread_no)
		{
			wait_count.fetch_add(1);
		#ifdef ENABLE_TEST_PRINT
			printf("[%s:%2d]:Waiting...\n", pop_name, thread_no);
		#endif//ENABLE_TEST_PRINT
			{
				std::unique_lock<std::mutex> lock(mutex);
				cond.wait(lock, isReady);
			}

			start_count.fetch_add(1);
		#ifdef ENABLE_TEST_PRINT
			printf("[%s:%2d]:Start\n", pop_name, thread_no);
		#endif//ENABLE_TEST_PRINT

			while (pop_count.load() <= TEST_COUNT)
			{
				data_t data;
				if (pop(data))
				{
				#ifdef ENABLE_TEST_PRINT
					const int count = 
				#endif//ENABLE_TEST_PRINT
						pop_count.fetch_add(1);
				#ifdef ENABLE_TEST_PRINT
					if (TEST_PRINT_STEP > 0 && count % TEST_PRINT_STEP == 0)
						printf("[%s:%2d] count=%d\n", pop_name, thread_no, count);
				#endif//ENABLE_TEST_PRINT
				}
				//std::this_thread::yield();
				std::this_thread::sleep_for(std::chrono::microseconds(0));
			}

			end_count.fetch_add(1);
		#ifdef ENABLE_TEST_PRINT
			printf("[%s:%2d]:End\n", pop_name, thread_no);
		#endif//ENABLE_TEST_PRINT
		};
		std::thread* th1[TEST_PUSH_THREADS];
		std::thread* th2[TEST_POP_THREADS];
		int thread_count = 0;
		{
			std::lock_guard<std::mutex> lock(mutex);
			for (int i = 0; i < static_cast<int>(extentof(th1)); ++i)
			{
				th1[i] = new std::thread(threadPush, i);
				++thread_count;
			}
			for (int i = 0; i < static_cast<int>(extentof(th2)); ++i)
			{
				th2[i] = new std::thread(threadPop, i);
				++thread_count;
			}
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
		const auto begin_time = nowTime();
		{
			is_ready = true;
			while (true)
			{
				cond.notify_all();//きちんと全スレッドに通知できない場合があるのでリトライする
				std::this_thread::sleep_for(std::chrono::microseconds(1));
				if (start_count.load() == thread_count)
					break;
			}
		}
		{
			for (int i = 0; i < static_cast<int>(extentof(th1)); ++i)
			{
				th1[i]->join();
				delete th1[i];
				th1[i] = nullptr;
			}
			for (int i = 0; i < static_cast<int>(extentof(th2)); ++i)
			{
				th2[i]->join();
				delete th2[i];
				th2[i] = nullptr;
			}
		}
		const double elapsed_time = calcElapsedTime(begin_time);
		printf("[%s:END] elapsed_time=%.9lf sec\n", caption, elapsed_time);
		printf("--------------------------------------------------------------------------------\n");
	};
#endif//defined(ENABLE_TEST_FOR_SHARED_STACK) || defined(ENABLE_TEST_FOR_LF_STACK) || defined(ENABLE_TEST_FOR_SHARED_QUEUE) || defined(ENABLE_TEST_FOR_LF_QUEUE)

#ifdef ENABLE_TEST_PRINT_DEBUG_INFO
	//デバッグ情報表示用処理
	auto debug_print_info = [](const data_t& data)
	{
		printf("temp=%d, value=%d", data.m_temp, data.m_value);
	};
#endif//ENABLE_TEST_PRINT_DEBUG_INFO

#ifdef ENABLE_TEST_FOR_SHARED_POOL_ALLOCATOR
	//マルチスレッド共有プールアロケータのテスト
	{
		auto alloc = []() -> data_t*
		{
			return allocNormal();
		};
		auto free = [](data_t* data) -> bool
		{
			return freeNormal(data);
		};
		test_pool_allocator("Shared Pool-allocator", alloc, free);

	#ifdef ENABLE_TEST_PRINT_DEBUG_INFO
		s_poolAllocator.printDebugInfo(debug_print_info);
	#endif//ENABLE_TEST_PRINT_DEBUG_INFO
	}
#endif//ENABLE_TEST_FOR_SHARED_POOL_ALLOCATOR

#ifdef ENABLE_TEST_FOR_LF_POOL_ALLOCATOR
	//ロックフリープールアロケータのテスト
	{
		auto alloc = []() -> data_t*
		{
			return allocLockFree();
		};
		auto free = [](data_t* data) -> bool
		{
			return freeLockFree(data);
		};
		test_pool_allocator("Lock-Free Pool-allocator", alloc, free);

	#ifdef ENABLE_TEST_PRINT_DEBUG_INFO
		s_lfPoolAllocator.printDebugInfo(debug_print_info);
	#endif//ENABLE_TEST_PRINT_DEBUG_INFO
	}
#endif//ENABLE_TEST_FOR_LF_POOL_ALLOCATOR

#ifdef ENABLE_TEST_FOR_SHARED_STACK
	//マルチスレッド共有スタックのテスト
	{
		auto push = [](data_t&& data) -> bool
		{
			return pushNormal(std::move(data));
		};
		auto pop = [](data_t& data) -> bool
		{
			return popNormal(data);
		};
		test_stack_queue("Shared Stack", "PUSH", "POP ", push, pop);
		
	#ifdef ENABLE_TEST_PRINT_DEBUG_INFO
		s_stack.printDebugInfo(debug_print_info);
	#endif//ENABLE_TEST_PRINT_DEBUG_INFO
	}
#endif//ENABLE_TEST_FOR_SHARED_STACK

#ifdef ENABLE_TEST_FOR_LF_STACK
	//ロックフリースタックのテスト
	{
		auto push = [](data_t&& data) -> bool
		{
			return pushLockFree(std::move(data));
		};
		auto pop = [](data_t& data) -> bool
		{
			return popLockFree(data);
		};
		test_stack_queue("Lock-Free Stack", "PUSH", "POP ", push, pop);

	#ifdef ENABLE_TEST_PRINT_DEBUG_INFO
		s_lfStack.printDebugInfo(debug_print_info);
	#endif//ENABLE_TEST_PRINT_DEBUG_INFO
	}
#endif//ENABLE_TEST_FOR_LF_STACK

#ifdef ENABLE_TEST_FOR_SHARED_QUEUE
	//マルチスレッド共有キューのテスト
	{
		auto push = [](data_t&& data) -> bool
		{
			return enqueueNormal(std::move(data));
		};
		auto pop = [](data_t& data) -> bool
		{
			return dequeueNormal(data);
		};
		test_stack_queue("Shared Queue", "ENQUEUE", "DEQUEUE", push, pop);

	#ifdef ENABLE_TEST_PRINT_DEBUG_INFO
		s_queue.printDebugInfo(debug_print_info);
	#endif//ENABLE_TEST_PRINT_DEBUG_INFO
	}
#endif//ENABLE_TEST_FOR_SHARED_QUEUE

#ifdef ENABLE_TEST_FOR_LF_QUEUE
	//ロックフリーキューのテスト
	{
		auto push = [](data_t&& data) -> bool
		{
			return enqueueLockFree(std::move(data));
		};
		auto pop = [](data_t& data) -> bool
		{
			return dequeueLockFree(data);
		};
		test_stack_queue("Lock-Free Queue", "ENQUEUE", "DEQUEUE", push, pop);

	#ifdef ENABLE_TEST_PRINT_DEBUG_INFO
		s_lfQueue.printDebugInfo(debug_print_info);
	#endif//ENABLE_TEST_PRINT_DEBUG_INFO
	}
#endif//ENABLE_TEST_FOR_LF_QUEUE
}
#endif//ENABLE_THREAD_TEST

//----------------------------------------
//一通りのスコープロック実行テスト
#include <gasha/spin_lock.h>//スピンロック
#include <gasha/lw_spin_lock.h>//サイズ計量スピンロック
#include <gasha/dummy_lock.h>//ダミーロック
#include <mutex>//C++11 std::mutex
#include <gasha/shared_spin_lock.h>//共有スピンロック
#include <gasha/simple_shared_spin_lock.h>//単純共有スピンロック
#include <gasha/unshared_spin_lock.h>//非共有スピンロック
#include <gasha/dummy_shared_lock.h>//ダミー共有ロック
template<class T>
void testScopedLock()
{
	T lock;
	{
		auto lk = lock.lockScoped();
	}
	{
		auto lk = lock.lockUnique();
	}
	{
		auto lk = lock.lockUnique(with_lock);
	}
	{
		auto lk = lock.lockUnique(try_to_lock);
	}
	{
		lock.lock();
		auto lk = lock.lockUnique(adopt_lock);
	}
	{
		auto lk = lock.lockUnique(defer_lock);
	}
}
template<class T>
void testScopedLockStandard()
{
	T lock;
	{
		GASHA_ lock_guard<T> lk(lock);
	}
	{
		GASHA_ unique_lock<T> lk(lock);
	}
	{
		GASHA_ unique_lock<T> lk(lock, with_lock);
	}
	{
		GASHA_ unique_lock<T> lk(lock, try_to_lock);
	}
	{
		lock.lock();
		GASHA_ unique_lock<T> lk(lock, adopt_lock);
	}
	{
		GASHA_ unique_lock<T> lk(lock, defer_lock);
	}
}
template<class T>
void testScopedSharedLock()
{
	T lock;
	{
		auto lk = lock.lockScoped();
	}
	{
		auto lk = lock.lockSharedScoped();
	}
	{
		auto lk = lock.lockUnique();
	}
	{
		auto lk = lock.lockUnique(with_lock);
	}
	{
		auto lk = lock.lockUnique(with_lock_shared);
	}
	{
		auto lk = lock.lockUnique(try_to_lock);
	}
	{
		auto lk = lock.lockUnique(try_to_lock_shared);
	}
	{
		lock.lock();
		auto lk = lock.lockUnique(adopt_lock);
	}
	{
		lock.lock_shared();
		auto lk = lock.lockUnique(adopt_shared_lock);
	}
	{
		auto lk = lock.lockUnique(defer_lock);
	}
}

//----------------------------------------
//マルチスレッド共有データテスト
void example_shared_data()
{
	printf("----- Basin information -----\n");
	printf("alignof(data_t)=%d\n", alignof(data_t));
	printf("calcStaticMSB<alignof(data_t)>::value=%d\n", calcStaticMSB<alignof(data_t)>::value);
	printf("sizeof(lf_stack_t)=%d\n", sizeof(lf_stack_t));
	printf("alignof(lf_stack_t)=%d\n", alignof(lf_stack_t));
	printf("sizeof(lf_stack_t::stack_t)=%d\n", sizeof(lf_stack_t::stack_t));
	printf("alignof(lf_stack_t::stack_t)=%d\n", alignof(lf_stack_t::stack_t));
	printf("lf_stack_t::TAGGED_PTR_TAG_BITS=%d\n", lf_stack_t::TAGGED_PTR_TAG_BITS);
	printf("lf_stack_t::TAGGED_PTR_TAG_SHIFT=%d\n", lf_stack_t::TAGGED_PTR_TAG_SHIFT);
	printf("lf_stack_t::stack_ptr_t::TAG_BITS=%d\n", lf_stack_t::stack_ptr_t::TAG_BITS);
	printf("lf_stack_t::stack_ptr_t::TAG_SHIFT=%d\n", lf_stack_t::stack_ptr_t::TAG_SHIFT);
	printf("sizeof(lf_queue_t)=%d\n", sizeof(lf_queue_t));
	printf("alignof(lf_queue_t)=%d\n", alignof(lf_queue_t));
	printf("sizeof(lf_queue_t::queue_t)=%d\n", sizeof(lf_queue_t::queue_t));
	printf("alignof(lf_queue_t::queue_t)=%d\n", alignof(lf_queue_t::queue_t));
	printf("lf_queue_t::TAGGED_PTR_TAG_BITS=%d\n", lf_queue_t::TAGGED_PTR_TAG_BITS);
	printf("lf_queue_t::TAGGED_PTR_TAG_SHIFT=%d\n", lf_queue_t::TAGGED_PTR_TAG_SHIFT);
	printf("lf_queue_t::queue_ptr_t::TAG_BITS=%d\n", lf_queue_t::queue_ptr_t::TAG_BITS);
	printf("lf_queue_t::queue_ptr_t::TAG_SHIFT=%d\n", lf_queue_t::queue_ptr_t::TAG_SHIFT);
	printf("-----------------------------\n");
	printf("\n");

#ifdef ENABLE_TAGGED_PTRTEST
	//タグ付きポインタテスト
	taggedPtrTest();
#endif//ENABLE_TAGGED_PTRTEST

#ifdef ENABLE_EASY_TEST
	//簡易テスト
	easyTest();
#endif//ENABLE_EASY_TEST

#ifdef ENABLE_THREAD_TEST
	//スレッドを使ったテスト
	thread_test();
#endif//ENABLE_THREAD_TEST

	//一通りのスコープロックテスト
	//※すべてコンパイルが通って、実行時には素通りすればOK
	//※特に何も結果を表示しない
	testScopedLock<spinLock>();
	testScopedLock<lwSpinLock>();
	testScopedLock<dummyLock>();
	testScopedLockStandard<std::mutex>();
	testScopedSharedLock<sharedSpinLock>();
	testScopedSharedLock<simpleSharedSpinLock>();
	testScopedSharedLock<unsharedSpinLock>();
	testScopedSharedLock<dummySharedLock>();
}

// End of file
