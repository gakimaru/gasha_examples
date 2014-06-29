#pragma once
#ifndef __EXAMPLE_LINKED_LIST_H_
#define __EXAMPLE_LINKED_LIST_H_

//--------------------------------------------------------------------------------
// exmaple_linked_list.h
// 双方向連結リストコンテナテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include <gasha/linked_list.h>//双方向連結リストコンテナ【宣言部】

#include <gasha/shared_spin_lock.h>//共有スピンロック

//----------------------------------------
//双方向連結リストテスト用設定とコンパイラスイッチ

#ifdef GASHA_OPTIMIZED

static const int TEST_DATA_NUM = 20000;//大量登録テストデータの登録数

static const int TEST_DATA_FIND_NUM = 100;//線形探索テストの回数
static const int TEST_DATA_FIND_STEP = TEST_DATA_NUM > TEST_DATA_FIND_NUM ? TEST_DATA_NUM / TEST_DATA_FIND_NUM : 1;//線形探索テストの実行ステップ

//#define PRINT_TEST_DATA_DETAIL//テストデータの詳細を表示する場合は、このマクロを有効化する
//#define TEST_DATA_WATCH_CONSTRUCTOR//コンストラクタ／デストラクタ／代入演算子の動作を確認する場合、このマクロを有効化する

#else//GASHA_OPTIMIZED

static const int TEST_DATA_NUM = 10;//大量登録テストデータの登録数

static const int TEST_DATA_FIND_NUM = 100;//線形探索テストの回数
static const int TEST_DATA_FIND_STEP = TEST_DATA_NUM > TEST_DATA_FIND_NUM ? TEST_DATA_NUM / TEST_DATA_FIND_NUM : 1;//線形探索テストの実行ステップ

#define PRINT_TEST_DATA_DETAIL//テストデータの詳細を表示する場合は、このマクロを有効化する
//#define TEST_DATA_WATCH_CONSTRUCTOR//コンストラクタ／デストラクタ／代入演算子の動作を確認する場合、このマクロを有効化する

#endif//GASHA_OPTIMIZED

//#define USE_STL_ALGORITM//ソート／線形探索／二分探索で、内部関数の代わりに STL を使用する場合は、このマクロを有効にする

GASHA_USING_NAMESPACE;//ネームスペース使用

//----------------------------------------
//テストデータ
struct data_t
{
	mutable const data_t* m_next;//次ノード
	mutable const data_t* m_prev;//前ノード

	int m_key;//キー
	int m_val;//値

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
#ifdef GASHA_LINKED_LIST_ENABLE_BINARY_SEARCH
	inline bool operator<(const int key) const
	{
		return m_key < key;
	}
#endif//GASHA_LINKED_LIST_ENABLE_BINARY_SEARCH
};

//----------------------------------------
//テストデータ向けノード操作用クラス（CRTP）
struct ope : public linked_list::baseOpe<ope, data_t>
{
	//前ノードを取得
	inline static const node_type* getPrev(const node_type& node){ return node.m_prev; }
	//前ノードを変更
	inline static void setPrev(node_type& node, const node_type* prev){ node.m_prev = prev; }

	//次ノードを取得
	inline static const node_type* getNext(const node_type& node){ return node.m_next; }
	//次ノードを変更
	inline static void setNext(node_type& node, const node_type* next){ node.m_next = next; }

	//ロック型
	//※デフォルト（dummySharedLock）のままとする
	//typedef GASHA_ sharedSpinLock lock_type;//ロックオブジェクト型
};

//----------------------------------------
//テストデータ操作クラス②：ソート／探索方法をデフォルトから変える
struct another_ope_t : public ope
{
	//ソート用プレディケート関数オブジェクト
	//※m_valメンバーを基準にソート
	struct predicateForSort{
		inline bool operator()(const node_type& lhs, const node_type& rhs) const
		{
			return lhs.m_val < rhs.m_val;
		}
	};

	//線形探索用プレディケート関数オブジェクト
	//※m_valメンバーを探索
	struct predicateForFind{
		inline bool operator()(const node_type& lhs, const int rhs) const
		{
			return lhs.m_val == rhs;
		}
	};

#ifdef GASHA_LINKED_LIST_ENABLE_BINARY_SEARCH
	//二分探索用比較関数オブジェクト
	//※m_valメンバーを比較
	struct comparisonForSearch{
		inline int operator()(const node_type& lhs, const int rhs) const
		{
			return rhs - lhs.m_val;
		}
	};
#endif//GASHA_LINKED_LIST_ENABLE_BINARY_SEARCH
};

//----------------------------------------
//双方向連結リストコンテナテスト
void example_linked_list();

//----------------------------------------
//シンプル双方向連結リストコンテナテスト
void example_simple_linked_list();

#endif//__EXAMPLE_LINKED_LIST_H_

// End of file
