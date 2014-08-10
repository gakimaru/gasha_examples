//--------------------------------------------------------------------------------
// game_loop.cpp
// ユニットテスト：ゲームループ
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include <cstdio>
#include <cstdlib>

#include "real_test/game_loop/game_loop.h"
#include "real_test/unit_test_id.h"

#include <gasha/std_console.h>//標準コンソール

GASHA_USING_NAMESPACE;//ネームスペース使用

//ゲームループクラス

//コンストラクタ
gameLoop::gameLoop(const int argc, const char* argv[]):
	m_argc(argc),
	m_argv(argv),
	m_resultOfInitialize(EXIT_SUCCESS),
	m_resultOfFinalize(EXIT_SUCCESS),
	m_resultOfMain(EXIT_SUCCESS)
{
	std::printf("- gameLoop::gameLoop()\n");
}

//デストラクタ
gameLoop::~gameLoop()
{
	std::printf("- gameLoop::~gameLoop()\n");
}

//初期化処理
int gameLoop::initialize()
{
	std::printf("- gameLoop::initialize() [BEGIN] ...\n");
	m_resultOfInitialize = EXIT_SUCCESS;
	std::printf("- gameLoop::initialize() [END]\n");
	return m_resultOfInitialize;
}

//終了処理
int gameLoop::finalize()
{
	std::printf("- gameLoop::finalize() [BEGIN] ...\n");
	m_resultOfFinalize = EXIT_SUCCESS;
	std::printf("- gameLoop::finalize() [END]\n");
	return m_resultOfFinalize;
}

//メイン処理
int gameLoop::main()
{
	std::printf("- gameLoop::main() [BEGIN] ...\n");

	//自動ユニットテスト
	//※GASHA_UNIT_TEST_ENABLED と GASHA_UNIT_TEST_AUTO_RUN_ENABLED が定義されている時だけ処理が有効化される。
	//※実行後に自動的に return する。
	//※通常、プログラムの初期化後、メイン処理の開始時に呼び出しを行う。
	GASHA_UT_AUTO_RUN();
	GASHA_UT_AUTO_RUN_AND_GET_RESULT(m_resultOfMain);//※結果を変数に格納したい場合はこのマクロを使用

	//手動ユニットテスト
	//※GASHA_UNIT_TEST_ENABLED が定義されている時だけ処理が有効化される。
	//※プレイヤー操作を伴う特定の状況設定に基づく回帰テストなどに有効。（特定のステージや装備状態で繰り返し実行するテストなど）
	GASHA_UT_PUT("[Unit Test:MANUAL:ALL]\n");
	GASHA_UT_MANUAL_RUN(ut::ATTR_ANY);//全モジュール、全グループの手動実行
	GASHA_UT_PUT("[Unit Test:MANUAL:MODULE(\"moduleB\")]\n");
	GASHA_UT_MANUAL_RUN_BY_MODULE("moduleB", ut::ATTR_ANY);//特定モジュール、全グループの手動実行
	GASHA_UT_PUT("[Unit Test:MANUAL:GROUP(2)]\n");
	GASHA_UT_MANUAL_RUN_BY_GROUP(2, ut::ATTR_ANY);//全モジュール、特定グループの手動実行
	GASHA_UT_PUT("[Unit Test:MANUAL:MODULE(\"moduleB\") & GROUP(2)]\n");
	GASHA_UT_MANUAL_RUN_BY_MODULE_AND_GROUP("moduleB", 2, ut::ATTR_ANY);//特定モジュール、特定グループの手動実行
	GASHA_UT_PUT("[Unit Test:MANUAL:ATTR_AUTO]\n");
	GASHA_UT_MANUAL_RUN(ut::ATTR_AUTO);//自動実行属性のテストを手動実行

	//前回のユニットテストの結果を確認／リセット／結果に基づいて終了
	GASHA_UT_PRINTF("Result of Unit-test: passed=%d, missed=%d\n", GASHA_UT_LAST_PASSED_TOTAL(), GASHA_UT_LAST_MISSED_TOTAL());//前回のユニットテストの結果
	GASHA_UT_RESET_LAST_RESULT();   //前回のユニットテストの結果をリセット
	GASHA_UT_PRINTF("After reset: passed=%d, missed=%d\n", GASHA_UT_LAST_PASSED_TOTAL(), GASHA_UT_LAST_MISSED_TOTAL());
	GASHA_UT_RETURN_WHEN_MISSED();  //前回のユニットテストの結果に一つでもミスがあったら return する
	GASHA_UT_EXIT_WHEN_MISSED();    //前回のユニットテストの結果に一つでもミスがあったら exit する
	GASHA_UT_ABORT_WHEN_MISSED();   //前回のユニットテストの結果に一つでもミスがあったら abort する
	GASHA_UT_ASSERT_WHEN_MISSED();  //前回のユニットテストの結果に一つでもミスがあったらアサーション違反とする

	//ユニットテストの補助処理
	GASHA_UT_SET_CONSOLE(stdDummyConsole::instance());//ユニットテスト結果の表示用コンソールを変更
	                                                  //※stdDummyConsole::instance() を指定すると一切何も表示されなくなる（処理は実行される）。
	GASHA_UT_PUT("Print after GASHA_UT_SET_CONSOLE()\n");
#ifdef GASHA_UNIT_TEST_ENABLED//ユニットテスト有効時のみの処理
	int var_for_ut = 123;
	std::printf("Print: var_for_ut=%d\n", var_for_ut);
#endif//GASHA_UNIT_TEST_ENABLED

	m_resultOfMain = EXIT_SUCCESS;
	std::printf("- gameLoop::main() [END]\n");
	return m_resultOfMain;
}

// End of file
