//--------------------------------------------------------------------------------
// exmaple_singleton.cpp
// シングルトンテスト
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_singleton.h"//シングルトンテスト

#include <gasha/singleton.h>//シングルトン

#include <gasha/type_traits.h>//型特性ユーティリティ：extentof
#include <gasha/string.h>//文字列処理：spprintf
#include <gasha/thread_id.h>//スレッドID

#include <cstdio>//std::printf()

#pragma warning(push)//【VC++】ワーニング設定を退避
#pragma warning(disable: 4530)//【VC++】C4530を抑える
#include <thread>//C++11 std::thread
#pragma warning(pop)//【VC++】ワーニング設定を復元

GASHA_USING_NAMESPACE;//ネームスペース使用

//----------------------------------------
//シングルトン対象クラス①

//メソッド
void common_data1_t::printInfo()
{
	std::printf("common_data1: val1=%d, val2=%d\n", m_val1, m_val2);
}

//コンストラクタ
common_data1_t::common_data1_t(const int val1, const int val2) :
	m_val1(val1),
	m_val2(val2)
{
#ifdef TEST_DATA_WATCH_CONSTRUCTOR
	std::printf("common_data1_t::constructor(%d, %d)\n", val1, val2);
#endif//TEST_DATA_WATCH_CONSTRUCTOR
}

//デストラクタ
common_data1_t::~common_data1_t()
{
#ifdef TEST_DATA_WATCH_CONSTRUCTOR
	std::printf("common_data1_t::destructor(): m_val1=%d, m_val2=%d\n", m_val1, m_val2);
#endif//TEST_DATA_WATCH_CONSTRUCTOR
}

//----------------------------------------
//シングルトン対象クラス②

//メソッド
void common_data2_t::printInfo()
{
	std::printf("common_data2: val1=%d, val2=%d\n", m_val1, m_val2);
}

//コンストラクタ
common_data2_t::common_data2_t() :
	m_val1(0),
	m_val2(0)
{
#ifdef TEST_DATA_WATCH_CONSTRUCTOR
	std::printf("common_data2_t::constructor(): m_val1=%d, m_val2=%d\n", m_val1, m_val2);
#endif//TEST_DATA_WATCH_CONSTRUCTOR
}

//デストラクタ
common_data2_t::~common_data2_t()
{
#ifdef TEST_DATA_WATCH_CONSTRUCTOR
	std::printf("common_data2_t::destructor(): m_val1=%d, m_val2=%d\n", m_val1, m_val2);
#endif//TEST_DATA_WATCH_CONSTRUCTOR
}

