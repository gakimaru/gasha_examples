#pragma once
#ifndef GASHA_INCLUDED_EXAMPLE_RB_TREE_H
#define GASHA_INCLUDED_EXAMPLE_RB_TREE_H

//--------------------------------------------------------------------------------
// exmaple_rb_tree.h
// 赤黒木コンテナテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include <gasha/rb_tree.h>//赤黒木コンテナ【宣言部】

#include <gasha/shared_spin_lock.h>//共有スピンロック

//--------------------------------------------------------------------------------
//赤黒木テスト用設定とコンパイラスイッチ

#ifdef GASHA_OPTIMIZED

static const int TEST_DATA_KEY_MIN = 1;//テストデータの最小キー
static const int TEST_DATA_KEY_MAX = 1000;//テストデータの最大キー
static const int TEST_DATA_REG_NUM = 1000000;//テストデータの登録数
static const int TEST_DATA_STACK_DEPTH_MAX = 40;//テストデータの赤黒木操作用スタックの最大の深さ（デフォルトは40で、平衡木なら100万件は扱える）

//#define TEST_ITERATOR_OPERATION//イテレータ操作をテストする場合は、このマクロを有効にする
//#define TEST_LOCK_OPERATION//ロック操作をテストする場合は、このマクロを有効にする

//#define PRINT_TEST_DATA_TREE//テストデータの木を表示する場合は、このマクロを有効化する（表示しなくても検索は実行する）
//#define PRINT_TEST_DATA_SEARCH//テストデーたの検索結果を表示する場合は、このマクロを有効化する（表示しなくても検索は実行する）
//#define PRINT_TEST_DATA_COLOR_COUNT//テストデータの赤黒カウント数計測を表示する場合は、このマクロを有効化する
//#define PRINT_TEST_DATA_DETAIL//テストデータの詳細を表示する場合は、このマクロを有効化する
//#define TEST_DATA_WATCH_CONSTRUCTOR//コンストラクタ／デストラクタ／代入演算子の動作を確認する場合、このマクロを有効化する

#else//GASHA_OPTIMIZED

static const int TEST_DATA_KEY_MIN = 1;//テストデータの最小キー
static const int TEST_DATA_KEY_MAX = 10;//テストデータの最大キー
static const int TEST_DATA_REG_NUM = 20;//テストデータの登録数
static const int TEST_DATA_STACK_DEPTH_MAX = 40;//テストデータの赤黒木操作用スタックの最大の深さ（デフォルトは40で、平衡木なら100万件は扱える）

#define TEST_ITERATOR_OPERATION//イテレータ操作をテストする場合は、このマクロを有効にする
#define TEST_LOCK_OPERATION//ロック操作をテストする場合は、このマクロを有効にする

#define PRINT_TEST_DATA_TREE//テストデータの木を表示する場合は、このマクロを有効化する（表示しなくても検索は実行する）
#define PRINT_TEST_DATA_SEARCH//テストデーたの検索結果を表示する場合は、このマクロを有効化する（表示しなくても検索は実行する）
#define PRINT_TEST_DATA_COLOR_COUNT//テストデータの赤黒カウント数計測を表示する場合は、このマクロを有効化する
#define PRINT_TEST_DATA_DETAIL//テストデータの詳細を表示する場合は、このマクロを有効化する
//#define TEST_DATA_WATCH_CONSTRUCTOR//コンストラクタ／デストラクタ／代入演算子の動作を確認する場合、このマクロを有効化する

#endif//GASHA_OPTIMIZED

//#define REGIST_TEST_DATA_SEQUENTIALLY//データをシーケンシャルに登録する場合は、このマクロを有効化する（無効化時はランダム）

//テストデータを固定順に登録する場合は、このマクロを有効化する（無効時はランダム、ただし、REGIST_TEST_DATA_SEQUENTIALLYが優先）
//#define TEST_DATA_REGISTRATION_LIST { 54, 59, 71, 84, 60, 85, 54, 84, 42, 62, 64, 38, 43, 29, 89, 5, 96, 27, 38, 47, 79, 81, 52, 47, 56, 39, 92, 83, 7, 33, 8, 64, 2, 36, 83, 95, 77, 14, 87, 87, 97, 47, 79, 80, 46, 52, 78, 67, 11, 72, 63, 58, 14, 53, 94, 75, 52, 10, 41, 47, 26, 18, 77, 73, 45, 21, 56, 13, 1, 32, 61, 14, 61, 22, 61, 38, 94, 90, 68, 44, 35, 61, 43, 90, 69, 9, 6, 96, 66, 65, 67, 17, 21, 35, 12, 75, 31, 60, 36, 32}

//テストデータを固定順に削除する場合は、このマクロを有効化する（無効時はランダム）
//#define TEST_DATA_REMOVING_LIST { 41, 72, 12, 14, 9, 39, 18, 38, 66, 53, 84, 31, 68, 52, 44, 87 }

GASHA_USING_NAMESPACE;//ネームスペース使用

//----------------------------------------
//テストデータ
struct data_t
{
	mutable const data_t* m_childS;//小（左）側の子ノード
	mutable const data_t* m_childL;//大（右）側の子ノード

	bool m_isBlack;//ノードの色
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
	data_t& operator=(data_t&& rhs);
	//コピーオペレータ
	data_t& operator=(const data_t& rhs);
	//ムーブコンストラクタ
	data_t(data_t&& src);
	//コピーコンストラクタ
	data_t(const data_t& src);
#endif//TEST_DATA_WATCH_CONSTRUCTOR
};
//----------------------------------------
//テストデータ向けノード操作用クラス（CRTP）
struct ope : public rb_tree::baseOpe<ope, data_t, int, TEST_DATA_STACK_DEPTH_MAX>
{
	//子ノードを取得
	inline static const node_type* getChildL(const node_type& node){ return node.m_childL; }//大（右）側
	inline static const node_type* getChildS(const node_type& node){ return node.m_childS; }//小（左）側
	//子ノードを変更
	inline static void setChildL(node_type& node, const node_type* child){ node.m_childL = child; }//大（右）側
	inline static void setChildS(node_type& node, const node_type* child){ node.m_childS = child; }//小（左）側

	//ノードの色を取得
	inline static color_t getColor(const node_type& node){ return node.m_isBlack ? color_t::BLACK : color_t::RED; }
	//ノードの色を変更
	inline static void setColor(node_type& node, const color_t color){ node.m_isBlack = color == color_t::BLACK; }

	//キーを取得
	inline static key_type getKey(const node_type& node){ return node.m_key; }

	//キーを比較
	//※デフォルトのままとする
	//inline static int compareKey(const key_type lhs, const key_type rhs);

#ifdef TEST_LOCK_OPERATION
	//ロック型
	//※デフォルトは dummySharedLock
	typedef sharedSpinLock lock_type;//ロックオブジェクト型
#endif//TEST_LOCK_OPERATION
};

//----------------------------------------
//赤黒木コンテナテスト
void example_rb_tree();

//----------------------------------------
//シンプル赤黒木コンテナテスト
void example_simple_rb_tree();

#endif//GASHA_INCLUDED_EXAMPLE_RB_TREE_H

// End of file
