#pragma once
#ifndef GASHA_INCLUDED_EXAMPLE_SHARED_DATA_H
#define GASHA_INCLUDED_EXAMPLE_SHARED_DATA_H

//--------------------------------------------------------------------------------
// exmaple_shared_data.h
// マルチスレッド共有データテスト
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include <cstddef>//std::size_t

//--------------------------------------------------------------------------------
//テスト用コンパイラスイッチ／定数

#define ENABLE_TAGGED_PTRTEST//タグ付きポインタテストを実行する場合は、このマクロを有効化する
#define ENABLE_EASY_TEST//簡易テストを実行する場合は、このマクロを有効化する
#define ENABLE_THREAD_TEST//スレッドテストを実行する場合は、このマクロを有効化する

#define ENABLE_TEST_FOR_SHARED_POOL_ALLOCATOR//プールアロケータのテストを有効にする場合は、このマクロを有効化する
#define ENABLE_TEST_FOR_LF_POOL_ALLOCATOR//ロックフリープールアロケータのテストを有効にする場合は、このマクロを有効化する
#define ENABLE_TEST_FOR_SHARED_STACK//マルチスレッド共有スタックのテストを有効にする場合は、このマクロを有効化する
#define ENABLE_TEST_FOR_LF_STACK//ロックフリースタックのテストを有効にする場合は、このマクロを有効化する
#define ENABLE_TEST_FOR_SHARED_QUEUE//マルチスレッド共有キューのテストを有効にする場合は、このマクロを有効化する
#define ENABLE_TEST_FOR_LF_QUEUE//ロックフリーキューのテストを有効にする場合は、このマクロを有効化する

#define USE_LOCK_TYPE 1//共有データのロックに spinLock を使用する場合は、このマクロを有効化する（デフォルト）
//#define USE_LOCK_TYPE 2//共有データのロックに std::mutex を使用する場合は、このマクロを有効化する
//#define USE_LOCK_TYPE 0//共有データのロックに dummyLock を使用する場合は、このマクロを有効化する（ロックしなくなる）

//ロックフリーアルゴリズムABA問題対策設定
//※ロックフリースタック／キューで使用するタグ付きポインタのタグサイズ（ビット数）とタグ位置（ビットシフト数）の設定。
//※タグサイズが大きければ大きいほど、スレッドが混み合った時の問題発生を抑えることができる。
#ifdef GASHA_IS_32BIT//【32bit環境向け】
static const std::size_t TEST_TAGGED_PTR_TAG_SIZE = 32;//タグサイズ=32bit
static const int TEST_TAGGED_PTR_TAG_SHIFT = 32;//タグ位置=上位32bit
//static const std::size_t TEST_TAGGED_PTR_TAG_SIZE = 0;//タグサイズ=デフォルト：型のアラインメントサイズに合わせる
//static const int TEST_TAGGED_PTR_TAG_SHIFT = 0;//タグ位置=最下位ビット（ポインターのアラインメントの隙間にタグを仕込む）
#endif//GASHA_IS_32BIT
#ifdef GASHA_IS_64BIT//【64bit環境向け】
static const std::size_t TEST_TAGGED_PTR_TAG_SIZE = 8;//タグサイズ=8bit ※64ビット中上位8bitを未使用と仮定しているので、あまり良い設定ではない
static const int TEST_TAGGED_PTR_TAG_SHIFT = -8;//タグ位置=上位8bit       （タグ付きポインタを128bitで扱うのがベストだが、現状未対応）
//static const std::size_t TEST_TAGGED_PTR_TAG_SIZE = 0;//タグサイズ=デフォルト：型のアラインメントサイズに合わせる
//static const int TEST_TAGGED_PTR_TAG_SHIFT = 0;//タグ位置=最下位ビット（ポインターのアラインメントの隙間にタグを仕込む）
#endif//GASHA_IS_64BIT

#define ENABLE_TEST_PRINT//スレッドテストの経過メッセージを表示する場合は、このマクロを有効化する
#define ENABLE_TEST_PRINT_DEBUG_INFO//スレッドテストの最後にデバッグ情報を表示する場合は、このマクロを有効化する
//#define TEST_1_TIME//1回だけのテストを実行する場合は、このマクロを有効化する
#ifdef TEST_1_TIME

