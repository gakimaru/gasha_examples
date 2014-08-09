#pragma once
#ifndef GASHA_INCLUDED_EXAMPLE_HASH_TABLE_H
#define GASHA_INCLUDED_EXAMPLE_HASH_TABLE_H

//--------------------------------------------------------------------------------
// exmaple_hash_table.h
// 開番地法ハッシュテーブルコンテナテスト
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include <gasha/hash_table.h>//開番地法ハッシュテーブルコンテナ【宣言部】

#include <gasha/shared_spin_lock.h>//共有スピンロック
#include <gasha/crc32.h>//CRC32計算

#pragma warning(push)//【VC++】ワーニング設定を退避
#pragma warning(disable: 4530)//【VC++】C4530を抑える
#include <functional>//C++11 std::function
#pragma warning(pop)//【VC++】ワーニング設定を復元

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------------------------------------------------------------------
//開番地法ハッシュテーブルテスト用設定とコンパイラスイッチ

#ifdef GASHA_OPTIMIZED

static const int TEST_DATA_TABLE_SIZE = 50000;//テストデータテーブルサイズ

//#define TEST_ITERATOR_OPERATION//イテレータ操作をテストする場合は、このマクロを有効にする
//#define TEST_LOCK_OPERATION//ロック操作をテストする場合は、このマクロを有効にする

//#define PRINT_TEST_DATA_DETAIL//テストデータの詳細を表示する場合は、このマクロを有効化する
//#define TEST_DATA_WATCH_CONSTRUCTOR//コンストラクタ／デストラクタ／代入演算子の動作を確認する場合、このマクロを有効化する

#else//GASHA_OPTIMIZED

static const int TEST_DATA_TABLE_SIZE = 20;//テストデータテーブルサイズ

#define TEST_ITERATOR_OPERATION//イテレータ操作をテストする場合は、このマクロを有効にする
#define TEST_LOCK_OPERATION//ロック操作をテストする場合は、このマクロを有効にする

#define PRINT_TEST_DATA_DETAIL//テストデータの詳細を表示する場合は、このマクロを有効化する
//#define TEST_DATA_WATCH_CONSTRUCTOR//コンストラクタ／デストラクタ／代入演算子の動作を確認する場合、このマクロを有効化する

#endif//GASHA_OPTIMIZED

static const int TEST_DATA_TABLE_SIZE_FOR_POINTER = 100;//ポインター型テストデータテーブルサイズ
static const int TEST_DATA_TABLE_SIZE_FOR_FUNC = 100;//関数型テストデータテーブルサイズ

#define TEST_USE_INSERT_TYPE 1//テストで使用するデータ登録方法
                              //1 ... 【推奨】emplace()メソッドにキーとコンストラクタパラメータを渡して登録する方法
                              //2 ... emplaceAuto()メソッドにコンストラクタパラメータを渡して登録する方法 ※操作用クラス（baseOpe の派生クラス）で、getKey() を実装する必要あり
                              //3 ... insert()メソッドにキーとオブジェクトを渡して登録する方法
                              //4 ... insertAuto()メソッドにオブジェクトを渡して登録する方法 ※操作用クラス（baseOpe の派生クラス）で、getKey() を実装する必要あり
                              //5 ... assign()メソッドで先にキーの割り付けを行った後、データを書き込む

#define TEST_USE_FIND_TYPE 1//テストで使用するデータ検索方法
                            //1 ... 【推奨】[]オペレータにキーを渡して検索する方法
                            //2 ... at()メソッドにキーを渡して検索する方法
                            //3 ... find()メソッドにキーを渡して検索する方法 ※イテレータを返す

//----------------------------------------
//開番地法ハッシュテーブル用テストデータ
struct data_t
{
	crc32_t m_key;//キー ※名前のハッシュ値
	int m_value;//値
	char m_name[20];//名前