//----------------------------------------
//シングルトンテスト
void example_singleton()
{
	std::printf("----- Test for singleton -----\n");

	//シングルトンインスタンス生成
	{
		std::printf("[ Create Singleton Instance : common_data1_t ]\n");
		singleton<common_data1_t> data("test1-A", with_lock_shared);
		data.create("test1:create", 1, 2);//明示的インスタンス生成
	}
	{
		std::printf("[ Create Singleton Instance : common_data2_t ]\n");
		simpleSingleton<common_data2_t> data("test2-A");//暗黙的インスタンス生成
		data->val1() = 3;
		data->val2() = 4;
	}
	
	//シングルトンアクセス
	{
		std::printf("[ Access Singleton : common_data1_t(1) ]\n");
		singleton<common_data1_t> data("test1-B");
		data->printInfo();
	}
	{
		std::printf("[ Access Singleton : common_data2_t(1) ]\n");
		simpleSingleton<common_data2_t> data("test2-B");
		data->printInfo();
	}

	//シングルトンインスタンス破棄
	{
		std::printf("[ Destroy Singleton Instance : common_data1_t ]\n");
		singleton<common_data1_t> data("test1-C");
		data.destroy("test1:destroy");//明示的インスタンス破棄
	}
	{
		std::printf("[ Destroy Singleton Instance : common_data2_t ]\n");
		simpleSingleton<common_data2_t> data("test2-A");
		data.destroy("test2:destroy");//明示的インスタンス破棄
	}

	//シングルトンアクセス
	{
		std::printf("[ Access Singleton : common_data1_t(2) ]\n");
		singleton<common_data1_t> data("test1-C");
		if (data.isCreated())
			data->printInfo();
		else
			std::printf("common_data1_t is not created.\n");
	}
	{
		std::printf("[ Access Singleton : common_data2_t(2) ]\n");
		simpleSingleton<common_data2_t> data("test2-C");//暗黙的インスタンス生成
		data->printInfo();
	}
	
	//シングルトンインスタンス生成
	{
		std::printf("[ Create Singleton Instance(2) : common_data1_t ]\n");
		singleton<common_data1_t> data("test1-D", with_lock_shared);
		data.create("test1:create(2)", 0, 0);//明示的インスタンス生成
	}
	{
		char message[2048];
		singleton<common_data1_t> data("test1-D'", with_lock_shared);
		data.debugInfo(message, sizeof(message));
		std::printf("%s\n", message);
	}

	//スレッド用関数１Ａ
	auto thread_func1a = [](const std::size_t thread_no)
	{
		char name[20];
		GASHA_ spprintf(name, "Thread1A(%d)", thread_no);
		threadId thread_id(name);
		std::this_thread::sleep_for(std::chrono::milliseconds(20));//20ミリ秒スリープ
		for (int i = 0; i < 10; ++i)
		{
			for (int j = 0; j < 20; ++j)
			{
				char procedure[32];
				GASHA_ spprintf(procedure, "Write-Proc[%d][%d]", i, j);
				//シングルトンアクセス
				{
					singleton<common_data1_t> data(procedure, with_lock_shared);//共有ロックでシングルトンにアクセス
					{
						data.lock().upgrade();//共有ロックを排他ロックに昇格
						//シングルトンの情報を更新
						data->val1()++;
						data->val2()--;
					}
					std::this_thread::sleep_for(std::chrono::microseconds(10));//10マイクロ秒スリープ
				}
			}
		}
	};

	//スレッド用関数１Ｂ
	auto thread_func1b = [](const std::size_t thread_no)
	{
		char name[20];
		GASHA_ spprintf(name, "Thread1B(%d)", thread_no);
		threadId thread_id(name);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));//1ミリ秒スリープ
		for (int i = 0; i < 10; ++i)
		{
			for (int j = 0; j < 20; ++j)
			{
				char procedure[32];
				GASHA_ spprintf(procedure, "Read-Proc[%d][%d]", i, j);
				//シングルトンアクセス
				{
					const singleton<common_data1_t> data(procedure, with_lock_shared);//共有ロックでシングルトンにアクセス
					std::this_thread::sleep_for(std::chrono::microseconds(10));//10マイクロ秒スリープ
				}
			}
		}
	};

	//スレッド用関数２
	auto thread_func2 = [](const std::size_t thread_no)
	{
		char name[20];
		GASHA_ spprintf(name, "Thread2(%d)", thread_no);
		threadId thread_id(name);
		std::this_thread::sleep_for(std::chrono::milliseconds(20));//20ミリ秒スリープ
		for (int i = 0; i < 10; ++i)
		{
			for (int j = 0; j < 20; ++j)
			{
				char procedure[20];
				GASHA_ spprintf(procedure, "Write-Proc[%d][%d]", i, j);
				//シングルトンアクセス
				{
					simpleSingleton<common_data2_t> data(procedure);//ロックなしでシングルトンにアクセス
					data->val1()++;
					data->val2()--;
				}
				std::this_thread::sleep_for(std::chrono::microseconds(10));//10マイクロ秒スリープ
			}
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

	//100ミリ秒待ち
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	//シングルトンアクセス中情報を表示
	{
		char message[2048];
		singleton<common_data1_t> data("test1-E", with_lock_shared);//共有ロックでシングルトンにアクセス
		data.debugInfo(message, sizeof(message));
		std::printf("%s\n", message);
	}

	//スレッド終了待ち
	for (std::size_t i = 0; i < extentof(th); ++i)
	{
		th[i]->join();
		delete th[i];
		th[i] = nullptr;
	}

	//シングルトンアクセス（スレッドで更新した結果の表示）
	{
		std::printf("[ Access Singleton : common_data1_t(3) ]\n");
		singleton<common_data1_t> data("test1-F");
		data->printInfo();
	}
	{
		std::printf("[ Access Singleton : common_data2_t(3) ]\n");
		simpleSingleton<common_data2_t> data("test2-F");
		data->printInfo();
	}

	//シングルトンを破棄
	{
		singleton<common_data1_t> data("test1-G");
		data.destroy("test1:destroy");//明示的インスタンス破棄
	}

	//シングルトンアクセス後情報を表示
	{
		char message[2048];
		singleton<common_data1_t> data("test1-G'", with_lock_shared);
		data.debugInfo(message, sizeof(message));
		std::printf("%s\n", message);
	}

	std::printf("- end -\n");
}

// End of file
