﻿//--------------------------------------------------------------------------------
// exmaple_singleton.cpp
// シングルトンテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_singleton.h"//シングルトンテスト

#include <gasha/singleton.h>//シングルトン

#include <gasha/type_traits.h>//型特性ユーティリティ：extentof
#include <gasha/thread_id.h>//スレッドID

#include <stdio.h>//printf()

//【VC++】sprintf を使用すると、error C4996 が発生する
//  error C4996: 'sprintf': This function or variable may be unsafe. Consider using strncpy_fast_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.
#pragma warning(disable: 4996)//C4996を抑える

//【VC++】例外を無効化した状態で <thread> をインクルードすると、warning C4530 が発生する
//  warning C4530: C++ 例外処理を使っていますが、アンワインド セマンティクスは有効にはなりません。/EHsc を指定してください。
#pragma warning(disable: 4530)//C4530を抑える

#include <thread>//C++11 std::thread

GASHA_USING_NAMESPACE;//ネームスペース使用

//----------------------------------------
//シングルトン対象クラス①

//メソッド
void common_data1_t::printInfo()
{
	printf("common_data1: val1=%d, val2=%d\n", m_val1, m_val2);
}

//コンストラクタ
common_data1_t::common_data1_t(const int val1, const int val2) :
	m_val1(val1),
	m_val2(val2)
{
#ifdef TEST_DATA_WATCH_CONSTRUCTOR
	printf("common_data1_t::constructor(%d, %d)\n", val1, val2);
#endif//TEST_DATA_WATCH_CONSTRUCTOR
}

//デストラクタ
common_data1_t::~common_data1_t()
{
#ifdef TEST_DATA_WATCH_CONSTRUCTOR
	printf("common_data1_t::constructor(): m_val1=%d, m_val2=%d\n", m_val1, m_val2);
#endif//TEST_DATA_WATCH_CONSTRUCTOR
}

//----------------------------------------
//シングルトン対象クラス②

//メソッド
void common_data2_t::printInfo()
{
	printf("common_data2: val1=%d, val2=%d\n", m_val1, m_val2);
}

//コンストラクタ
common_data2_t::common_data2_t() :
	m_val1(0),
	m_val2(0)
{
#ifdef TEST_DATA_WATCH_CONSTRUCTOR
	printf("common_data2_t::constructor(): m_val1=%d, m_val2=%d\n", m_val1, m_val2);
#endif//TEST_DATA_WATCH_CONSTRUCTOR
}

//デストラクタ
common_data2_t::~common_data2_t()
{
#ifdef TEST_DATA_WATCH_CONSTRUCTOR
	printf("common_data2_t::constructor(): m_val1=%d, m_val2=%d\n", m_val1, m_val2);
#endif//TEST_DATA_WATCH_CONSTRUCTOR
}

