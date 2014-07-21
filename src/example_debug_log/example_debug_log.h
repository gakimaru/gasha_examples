#pragma once
#ifndef GASHA_INCLUDED_EXAMPLE_DEBUG_MESSAGE_H
#define GASHA_INCLUDED_EXAMPLE_DEBUG_MESSAGE_H

//--------------------------------------------------------------------------------
// exmaple_debug_log.h
// デバッグログテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include <gasha/log_category.h>//ログカテゴリ

//--------------------------------------------------------------------------------
//テスト用コンパイラスイッチ／定数

#define ENABLE_BASIC_TEST//デバッグログ系基本テストを有効にする場合は、このマクロを有効化する
//#define ENABLE_BASIC_ASSERT_TEST//基本テストでアサーション／ブレークポイント／ウォッチポイントを有効にする場合は、このマクロを有効化する ※ブレークポイントやキー入力待ちが発生するので注意
#define ENABLE_FEATURE_TEST//デバッグログ系個別機能テストを有効にする場合は、このマクロを有効化する
//#define ENABLE_FEATURE_ASSERT_TEST//個別機能テストでアサーション／ブレークポイント／ウォッチポイントを有効にする場合は、このマクロを有効化する ※ブレークポイントやキー入力待ちが発生するので注意
#define ENABLE_PERFORMANCE_TEST//デバッグログ系パフォーマンステストを有効にする場合は、このマクロを有効化する

#ifdef GASHA_OPTIMIZED

static const int PROFILE_TEST1_THREADS = 4;//プロファイルテスト１：スレッド数
static const int PROFILE_TEST1_FRAMES = 120;//プロファイルテスト１：総フレーム数
static const int PROFILE_TEST1_PROCESSES = 2;//プロファイルテスト１：処理数
static const int PROFILE_TEST1_REPEATS = 2;//プロファイルテスト１：1フレーム内での処理の繰り返し数
static const int PROFILE_TEST1_SUM_FRAMES = 30;//プロファイルテスト１：集計フレーム数
static const int PROFILE_TEST1_TIME_MIN = 100;//プロファイルテスト１：ランダム処理時間範囲最小（μ秒）
static const int PROFILE_TEST1_TIME_MAX = 2000;//プロファイルテスト１：ランダム処理時間範囲最大（μ秒）

static const int PROFILE_TEST2_THREADS = 10;//プロファイルテスト２：スレッド数 ※二スレッドごとに同じスレッド名にする
static const int PROFILE_TEST2_FRAMES = 300;//プロファイルテスト２：総フレーム数
static const int PROFILE_TEST2_PROCESSES = 20;//プロファイルテスト２：処理数
static const int PROFILE_TEST2_REPEATS = 10;//プロファイルテスト２：1フレーム内での処理の繰り返し数
static const int PROFILE_TEST2_SUM_FRAMES = 30;//プロファイルテスト２：集計フレーム数

#else//GASHA_OPTIMIZED

static const int PROFILE_TEST1_THREADS = 3;//プロファイルテスト１：スレッド数
static const int PROFILE_TEST1_FRAMES = 120;//プロファイルテスト１：総フレーム数
static const int PROFILE_TEST1_PROCESSES = 2;//プロファイルテスト１：処理数
static const int PROFILE_TEST1_REPEATS = 2;//プロファイルテスト１：1フレーム内での処理の繰り返し数
static const int PROFILE_TEST1_SUM_FRAMES = 30;//プロファイルテスト１：集計フレーム数
static const int PROFILE_TEST1_TIME_MIN = 100;//プロファイルテスト１：ランダム処理時間範囲最小（μ秒）
static const int PROFILE_TEST1_TIME_MAX = 2000;//プロファイルテスト１：ランダム処理時間範囲最大（μ秒）

static const int PROFILE_TEST2_THREADS = 4;//プロファイルテスト２：スレッド数 ※二スレッドごとに同じスレッド名にする
static const int PROFILE_TEST2_FRAMES = 120;//プロファイルテスト２：総フレーム数
static const int PROFILE_TEST2_PROCESSES = 10;//プロファイルテスト２：処理数
static const int PROFILE_TEST2_REPEATS = 5;//プロファイルテスト２：1フレーム内での処理の繰り返し数
static const int PROFILE_TEST2_SUM_FRAMES = 30;//プロファイルテスト２：集計フレーム数

#endif//GASHA_OPTIMIZED

//----------------------------------------
//独自ログカテゴリ
enum extLogCategory : GASHA_ logCategory::category_type
{
	forMiniGame = MAKE_LOG_CATEGORY_VALUE(10),//ミニゲーム用
	forTARO = MAKE_LOG_CATEGORY_VALUE(20),//開発者個人用：太郎用
	forJIRO = MAKE_LOG_CATEGORY_VALUE(21),//開発者個人用：次郎用
};

//----------------------------------------
//デバッグログテスト
void example_debugLog();//デバッグログテスト
void example_debugLog_basicTest();//デバッグログ基本テスト
void example_debugLog_featureTest();//デバッグログ個別機能テスト
void example_debugLog_perfomanceTest();//デバッグログパフォーマンステスト

void beginLogQueueMonitor();//ログキューモニタースレッドを起動
void waitLogQueueMonitor();//ログキューモニタースレッドを終了待ち

#endif//GASHA_INCLUDED_EXAMPLE_DEBUG_MESSAGE_H

// End of file
