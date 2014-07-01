#pragma once
#ifndef GASHA_INCLUDED_EXAMPLE_RING_BUFFER_H
#define GASHA_INCLUDED_EXAMPLE_RING_BUFFER_H

//--------------------------------------------------------------------------------
// exmaple_ring_buffer.h
// リングバッファコンテナテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include <gasha/ring_buffer.h>//リングバッファコンテナ【宣言部】

#include <gasha/shared_spin_lock.h>//共有スピンロック

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------------------------------------------------------------------
//リングバッファテスト用設定とコンパイラスイッチ

#ifdef GASHA_OPTIMIZED

static const int TEST_DATA_NUM = 10000000;//大量登録テストデータの登録数

static const int TEST_DATA_FIND_NUM = 100;//線形探索テストの回数
static const int TEST_DATA_FIND_STEP = TEST_DATA_NUM > TEST_DATA_FIND_NUM ? TEST_DATA_NUM / TEST_DATA_FIND_NUM : 1;//線形探索テストの実行ステップ

//#define PRINT_TEST_DATA_DETAIL//テストデータの詳細を表示する場合は、このマクロを有効にする
//#define TEST_DATA_WATCH_CONSTRUCTOR//コンストラクタ／デストラクタ／代入演算子の動作を確認する場合、このマクロを有効にする

#else//GASHA_OPTIMIZED

static const int TEST_DATA_NUM = 10;//大量登録テストデータの登録数

static const int TEST_DATA_FIND_NUM = 100;//線形探索テストの回数
static const int TEST_DATA_FIND_STEP = TEST_DATA_NUM > TEST_DATA_FIND_NUM ? TEST_DATA_NUM / TEST_DATA_FIND_NUM : 1;//線形探索テストの実行ステップ

#define PRINT_TEST_DATA_DETAIL//テストデータの詳細を表示する場合は、このマクロを有効にする
//#define TEST_DATA_WATCH_CONSTRUCTOR//コンストラクタ／デストラクタ／代入演算子の動作を確認する場合、このマクロを有効にする

#endif//GASHA_OPTIMIZED

//#define USE_STL_ALGORITM//ソート／線形探索／二分探索で、内部関数の代わりに STL を使用する場合は、このマクロを有効にする

//----------------------------------------
//テストデータ
struct data_t
{
	int m_key;//キー
	int m_val;//データ

	//コンストラクタ
	data_t(const int key, const int val);
	//デフォルトコンストラクタ
	data_t();
	//デストラクタ
	~data_t();
#ifdef TEST_DATA_WATCH_CONSTRUCTOR
	//ムーブオペレータ
	data_t& operator=(data_t&& rhs);
	//コピーオペレータ
	data_t& operator=(const data_t& rhs);
	//ムーブコンストラクタ
	data_t(data_t&& src);
	//コピーコンストラクタ
	data_t(const data_t& src);
#endif//TEST_DATA_WATCH_CONSTRUCTOR
	//デフォルトのソート用の比較演算子（必須ではない）
	inline bool operator<(const data_t& rhs) const
	{
		return m_key < rhs.m_key;
	}
	//デフォルトの線形／二分探索用の比較演算子（必須ではない）
	inline bool operator==(const int key) const
	{
		return m_key == key;
	}
	inline bool operator<(const int key) const
	{
		return m_key < key;
	}
#ifdef USE_STL_ALGORITM
	//※std::binary_search(), std::upper_bound() を使用する場合は、このオペレータも必要（std::lower_bound()には不要）
	friend inline bool operator<(const int key, const data_t& rhs)
	{
		return key < rhs.m_key;
	}
#endif//USE_STL_ALGORITM
};

//----------------------------------------
//テストデータ操作クラス①：デフォルトのまま使う
struct ope : public ring_buffer::baseOpe<ope, data_t>{};

//----------------------------------------
//テストデータ操作クラス②：ソート／探索方法をデフォルトから変える
struct another_ope : public ring_buffer::baseOpe<ope, data_t>
{
	//ソート用プレディケート関数オブジェクト
	//※m_valメンバーを基準にソート
	struct predicateForSort{
		inline bool operator()(const value_type& lhs, const value_type& rhs) const
		{
			return lhs.m_val < rhs.m_val;
		}
	};

	//線形探索用プレディケート関数オブジェクト
	//※m_valメンバーを探索
	struct predicateForFind{
		inline bool operator()(const value_type& lhs, const int rhs) const
		{
			return lhs.m_val == rhs;
		}
	};

	//二分探索用比較関数オブジェクト
	//※m_valメンバーを比較
	struct comparisonForSearch{
		inline int operator()(const value_type& lhs, const int rhs) const
		{
			return rhs - lhs.m_val;
		}
	};

	//デストラクタ呼び出しを禁止したい場合、
	//この空のメソッドを定義する。
	inline static void callDestructor(value_type* obj)
	{
		//何もしない
	}
};

//----------------------------------------
//テストデータ操作クラス③：ロックを有効化する
struct mt_ope_t : public ring_buffer::baseOpe<mt_ope_t, data_t>
{
	//ロック型
	typedef GASHA_ sharedSpinLock lock_type;//ロックオブジェクトを指定
};

//----------------------------------------
//int型用のデータ操作クラス定義
struct int_ope : public ring_buffer::baseOpe<ope, int>{};

//----------------------------------------
//リングバッファコンテナテスト
void example_ring_buffer();

//----------------------------------------
//シンプルリングバッファコンテナテスト
void example_simple_ring_buffer();

#endif//GASHA_INCLUDED_EXAMPLE_RING_BUFFER_H

// End of file
