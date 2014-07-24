//--------------------------------------------------------------------------------
// exmaple_debug_log_sub.cpp
// デバッグログテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_debug_log.h"//デバッグログテスト

#include <gasha/log_queue_monitor.h>//ログキューモニター
//#include <gasha/std_log_print.h>//標準ログ出力：stdLogPrint

#include <cstdio>//std::printf()

#pragma warning(push)//【VC++】ワーニング設定を退避
#pragma warning(disable: 4530)//【VC++】C4530を抑える
#include <thread>//C++11 std::thread
#pragma warning(pop)//【VC++】ワーニング設定を復元

GASHA_USING_NAMESPACE;//ネームスペース使用

#ifdef GASHA_LOG_IS_ENABLED//デバッグログ無効時は無効化
//スレッドのインスタンス
static std::thread* s_threadInstance = nullptr;
#endif//GASHA_LOG_IS_ENABLED//デバッグログ無効時は無効化

//----------------------------------------
//ログキューモニタースレッド
static void monitor_thread()
{
	//開始メッセージ
	std::printf("START: logQueueMonitor thread\n");

	//ログキューモニター起動 ※abort() が呼び出されるまでループし続ける
	logQueueMonitor mon;
	mon.monitor();
	//mon.monitor(stdLogPrint());//※出力処理をカスタマイズする場合、引数で処理（関数オブジェクトなど）を渡す

	//終了メッセージ
	std::printf("END: logQueueMonitor thread\n");
};

//----------------------------------------
//ログキューモニタースレッドを起動
void beginLogQueueMonitor()
{
#ifdef GASHA_LOG_IS_ENABLED//デバッグログ無効時は無効化
	//スレッド起動
	s_threadInstance = new std::thread(monitor_thread);
#endif//GASHA_LOG_IS_ENABLED//デバッグログ無効時は無効化
}

//ログキューモニタースレッドを終了待ち
void waitLogQueueMonitor()
{
#ifdef GASHA_LOG_IS_ENABLED//デバッグログ無効時は無効化
	//スレッド終了待ちと破棄
	s_threadInstance->join();
	delete s_threadInstance;
	s_threadInstance = nullptr;
#endif//GASHA_LOG_IS_ENABLED//デバッグログ無効時は無効化
}

// End of file
