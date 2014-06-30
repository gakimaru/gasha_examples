#pragma once
#ifndef __EXAMPLE_RB_TREE_H_
#define __EXAMPLE_RB_TREE_H_

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
static const int TEST_DATA_KEY_MIN = 1;//テストデータの最小キー
static const int TEST_DATA_KEY_MAX = 10;//テストデータの最大キー
static const int TEST_DATA_REG_NUM = 20;//テストデータの登録数
static const int TEST_DATA_STACK_DEPTH_MAX = 40;//テストデータの赤黒木操作用スタックの最大の深さ（デフォルトは40で、平衡木なら100万件は扱える）

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

//#define REGIST_TEST_DATA_SEQUENTIALLY//データをシーケンシャルに登録する場合は、このマクロを有効化する（無効化時はランダム）
#define PRINT_TEST_DATA_TREE//テストデータの木を表示する場合は、このマクロを有効化する（表示しなくても検索は実行する）
#define PRINT_TEST_DATA_SEARCH//テストデーたの検索結果を表示する場合は、このマクロを有効化する（表示しなくても検索は実行する）
#define PRINT_TEST_DATA_COLOR_COUNT//テストデータの赤黒カウント数計測を表示する場合は、このマクロを有効化する
#define PRINT_TEST_DATA_DETAIL//テストデータの詳細を表示する場合は、このマクロを有効化する
//#define ENABLE_CALC_COUNT_PERFORMANCE//データ件数カウントの処理時間を計測する場合は、このマクロを有効化する

//#define USE_STL_ALGORITM//ソート／線形探索／二分探索で、内部関数の代わりに STL を使用する場合は、このマクロを有効にする

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
	inline static color_t getColor(const node_type& node){ return node.m_isBlack ? BLACK : RED; }
	//ノードの色を変更
	inline static void setColor(node_type& node, const color_t color){ node.m_isBlack = color == BLACK; }

	//キーを取得
	inline static key_type getKey(const node_type& node){ return node.m_key; }

	//キーを比較
	//※デフォルトのままとする
	//inline static int compareKey(const key_type lhs, const key_type rhs);

	//ロック型
	//※デフォルト（dummy_shared_lock）のままとする
	//typedef shared_spin_lock lock_type;//ロックオブジェクト型
};

//----------------------------------------
//赤黒木コンテナテスト
void example_rb_tree();

//----------------------------------------
//シンプル赤黒木コンテナテスト
void example_simple_rb_tree();

#endif//__EXAMPLE_RB_TREE_H_

// End of file