//----------------------------------------
//シングルトンテスト
void example_singleton()
{
	printf("----- Test for singleton -----\n");

	//シングルトンインスタンス生成
	{
		printf("[ Create Singleton Instance : common_data1_t ]\n");
		singleton<common_data1_t> data("test1-A", with_lock_shared);
		data.create("test1:create", 1, 2);//明示的インスタンス生成
	}
	{
		printf("[ Create Singleton Instance : common_data2_t ]\n");
		simpleSingleton<common_data2_t> data("test2-A");//暗黙的インスタンス生成
		data->val1() = 3;
		data->val2() = 4;
	}
	
	//シングルトンアクセス
	{
		printf("[ Access Singleton : common_data1_t(1) ]\n");
		singleton<common_data1_t> data("test1-B");
		data->printInfo();
	}
	{
		printf("[ Access Singleton : common_data2_t(1) ]\n");
		simpleSingleton<common_data2_t> data("test2-B");
		data->printInfo();
	}

	//シングルトンインスタンス破棄
	{
		printf("[ Destroy Singleton Instance : common_data1_t ]\n");
		singleton<common_data1_t> data("test1-C");
		data.destroy("test1:destroy");//明示的インスタンス破棄
	}
	{
		printf("[ Destroy Singleton Instance : common_data2_t ]\n");
		simpleSingleton<common_data2_t> data("test2-A");
		data.destroy("test2:destroy");//明示的インスタンス破棄
	}

	//シングルトンアクセス
	{
		printf("[ Access Singleton : common_data1_t(2) ]\n");
		singleton<common_data1_t> data("test1-C");
		if (data.isCreated())
			data->printInfo();
		else
			printf("common_data1_t is not created.\n");
	}
	{
		printf("[ Access Singleton : common_data2_t(2) ]\n");
		simpleSingleton<common_data2_t> data("test2-C");//暗黙的インスタンス生成
		data->printInfo();
	}
	
	//シングルトンインスタンス生成
	{
		printf("[ Create Singleton Instance(2) : common_data1_t ]\n");
		singleton<common_data1_t> data("test1-D", with_lock_shared);
		data.create("test1:create(2)", 0, 0);//明示的インスタンス生成
	}

	//スレッド用関数１Ａ
	auto thread_func1a = [](const int thread_no)
	{
		char name[20];
		sprintf(name, "Thread1A(%d)", thread_no);
		threadId thread_id(name);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));//100ミリ秒スリープ
		for (int i = 0; i < 10; ++i)
		{
			for (int j = 0; j < 100; ++j)
			{
				char procedure[20];
				sprintf(procedure, "Proc[%d][%d]", i, j);
				//シングルトンアクセス
				{
					singleton<common_data1_t> data(procedure, with_lock_shared);//共有ロックでシングルトンにアクセス
					{
						data.lock().upgrade();//共有ロックを排他ロックに昇格
						data->val1()++;
						data->val2()--;
					}
				}
			}
			std::this_thread::sleep_for(std::chrono::microseconds(10));//10マイクロ秒スリープ
		}
	};

	//スレッド用関数１Ｂ
	auto thread_func1b = [](const int thread_no)
	{
		char name[20];
		sprintf(name, "Thread1B(%d)", thread_no);
		threadId thread_id(name);
		char procedure[20];
		sprintf(procedure, "Proc");
		//シングルトンアクセス
		{
			singleton<common_data1_t> data(procedure, with_lock_shared);//共有ロックでシングルトンにアクセス
			std::this_thread::sleep_for(std::chrono::milliseconds(500));//500ミリ秒スリープ
		}
	};

	//スレッド用関数２
	auto thread_func2 = [](const int thread_no)
	{
		char name[20];
		sprintf(name, "Thread2(%d)", thread_no);
		threadId thread_id(name);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));//10ミリ秒スリープ
		for (int i = 0; i < 10; ++i)
		{
			for (int j = 0; j < 100; ++j)
			{
				char procedure[20];
				sprintf(procedure, "Proc[%d][%d]", i, j);
				//シングルトンアクセス
				{
					simpleSingleton<common_data2_t> data(procedure);//排他ロックでシングルトンにアクセス
					data->val1()++;
					data->val2()--;
				}
			}
			std::this_thread::sleep_for(std::chrono::microseconds(10));//10マイクロ秒スリープ
		}
	};

	//メインスレッドに名前を付与
	{
		threadId thread_id("MainThread");
	}

	//スレッド生成
	std::thread* th[15];
	for (std::size_t i = 0; i < extentof(th); ++i)
	{
		const int type = i % 3;
		if (type == 0)
			th[i] = new std::thread(thread_func1a, i);
		else if (type == 1)
			th[i] = new std::thread(thread_func1b, i);
		else//if (type == 2)
			th[i] = new std::thread(thread_func2, i);
	}

	//10ミリ秒待ち
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

#ifdef GASHA_SINGLETON_DEBUG_ENABLED
	//シングルトンアクセス中情報を表示
	{
		printf("[ Singleton Access Info ]\n");
		singleton<common_data1_t> data("test1-E", with_lock_shared);
		auto& debug = data.debug();
		printf("accessCount=%d\n", debug.accessCount());
		printf("created=%.9lf sec, \"%s\"\n", debug.createdSysTime(), debug.createdProcedureName());
		printf("destroy=%.9lf sec, \"%s\"\n", debug.destroyedSysTime(), debug.destroyedProcedureName());
		auto& list = debug.list();
		{
			auto lock = list.lockSharedScoped();
			printf("  Access Info: (Count=%d)\n", list.size());
			for (auto& info : list)
			{
				printf("  - [%d] %.9lf sec, \"%s\": thread=\"%s\"(0x%08x)\n", info.m_seqNo, info.m_sysTime, info.m_procedureName, info.m_threadId.name(), info.m_threadId.id());
			}
		}
	}
#endif//GASHA_SINGLETON_DEBUG_ENABLED

	//スレッド終了待ち
	for (std::size_t i = 0; i < extentof(th); ++i)
	{
		th[i]->join();
		delete th[i];
		th[i] = nullptr;
	}

	//シングルトンアクセス（スレッドで更新した結果の表示）
	{
		printf("[ Access Singleton : common_data1_t(3) ]\n");
		singleton<common_data1_t> data("test1-F");
		data->printInfo();
	}
	{
		printf("[ Access Singleton : common_data2_t(3) ]\n");
		simpleSingleton<common_data2_t> data("test2-F");
		data->printInfo();
	}

	printf("- end -\n");
}

// End of file