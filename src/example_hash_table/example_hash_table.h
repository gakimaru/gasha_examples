#pragma once
#ifndef GASHA_INCLUDED_EXAMPLE_HASH_TABLE_H
#define GASHA_INCLUDED_EXAMPLE_HASH_TABLE_H

//--------------------------------------------------------------------------------
// exmaple_hash_table.h
// 開番地法ハッシュテーブルコンテナテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include <gasha/hash_table.h>//開番地法ハッシュテーブルコンテナ【宣言部】

#include <gasha/shared_spin_lock.h>//共有スピンロック
#include <gasha/crc32.h>//CRC32計算

//【VC++】例外を無効化した状態で <functoinal> をインクルードすると、もしくは、new 演算子を使用すると warning C4530 が出る
//  warning C4530: C++ 例外処理を使っていますが、アンワインド セマンティクスは有効にはなりません。/EHsc を指定してください。
#pragma warning(disable: 4530)//C4530を抑える

#include <functional>//C++11 std::function

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------------------------------------------------------------------
//開番地法ハッシュテーブルテスト用設定とコンパイラスイッチ

#ifdef GASHA_OPTIMIZED

static const int TEST_DATA_TABLE_SIZE = 500000;//テストデータテーブルサイズ

//#define PRINT_TEST_DATA_DETAIL//テストデータの詳細を表示する場合は、このマクロを有効化する
//#define TEST_DATA_WATCH_CONSTRUCTOR//コンストラクタ／デストラクタ／代入演算子の動作を確認する場合、このマクロを有効化する

#else//GASHA_OPTIMIZED

static const int TEST_DATA_TABLE_SIZE = 20;//テストデータテーブルサイズ

#define PRINT_TEST_DATA_DETAIL//テストデータの詳細を表示する場合は、このマクロを有効化する
//#define TEST_DATA_WATCH_CONSTRUCTOR//コンストラクタ／デストラクタ／代入演算子の動作を確認する場合、このマクロを有効化する

#endif//GASHA_OPTIMIZED

static const int TEST_DATA_TABLE_SIZE_FOR_POINTER = 100;//ポインター型テストデータテーブルサイズ
static const int TEST_DATA_TABLE_SIZE_FOR_FUNC = 100;//関数型テストデータテーブルサイズ

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
struct ope : public hash_table::baseOpe<ope, data_t, crc32_t>
{
	//データ置換属性
	//※デフォルト（NEVER_REPLACE）のままとする
	//static const replace_attr_t REPLACE_ATTR = REPLACE;//キーが重複するデータは置換して登録する

	//キーを取得
	inline static key_type getKey(const value_type& value){ return value.m_key; }

	//ロック型
	//※デフォルト（dummy_shared_lock）のままとする
	//typedef shared_spin_lock lock_type;//ロックオブジェクト型
};

//----------------------------------------
//テストデータ操作クラス：ポインター型
struct ptr_ope : public hash_table::baseOpe<ptr_ope, data_t*, int>
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
//テストデータ操作クラス：関数型
struct func_ope : public hash_table::baseOpe<func_ope, std::function<int(int, int)>, crc32_t>
{
};

//----------------------------------------
//テストデータ操作クラス：クラスメンバー関数型
struct obj_ope : public hash_table::baseOpe<func_ope, std::function<int(calc_t&, int, int)>, crc32_t>
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
