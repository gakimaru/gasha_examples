#pragma once
#ifndef __EXAMPLE_SHARED_DATA_H_
#define __EXAMPLE_SHARED_DATA_H_

//--------------------------------------------------------------------------------
// exmaple_shared_data.h
// マルチスレッド共有データテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//テスト用コンパイラスイッチ／定数

#define ENABLE_TAGGED_PTRTEST//タグ付きポインタテストを実行する場合は、このマクロを有効化する
#define ENABLE_EASY_TEST//簡易テストを実行する場合は、このマクロを有効化する
#define ENABLE_THREAD_TEST//スレッドテストを実行する場合は、このマクロを有効化する

#define ENABLE_TEST_FOR_SHARED_POOL_ALLOCATOR//マルチスレッド共有プールアロケータのテストを有効にする場合は、このマクロを有効化する
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
#ifdef IS_32BIT//【32bit環境向け】
static const std::size_t TEST_TAGGED_PTR_TAG_SIZE = 32;//タグサイズ=32bit
static const int TEST_TAGGED_PTR_TAG_SHIFT = 32;//タグ位置=上位32bit
//static const std::size_t TEST_TAGGED_PTR_TAG_SIZE = 0;//タグサイズ=デフォルト：型のアラインメントサイズに合わせる
//static const int TEST_TAGGED_PTR_TAG_SHIFT = 0;//タグ位置=最下位ビット（ポインターのアラインメントの隙間にタグを仕込む）
#endif//IS_32BIT
#ifdef IS_64BIT//【64bit環境向け】
static const std::size_t TEST_TAGGED_PTR_TAG_SIZE = 8;//タグサイズ=8bit ※64ビット中上位8bitを未使用と仮定しているので、あまり良い設定ではない
static const int TEST_TAGGED_PTR_TAG_SHIFT = -8;//タグ位置=上位8bit       （タグ付きポインタを128bitで扱うのがベストだが、現状未対応）
//static const std::size_t TEST_TAGGED_PTR_TAG_SIZE = 0;//タグサイズ=デフォルト：型のアラインメントサイズに合わせる
//static const int TEST_TAGGED_PTR_TAG_SHIFT = 0;//タグ位置=最下位ビット（ポインターのアラインメントの隙間にタグを仕込む）
#endif//IS_64BIT

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
#ifdef _DEBUG//デバッグ版
static const int TEST_COUNT = 1000000;//テスト回数
static const int TEST_PRINT_COUNT = 5;//テスト中のメッセージ表示回数
static const int TEST_PUSH_THREADS = 3;//テスト用のプッシュ／エンキュースレッド数
static const int TEST_POP_THREADS = 2;//テスト用のポップ／デキュースレッド数
static const int TEST_ALLOC_THREADS = 5;//テスト用のアロケートスレッド数
static const int TEST_POOL_SIZE = 20;//テスト用プールアロケータのプールサイズ
#else//_DEBUG//リリース版
static const int TEST_COUNT = 10000000;//テスト回数
static const int TEST_PRINT_COUNT = 5;//テスト中のメッセージ表示回数
static const int TEST_PUSH_THREADS = 3;//テスト用のプッシュ／エンキュースレッド数
static const int TEST_POP_THREADS = 2;//テスト用のポップ／デキュースレッド数
static const int TEST_ALLOC_THREADS = 5;//テスト用のアロケートスレッド数
static const int TEST_POOL_SIZE = 20;//テスト用プールアロケータのプールサイズ
#endif//_DEBUG
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

//マルチスレッド共有データテスト
void example_shared_data();

#endif//__EXAMPLE_SHARED_DATA_H_

// End of file
