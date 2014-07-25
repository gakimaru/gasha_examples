//--------------------------------------------------------------------------------
// exmaple_debug_log.cpp
// デバッグログテスト
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_debug_log.h"//デバッグログテスト

#include <gasha/thread_id.h>//スレッドID

GASHA_USING_NAMESPACE;//ネームスペース使用

//----------------------------------------
//デバッグログテスト
void example_debugLog()
{
	//メインスレッドに名前を付ける
	threadId thread_id("MainThread");

#ifdef ENABLE_BASIC_TEST
example_debugLog_basicTest();//デバッグログ基本テスト
#endif//ENABLE_BASIC_TEST

#ifdef ENABLE_FEATURE_TEST
example_debugLog_featureTest();//デバッグログ個別機能テスト
#endif//ENABLE_FEATURE_TEST

#ifdef ENABLE_PERFORMANCE_TEST
example_debugLog_perfomanceTest();//デバッグログパフォーマンステスト
#endif//ENABLE_PERFORMANCE_TEST
}

// End of file