static const int TEST_COUNT = 1;//テスト回数
static const int TEST_PRINT_COUNT = 0;//テスト中のメッセージ表示回数
static const int TEST_PUSH_THREADS = 1;//テスト用のプッシュ／エンキュースレッド数
static const int TEST_POP_THREADS = 1;//テスト用のポップ／デキュースレッド数
static const int TEST_ALLOC_THREADS = 1;//テスト用のアロケートスレッド数
static const int TEST_POOL_SIZE = 10;//テスト用プールアロケータのプールサイズ

#else//TEST_1_TIME
#ifdef GASHA_OPTIMIZED

static const int TEST_COUNT = 1000000;//テスト回数
static const int TEST_PRINT_COUNT = 5;//テスト中のメッセージ表示回数
static const int TEST_PUSH_THREADS = 3;//テスト用のプッシュ／エンキュースレッド数
static const int TEST_POP_THREADS = 2;//テスト用のポップ／デキュースレッド数
static const int TEST_ALLOC_THREADS = 5;//テスト用のアロケートスレッド数
static const int TEST_POOL_SIZE = 20;//テスト用プールアロケータのプールサイズ

#else//GASHA_OPTIMIZED

static const int TEST_COUNT = 100;//テスト回数
static const int TEST_PRINT_COUNT = 5;//テスト中のメッセージ表示回数
static const int TEST_PUSH_THREADS = 3;//テスト用のプッシュ／エンキュースレッド数
static const int TEST_POP_THREADS = 2;//テスト用のポップ／デキュースレッド数
static const int TEST_ALLOC_THREADS = 5;//テスト用のアロケートスレッド数
static const int TEST_POOL_SIZE = 20;//テスト用プールアロケータのプールサイズ

#endif//GASHA_OPTIMIZED
#endif//TEST_1_TIME
#ifdef ENABLE_TEST_PRINT
static const int TEST_PRINT_STEP = TEST_COUNT >= TEST_PRINT_COUNT * 10 ? TEST_COUNT / TEST_PRINT_COUNT : 10;//テスト中のメッセージ表示の間隔
#else//ENABLE_TEST_PRINT
static const int TEST_PRINT_STEP = 0;
#endif//ENABLE_TEST_PRINT

//--------------------------------------------------------------------------------
//テスト用データ
struct data_t
{
	int m_temp;
	int m_value;
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
//マルチスレッド共有データ

#include <gasha/pool_allocator.h>//プールアロケータ【宣言部】
#include <gasha/shared_stack.h>//マルチスレッド共有スタック【宣言部】
#include <gasha/shared_queue.h>//マルチスレッド共有キュー【宣言部】

typedef GASHA_ poolAllocator_withType<data_t, TEST_POOL_SIZE, lock_type> shared_pool_allocator_t;//プールアロケータ
typedef GASHA_ sharedStack<data_t, TEST_POOL_SIZE, lock_type> shared_stack_t;//マルチスレッド共有スタック
typedef GASHA_ sharedQueue<data_t, TEST_POOL_SIZE, lock_type> shared_queue_t;//マルチスレッド共有キュー

//----------------------------------------
//ロックフリー共有データ

#include <gasha/lf_pool_allocator.h>//ロックフリープールアロケータ【宣言部】
#include <gasha/lf_stack.h>//ロックフリースタック【宣言部】
#include <gasha/lf_queue.h>//ロックフリーキュー【宣言部】

typedef GASHA_ lfPoolAllocator_withType<data_t, TEST_POOL_SIZE> lf_pool_allocator_t;//ロックフリープールアロケータ
typedef GASHA_ lfStack<data_t, TEST_POOL_SIZE, TEST_TAGGED_PTR_TAG_SIZE, TEST_TAGGED_PTR_TAG_SHIFT> lf_stack_t;//ロックフリースタック
typedef GASHA_ lfQueue<data_t, TEST_POOL_SIZE, TEST_TAGGED_PTR_TAG_SIZE, TEST_TAGGED_PTR_TAG_SHIFT> lf_queue_t;//ロックフリーキュー

//----------------------------------------
//マルチスレッド共有データテスト
void example_shared_data();

#endif//GASHA_INCLUDED_EXAMPLE_SHARED_DATA_H

// End of file
