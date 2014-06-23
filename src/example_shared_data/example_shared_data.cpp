//--------------------------------------------------------------------------------
// exmaple_utility.cpp
// 汎用ユーティリティテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_utility.h"//汎用ユーティリティ

#include <gasha/utility.h>//汎用ユーティリティ

#include <stdio.h>//printf()

//----------------------------------------
//汎用ユーティリティテスト
void example_utility()
{
	{
		//min() / max()関数テスト
		printf("----- Test for min() / max() -----\n");
		#define MIN_MAX(T, FMT, ...) \
			{ const T var = min(__VA_ARGS__); printf("min(" #__VA_ARGS__ ") = " FMT "\n", var); } \
			{ const T var = max(__VA_ARGS__); printf("max(" #__VA_ARGS__ ") = " FMT "\n", var); }

		MIN_MAX(int, "%d", 1, 2);
		MIN_MAX(int, "%d", 2, 1);
		MIN_MAX(int, "%d", 1, 2, 3);
		MIN_MAX(int, "%d", 3, 2, 1);
		MIN_MAX(int, "%d", 2, 3, 1);
		MIN_MAX(int, "%d", 3, 1, 2);

		MIN_MAX(float, "%.1f", 1.1f, 2.2f);
		MIN_MAX(float, "%.1f", 2.2f, 1.1f);
		MIN_MAX(float, "%.1f", 1.1f, 2.2f, 3.3f);
		MIN_MAX(float, "%.1f", 3.3f, 2.2f, 1.1f);
		MIN_MAX(float, "%.1f", 2.2f, 3.3f, 1.1f);
		MIN_MAX(float, "%.1f", 3.3f, 1.1f, 2.2f);

		MIN_MAX(int, "%d", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20);
		MIN_MAX(int, "%d", 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1);
		MIN_MAX(int, "%d", 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 19, 17, 15, 13, 11, 9, 7, 5, 3, 1);
	}
}




//--------------------------------------------------------------------------------
//テスト用データ
struct data_t
{
	int m_temp;
	int m_value;
};

//--------------------------------------------------------------------------------
//通常プールアロケータテスト

static pool_allocator<data_t, TEST_POOL_SIZE> s_poolAllocator;//通常プールアロケータ

//通常プールアロケータからアロケート
data_t* allocNormal()
{
	return s_poolAllocator.newObj();
}
//通常プールアロケータからフリー
bool freeNormal(data_t* value)
{
	return s_poolAllocator.deleteObj(value);
}

//--------------------------------------------------------------------------------
//ロックフリープールアロケータテスト

static lf_pool_allocator<data_t, TEST_POOL_SIZE> s_lfPoolAllocator;//ロックフリープールアロケータ

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
//通常スタックテスト

#ifdef USE_POOL_ALLOCATOR
static stack<data_t, TEST_POOL_SIZE> s_stack;//通常スタック
#else//USE_POOL_ALLOCATOR
static stack<data_t> s_stack;//通常スタック
#endif//USE_POOL_ALLOCATOR

//通常スタックにプッシュ
bool pushNormal(data_t&& data)
{
	return s_stack.push(std::move(data));//スタックにプッシュ
}
bool pushNormal(data_t& data)
{
	return s_stack.push(data);//スタックにプッシュ
}

//通常スタックからポップ
bool popNormal(data_t& data)
{
	return s_stack.pop(data);//スタックからポップ
}

//--------------------------------------------------------------------------------
//ロックフリースタックテスト

#ifdef USE_LF_POOL_ALLOCATOR
static lf_stack<data_t, TEST_POOL_SIZE> s_lfStack;//ロックフリースタック
#else//USE_LF_POOL_ALLOCATOR
static lf_stack<data_t> s_lfStack;//ロックフリースタック
#endif//USE_LF_POOL_ALLOCATOR

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
//通常キューテスト

#ifdef USE_POOL_ALLOCATOR
static queue<data_t, TEST_POOL_SIZE> s_queue;//通常キュー
#else//USE_POOL_ALLOCATOR
static queue<data_t> s_queue;//通常キュー
#endif//USE_POOL_ALLOCATOR

//通常キューにエンキュー
bool enqueueNormal(data_t&& value)
{
	return s_queue.enqueue(std::move(value));
}
bool enqueueNormal(data_t& value)
{
	return s_queue.enqueue(value);
}

//通常キューからデキュー
bool dequeueNormal(data_t& value)
{
	return s_queue.dequeue(value);
}

//--------------------------------------------------------------------------------
//ロックフリーキューテスト

#ifdef USE_LF_POOL_ALLOCATOR
static lf_queue<data_t, TEST_POOL_SIZE> s_lfQueue;//ロックフリーキュー
#else//USE_LF_POOL_ALLOCATOR
static lf_queue<data_t> s_lfQueue;//ロックフリーキュー
#endif//USE_LF_POOL_ALLOCATOR

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

//--------------------------------------------------------------------------------
//テスト

#ifdef ENABLE_EASY_TEST
//簡易テスト
void easyTest()
{
	printf("================================================================================\n");
	printf("[Test for Lock-free pool-allocator/stack/queue]\n");
	
	//プールアロケータのテスト（共通処理）
	auto test_pool_allocator = [](const char* caption, std::function<data_t*()> alloc, std::function<bool(data_t*)> free)
	{
		printf("--------------------------------------------------------------------------------\n");
		printf("[%s:START]\n", caption);
		printf("*Test count                     = %d\n", TEST_COUNT);
		printf("*Allocate and free test threads = %d\n", 1);
		printf("*Memory pool size               = %d\n", TEST_POOL_SIZE);
		const auto begin_time = std::chrono::system_clock::now();
		pool_allocator<data_t, TEST_POOL_SIZE> allocator;
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
		const auto end_time = std::chrono::system_clock::now();
		const auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - begin_time);
		const double elapsed_time = static_cast<double>(duration.count()) / 1000000000.;
		printf("[%s:END] elapsed_time=%.9llf sec\n", caption, elapsed_time);
		printf("--------------------------------------------------------------------------------\n");
	};

	//スタック／キューのテスト（共通処理）
	auto test_stack_queue = [](const char* caption, std::function<bool(data_t&&)> push, std::function<bool(data_t&)> pop)
	{
		printf("--------------------------------------------------------------------------------\n");
		printf("[%s:START]\n", caption);
		printf("*Test count                = %d\n", TEST_COUNT);
		printf("*Push/Enqueue test threads = %d\n", 1);
		printf("*Pop/Dequeue  test threads = %d\n", 1);
		printf("*Memory pool size          = %d\n", TEST_POOL_SIZE);
		const auto begin_time = std::chrono::system_clock::now();
		int count = 0;
		while (true)
		{
			int num = 0;
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
		const auto end_time = std::chrono::system_clock::now();
		const auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - begin_time);
		const double elapsed_time = static_cast<double>(duration.count()) / 1000000000.;
		printf("[%s:END] elapsed_time=%.9llf sec\n", caption, elapsed_time);
		printf("--------------------------------------------------------------------------------\n");
	};

	//デバッグ情報表示用処理
	auto debug_print_info = [](const data_t& data)
	{
		printf("temp=%d, value=%d", data.m_temp, data.m_value);
	};

#ifdef ENABLE_TEST_FOR_NORMAL_POOL_ALLOCATOR
	//通常プールアロケータのテスト
	{
		auto alloc = []() -> data_t*
		{
			return allocNormal();
		};
		auto free = [](data_t* data) -> bool
		{
			return freeNormal(data);
		};
		test_pool_allocator("Normal Pool-allocator", alloc, free);

#ifdef ENABLE_TEST_PRINT_DEBUG_INFO
		s_poolAllocator.printDebugInfo(debug_print_info);
#endif//ENABLE_TEST_PRINT_DEBUG_INFO
	}
#endif//ENABLE_TEST_FOR_NORMAL_POOL_ALLOCATOR

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

#ifdef ENABLE_TEST_FOR_NORMAL_STACK
	//通常スタックのテスト
	{
		auto push = [](data_t&& data) -> bool
		{
			return pushNormal(std::move(data));
		};
		auto pop = [](data_t& data) -> bool
		{
			return popNormal(data);
		};
		test_stack_queue("Normal Stack", push, pop);

#ifdef ENABLE_TEST_PRINT_DEBUG_INFO
		s_stack.printDebugInfo(debug_print_info);
#endif//ENABLE_TEST_PRINT_DEBUG_INFO
	}
#endif//ENABLE_TEST_FOR_NORMAL_STACK

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

#ifdef ENABLE_TEST_FOR_NORMAL_QUEUE
	//通常キューのテスト
	{
		auto push = [](data_t&& data) -> bool
		{
			return enqueueNormal(std::move(data));
		};
		auto pop = [](data_t& data) -> bool
		{
			return dequeueNormal(data);
		};
		test_stack_queue("Normal Queue", push, pop);

#ifdef ENABLE_TEST_PRINT_DEBUG_INFO
		s_queue.printDebugInfo(debug_print_info);
#endif//ENABLE_TEST_PRINT_DEBUG_INFO
	}
#endif//ENABLE_TEST_FOR_NORMAL_QUEUE

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
//スレッドを使ったテスト
#include <thread>//C++11 std::thread
#include <condition_variable>//C++11 std::condition_variable
#include <chrono>//C++11 std::chrono
#include <functional>//C++11 std::function
void thread_test()
{
	printf("================================================================================\n");
	printf("[Test for Lock-free pool-allocator/stack/queue with threads]\n");

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
				const int count = alloc_count.fetch_add(1);
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
		const auto begin_time = std::chrono::system_clock::now();
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
		const auto end_time = std::chrono::system_clock::now();
		const auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - begin_time);
		const double elapsed_time = static_cast<double>(duration.count()) / 1000000000.;
		printf("[%s:END] elapsed_time=%.9llf sec\n", caption, elapsed_time);
		printf("--------------------------------------------------------------------------------\n");
	};

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
				const int count = push_count.fetch_add(1);
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
					const int count = pop_count.fetch_add(1);
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
		const auto begin_time = std::chrono::system_clock::now();
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
		const auto end_time = std::chrono::system_clock::now();
		const auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - begin_time);
		const double elapsed_time = static_cast<double>(duration.count()) / 1000000000.;
		printf("[%s:END] elapsed_time=%.9llf sec\n", caption, elapsed_time);
		printf("--------------------------------------------------------------------------------\n");
	};
	
	//デバッグ情報表示用処理
	auto debug_print_info = [](const data_t& data)
	{
		printf("temp=%d, value=%d", data.m_temp, data.m_value);
	};
	
#ifdef ENABLE_TEST_FOR_NORMAL_POOL_ALLOCATOR
	//通常プールアロケータのテスト
	{
		auto alloc = []() -> data_t*
		{
			return allocNormal();
		};
		auto free = [](data_t* data) -> bool
		{
			return freeNormal(data);
		};
		test_pool_allocator("Normal Pool-allocator", alloc, free);

	#ifdef ENABLE_TEST_PRINT_DEBUG_INFO
		s_poolAllocator.printDebugInfo(debug_print_info);
	#endif//ENABLE_TEST_PRINT_DEBUG_INFO
	}
#endif//ENABLE_TEST_FOR_NORMAL_POOL_ALLOCATOR

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

#ifdef ENABLE_TEST_FOR_NORMAL_STACK
	//通常スタックのテスト
	{
		auto push = [](data_t&& data) -> bool
		{
			return pushNormal(std::move(data));
		};
		auto pop = [](data_t& data) -> bool
		{
			return popNormal(data);
		};
		test_stack_queue("Normal Stack", "PUSH", "POP ", push, pop);
		
	#ifdef ENABLE_TEST_PRINT_DEBUG_INFO
		s_stack.printDebugInfo(debug_print_info);
	#endif//ENABLE_TEST_PRINT_DEBUG_INFO
	}
#endif//ENABLE_TEST_FOR_NORMAL_STACK

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

#ifdef ENABLE_TEST_FOR_NORMAL_QUEUE
	//通常キューのテスト
	{
		auto push = [](data_t&& data) -> bool
		{
			return enqueueNormal(std::move(data));
		};
		auto pop = [](data_t& data) -> bool
		{
			return dequeueNormal(data);
		};
		test_stack_queue("Normal Queue", "ENQUEUE", "DEQUEUE", push, pop);

	#ifdef ENABLE_TEST_PRINT_DEBUG_INFO
		s_queue.printDebugInfo(debug_print_info);
	#endif//ENABLE_TEST_PRINT_DEBUG_INFO
	}
#endif//ENABLE_TEST_FOR_NORMAL_QUEUE

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

//テスト
int main(const int argc, const char* argv[])
{
#ifdef ENABLE_EASY_TEST
	//簡易テスト
	easyTest();
#endif//ENABLE_EASY_TEST

#ifdef ENABLE_THREAD_TEST
	//スレッドを使ったテスト
	thread_test();
#endif//ENABLE_THREAD_TEST

	return EXIT_SUCCESS;
}




#if 0
//テスト処理
{
	int i = 0;
	int* p = &i;
	std::uint32_t tag = 65535;
	GASHA_ taggedPtr<int, 32, 32> a;//タグ=上位32ビット
	GASHA_ taggedPtr<int, 2, 0> b;//タグ=下位2ビット
	GASHA_ taggedPtr<int, 8, -8> c;//タグ=上位8ビット
	a.set(p, tag);
	b.set(p, tag);
	c.set(p, tag);
	printf("p=0x%p, tag=%d\n", p, tag);
	printf("a:value=0x%016llx, ptr=0x%p, tag=%d\n", a.value(), a.ptr(), a.tag());
	printf("b:value=0x%016llx, ptr=0x%p, tag=%d\n", b.value(), b.ptr(), b.tag());
	printf("c:value=0x%016llx, ptr=0x%p, tag=%d\n", c.value(), c.ptr(), c.tag());
}
#endif

// End of file
