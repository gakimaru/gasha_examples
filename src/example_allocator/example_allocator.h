#pragma once
#ifndef GASHA_INCLUDED_EXAMPLE_ALLOCATOR_H
#define GASHA_INCLUDED_EXAMPLE_ALLOCATOR_H

//--------------------------------------------------------------------------------
// exmaple_allocator.h
// アロケータテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//テスト用コンパイラスイッチ／定数

#define ENABLE_BASIC_TEST//アロケータ系基本テストを有効にする場合は、このマクロを有効化する
#define ENABLE_TEST_FOR_STACK_ALLOATOR//スタックアロケータのテストを有効にする場合は、このマクロを有効化する
#define ENABLE_TEST_FOR_DUAL_STACK_ALLOATOR//スタックアロケータのテストを有効にする場合は、このマクロを有効化する
#define ENABLE_TEST_FOR_MONO_ALLOATOR//単一アロケータのテストを有効にする場合は、このマクロを有効化する
#define ENABLE_TEST_FOR_POOL_ALLOATOR//プールアロケータのテストを有効にする場合は、このマクロを有効化する
#define ENABLE_TEST_FOR_STD_ALLOATOR//標準アロケータのテストを有効にする場合は、このマクロを有効化する
#define ENABLE_TEST_FOR_POLY_ALLOATOR//多態アロケータのテストを有効にする場合は、このマクロを有効化する
#define ENABLE_TEST_FOR_ALLOATOR_PERFORMANCE//アロケータパフォーマンスのテストを有効にする場合は、このマクロを有効化する
#define ENABLE_TEST_FOR_ALLOATOR_PERFORMANCE_ST//シングルスレッド時のパフォーマンステストを有効にする場合は、このマクロを有効化する
#define ENABLE_TEST_FOR_ALLOATOR_PERFORMANCE_MT//マルチスレッド時のパフォーマンステストを有効にする場合は、このマクロを有効化する
#define ENABLE_TEST_FOR_NEW_DELETE//new/delete演算子の挙動確認用テストを有効にする場合は、このマクロを有効化する

#define USE_LOCK_TYPE 0//共有データのロックに dummyLock を使用する場合は、このマクロを有効化する（ロックしなくなる）（デフォルト）
//#define USE_LOCK_TYPE 1//共有データのロックに spinLock を使用する場合は、このマクロを有効化する
//#define USE_LOCK_TYPE 2//共有データのロックに std::mutex を使用する場合は、このマクロを有効化する

#ifdef GASHA_OPTIMIZED

static const int ST_TEST_REPEAT_COUNT1 = 1000;//シングルスレッドパフォーマンステストの繰り返しテスト回数① ※①×②回の繰り返し
static const int ST_TEST_REPEAT_COUNT2 = 1000;//シングルスレッドパフォーマンステストの繰り返しテスト回数②

static const int MT_TEST_REPEAT_COUNT1 = 10;//マルチスレッドパフォーマンステストの繰り返しテスト回数① ※①スレッド数×②×③回の繰り返し
static const int MT_TEST_REPEAT_COUNT2 = 100;//マルチスレッドパフォーマンステストの繰り返しテスト回数②
static const int MT_TEST_REPEAT_COUNT3 = 100;//マルチスレッドパフォーマンステストの繰り返しテスト回数②

#else//GASHA_OPTIMIZED

static const int ST_TEST_REPEAT_COUNT1 = 10;//シングルスレッドパフォーマンステストの繰り返しテスト回数① ※①×②回の繰り返し
static const int ST_TEST_REPEAT_COUNT2 = 10;//シングルスレッドパフォーマンステストの繰り返しテスト回数②

static const int MT_TEST_REPEAT_COUNT1 = 10;//マルチスレッドパフォーマンステストの繰り返しテスト回数① ※①スレッド数×②×③回の繰り返し
static const int MT_TEST_REPEAT_COUNT2 = 10;//マルチスレッドパフォーマンステストの繰り返しテスト回数②
static const int MT_TEST_REPEAT_COUNT3 = 10;//マルチスレッドパフォーマンステストの繰り返しテスト回数②

#endif//GASHA_OPTIMIZED

//----------------------------------------
//テスト用構造体
struct alignas(16) data_t
{
	int m_val;//値

	data_t();//デフォルトコンストラクタ
	data_t(const int val);//コンストラクタ
	data_t(data_t&& obj);//ムーブコンストラクタ
	data_t(const data_t& obj);//コピーコンストラクタ
	~data_t();//デストラクタ
};
//テスト用構造体
//※32バイトアラインメント
struct alignas(32) data2_t
{
	int m_val;//値
};

//--------------------------------------------------------------------------------
//各マルチスレッド共有データ定義

//----------------------------------------
//マルチスレッド共有データに使用するロックオブジェクトの設定

#if USE_LOCK_TYPE == 1//共有データのロックに spinLock を使用する場合

#include <gasha/spin_lock.h>//スピンロック
typedef GASHA_ spinLock lock_type;//共有データのロックに spinLock を使用する場合は、この行を有効化する

#elif USE_LOCK_TYPE == 2//共有データのロックに std::mutex を使用する場合

#pragma warning(push)//【VC++】ワーニング設定を退避
#pragma warning(disable: 4530)//【VC++】C4530を抑える
#include <mutex>//C++11 std::mutex
#pragma warning(pop)//【VC++】ワーニング設定を復元

typedef std::mutex lock_type;//共有データのロックに std::mutex を使用する場合は、この行を有効化する

#else//if USE_LOCK_TYPE == 0//共有データのロックに dummyLock を使用する場合

#include <gasha/dummy_lock.h>//ダミーロック
typedef GASHA_ dummyLock lock_type;//共有データのロックに dummyLock を使用する場合は、この行を有効化する（ロックしなくなる）

#endif//USE_LOCK_TYPE

//----------------------------------------
//アロケータテスト
void example_allocator();//アロケータテスト
void example_allocator_basic_test();//アロケータ系基本テスト
void example_stack_allocator();//スタックアロケータテスト
void example_dual_stack_allocator();//双方向スタックアロケータテスト
void example_mono_allocator();//単一アロケータテスト
void example_pool_allocator();//プールアロケータテスト
void example_std_allocator();//標準アロケータテスト
void example_poly_allocator();//多態アロケータテスト
void example_allocator_performance();//アロケータパフォーマンステスト
void examination_new_delete();//new/delete演算子の挙動確認用テスト

#endif//GASHA_INCLUDED_EXAMPLE_ALLOCATOR_H

// End of file
