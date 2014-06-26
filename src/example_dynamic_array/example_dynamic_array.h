#pragma once
#ifndef __EXAMPLE_DYNAMIC_ARRAY_H_
#define __EXAMPLE_DYNAMIC_ARRAY_H_

//--------------------------------------------------------------------------------
// exmaple_dynamic_array.h
// 動的配列コンテナテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include <gasha/dynamic_array.h>//動的配列【宣言部】

#include <gasha/shared_spin_lock.h>//共有スピンロック

//----------------------------------------
//動的配列テスト用設定とコンパイラスイッチ

#ifdef _DEBUG//デバッグ版
//static const int TEST_DATA_NUM = 10;//大量登録テストデータの登録数
static const int TEST_DATA_NUM = 10000;//大量登録テストデータの登録数

static const int TEST_DATA_FIND_NUM = 100;//大量テストの線形探索回数
static const int TEST_DATA_FIND_STEP = TEST_DATA_NUM > TEST_DATA_FIND_NUM ? TEST_DATA_NUM / TEST_DATA_FIND_NUM : 1;//大量テストの線形実行ステップ

#else//_DEBUG//リリース版
//static const int TEST_DATA_NUM = 10;//大量登録テストデータの登録数
static const int TEST_DATA_NUM = 10000000;//大量登録テストデータの登録数

static const int TEST_DATA_FIND_NUM = 100;//大量テストの線形探索回数
static const int TEST_DATA_FIND_STEP = TEST_DATA_NUM > TEST_DATA_FIND_NUM ? TEST_DATA_NUM / TEST_DATA_FIND_NUM : 1;//大量テストの線形実行ステップ

#endif//_DEBUG

//#define PRINT_TEST_DATA_DETAIL//テストデータの詳細を表示する場合は、このマクロを有効化する
//#define TEST_DATA_WATCH_CONSTRUCTOR//コンストラクタ／デストラクタ／代入演算子の動作を確認する場合、このマクロを有効化する

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
	data_t& operator=(const data_t&& rhs);
	//コピーオペレータ
	data_t& operator=(const data_t& rhs);
	//ムーブコンストラクタ
	data_t(const data_t&& src);
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
};
//※std::binary_searchを使用する場合は、このオペレータも必要
static inline bool operator<(const int key, const data_t& rhs)
{
	return key < rhs.m_key;
}

//----------------------------------------
//テストデータ操作クラス①：デフォルトのまま使う
struct ope_t : public GASHA_ dynamic_array::baseOpe_t<ope_t, data_t>{};

//----------------------------------------
//テストデータ操作クラス②：ソート／探索方法をデフォルトから変える
struct another_ope_t : public GASHA_ dynamic_array::baseOpe_t<ope_t, data_t>
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
struct mt_ope_t : public GASHA_ dynamic_array::baseOpe_t<mt_ope_t, data_t>
{
	//ロック型
	typedef GASHA_ sharedSpinLock lock_type;//ロックオブジェクトを指定
};

//----------------------------------------
//int型用のデータ操作クラス定義
struct int_ope_t : public GASHA_ dynamic_array::baseOpe_t<ope_t, int>{};

//----------------------------------------
//動的配列コンテナテスト
void example_dynamic_array();

#endif//__EXAMPLE_DYNAMIC_ARRAY_H_

// End of file
