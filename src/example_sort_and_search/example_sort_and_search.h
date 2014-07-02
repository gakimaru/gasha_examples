#pragma once
#ifndef GASHA_INCLUDED_EXAMPLE_SORT_AND_SEARCH_H
#define GASHA_INCLUDED_EXAMPLE_SORT_AND_SEARCH_H

//--------------------------------------------------------------------------------
// exmaple_sort_and_search.h
// ソート＆探索処理テスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//ソートアルゴリズムテスト用設定とコンパイラスイッチ

#ifdef GASHA_OPTIMIZED

static const int TEST_DATA_COUNT = 10000;//テストデータ件数（最大）

#else//GASHA_OPTIMIZED

static const int TEST_DATA_COUNT = 100;//テストデータ件数（最大）

#endif//GASHA_OPTIMIZED

//#define TEST_DATA_WATCH_CONSTRUCTOR//コンストラクタ／デストラクタ／代入演算子の動作を確認する場合、このマクロを有効化する

//#define TEST_DATA_SAME_KEY_NUM 1//テストデータに登録する同一キーデータの数（各キーのデータを指定数ずつ登録） ※安定ソートの挙動確認用
#define TEST_DATA_SAME_KEY_NUM 3//テストデータに登録する同一キーデータの数（各キーのデータを指定数ずつ登録） ※安定ソートの挙動確認用

//#define TEST_DATA_SIZE 512//テストデータ一件のサイズ
//#define TEST_DATA_SIZE 256//テストデータ一件のサイズ
#define TEST_DATA_SIZE 64//テストデータ一件のサイズ
//#define TEST_DATA_SIZE 4//テストデータ一件のサイズ

#define TEST_DATA_ALIGN 16//テストデータ一件のアラインメント
//#define TEST_DATA_ALIGN 4//テストデータ一件のアラインメント

//【VC++】ワーニング設定を退避
#pragma warning(push)

//【VC++】例外を無効化した状態で <array> をインクルードすると warning C4530 が出る
//  warning C4530: C++ 例外処理を使っていますが、アンワインド セマンティクスは有効にはなりません。/EHsc を指定してください。
#pragma warning(disable: 4530)//C4530を抑える

#include <array>//C++11 std::array

//----------------------------------------
//テスト用構造体
struct alignas(TEST_DATA_ALIGN) data_t
{
	int m_key;//キー
	int m_seqNo;//シーケンス番号
	char m_data[TEST_DATA_SIZE];//データ（大き目のサイズにする）

#ifdef TEST_DATA_WATCH_CONSTRUCTOR
	data_t& operator=(data_t&& rhs);
	data_t& operator=(const data_t& rhs);
	data_t(data_t&& src);
	data_t(const data_t& src);
	data_t();
	~data_t();
#endif//TEST_DATA_WATCH_CONSTRUCTOR
};

//----------------------------------------
//テスト用構造体の配列型
class array_t : public std::array<data_t, TEST_DATA_COUNT>
{
public:
	void* operator new(const size_t size);
	void operator delete (void* p);
};

//----------------------------------------
//テスト用の比較処理
//※通常関数
bool predicate_func(const data_t& lhs, const data_t& rhs);

//----------------------------------------
//ソート＆探索処理テスト関数
void example_sort_and_search();

//【VC++】ワーニング設定を退避
#pragma warning(pop)

#endif//GASHA_INCLUDED_EXAMPLE_SORT_AND_SEARCH_H

// End of file