	//コンストラクタ
	data_t(const char* name, const int value);
#ifdef TEST_DATA_WATCH_CONSTRUCTOR
	//ムーブオペレータ
	data_t& operator=(data_t&& rhs);
	//コピーオペレータ
	data_t& operator=(const data_t& rhs);
	//ムーブコンストラクタ
	data_t(data_t&& src);
	//コピーコンストラクタ
	data_t(const data_t& src);
	//デフォルトコンストラクタ
	data_t();
	//デストラクタ
	~data_t();
#endif//TEST_DATA_WATCH_CONSTRUCTOR
};

//----------------------------------------
//開番地法ハッシュテーブル用テストデータ：関数呼び出しテスト用
struct calc_t
{
	int calc(int a, int b)
	{
		return (a + b) * m_val;
	}
	calc_t(const int val) :
		m_val(val)
	{}
private:
	int m_val;
};

//----------------------------------------
//テストデータ操作クラス
struct ope : public hash_table::baseOpe<ope, TEST_DATA_TABLE_SIZE, data_t, crc32_t>
{
	//データ置換属性
	//※デフォルト（NEVER_REPLACE）のままとする
	//static const replace_attr_t REPLACE_ATTR = REPLACE;//キーが重複するデータは置換して登録する

	//キーを取得
	inline static key_type getKey(const value_type& value){ return value.m_key; }

#ifdef TEST_LOCK_OPERATION
	//ロック型
	//※デフォルトは dummySharedLock
	typedef sharedSpinLock lock_type;//ロックオブジェクト型
#endif//TEST_LOCK_OPERATION

	//定数
	//※任意に変更可能な設定
	static const std::size_t AUTO_REHASH_RATIO = 0;//自動リハッシュ実行の基準割合(0～100) ※0で自動リハッシュなし
	//static const std::size_t FINDING_CYCLE_LIMIT = 1;//検索時の巡回回数の制限 ※0で無制限 ※追加・削除時にも影響する
	//static const std::size_t INDEX_STEP_BASE = 7;//検索巡回時のインデックスのス歩幅の基準値 ※必ず素数でなければならない
	//static const replaceAttr_t REPLACE_ATTR = replaceAttr_t::REPLACE;//キーが重複するデータは置換する
};

//----------------------------------------
//テストデータ操作クラス：ポインター型
struct ptr_ope : public hash_table::baseOpe<ptr_ope, TEST_DATA_TABLE_SIZE_FOR_POINTER, data_t*, int>
{
	//データ置換属性
	//※デフォルト（NEVER_REPLACE）のままとする
	//static const replace_attr_t REPLACE_ATTR = REPLACE;//キーが重複するデータは置換して登録する

	//キーを取得
	inline static key_type getKey(const value_type& value){ return value->m_key; }

	//ロック型
	//※デフォルト（dummy_shared_lock）のままとする
	//typedef shared_spin_lock lock_type;//ロックオブジェクト型
};

//----------------------------------------
//テストデータ操作クラス：キーの範囲が小さいデータ
struct narrow_range_key_ope : public hash_table::baseOpe<narrow_range_key_ope, TEST_DATA_TABLE_SIZE_FOR_FUNC, int, char>
{
	static const key_type KEY_MIN = -2;//キーの最小値 ※範囲外のキーは登録不可
	static const key_type KEY_MAX = 2;//キーの最大値 ※範囲外のキーは登録不可
	static const key_type INVALID_KEY = -99;//不正なキー
};

//----------------------------------------
//テストデータ操作クラス：関数型
struct func_ope : public hash_table::baseOpe<func_ope, TEST_DATA_TABLE_SIZE_FOR_FUNC, std::function<int(int, int)>, crc32_t>
{
};

//----------------------------------------
//テストデータ操作クラス：クラスメンバー関数型
struct obj_ope : public hash_table::baseOpe<func_ope, TEST_DATA_TABLE_SIZE_FOR_FUNC, std::function<int(calc_t&, int, int)>, crc32_t>
{
};

//----------------------------------------
//開番地法ハッシュテーブルコンテナテスト
void example_hash_table();

//----------------------------------------
//シンプル開番地法ハッシュテーブルコンテナテスト
void example_simple_hash_table();

#endif//GASHA_INCLUDED_EXAMPLE_HASH_TABLE_H

// End of file
