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

#define ENABLE_TEST_FOR_STACK_ALLOATOR//スタックアロケータのテストを有効にする場合は、このマクロを有効化する
#define ENABLE_TEST_FOR_DUAL_STACK_ALLOATOR//スタックアロケータのテストを有効にする場合は、このマクロを有効化する

#define USE_LOCK_TYPE 0//共有データのロックに dummyLock を使用する場合は、このマクロを有効化する（ロックしなくなる）（デフォルト）
//#define USE_LOCK_TYPE 1//共有データのロックに spinLock を使用する場合は、このマクロを有効化する
//#define USE_LOCK_TYPE 2//共有データのロックに std::mutex を使用する場合は、このマクロを有効化する

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

//--------------------------------------------------------------------------------
//各マルチスレッド共有データ定義

//----------------------------------------
//マルチスレッド共有データに使用するロックオブジェクトの設定

#if USE_LOCK_TYPE == 1//共有データのロックに spinLock を使用する場合

#include <gasha/spin_lock.h>//スピンロック
typedef GASHA_ spinLock lock_type;//共有データのロックに spinLock を使用する場合は、この行を有効化する

#elif USE_LOCK_TYPE == 2//共有データのロックに std::mutex を使用する場合

//【VC++】例外を無効化した状態で <mutex> をインクルードすると、warning C4530 が発生する
//  warning C4530: C++ 例外処理を使っていますが、アンワインド セマンティクスは有効にはなりません。/EHsc を指定してください。
#pragma warning(disable: 4530)//C4530を抑える

#include <mutex>//C++11 std::mutex
typedef std::mutex lock_type;//共有データのロックに std::mutex を使用する場合は、この行を有効化する

#else//if USE_LOCK_TYPE == 0//共有データのロックに dummyLock を使用する場合

#include <gasha/dummy_lock.h>//ダミーロック
typedef GASHA_ dummyLock lock_type;//共有データのロックに dummyLock を使用する場合は、この行を有効化する（ロックしなくなる）

#endif//USE_LOCK_TYPE

//----------------------------------------
//アロケータテスト
void example_stack_allocator();//スタックアロケータテスト
void example_dual_stack_allocator();//双方向スタックアロケータテスト
void example_pool_allocator();//プールアロケータテスト
void example_mono_allocator();//単一アロケータテスト
void example_std_allocator();//標準アロケータテスト
void example_poly_allocator();//多態アロケータテスト
void example_allocator_performance();//アロケータパフォーマンステスト

#endif//GASHA_INCLUDED_EXAMPLE_ALLOCATOR_H

// End of file
