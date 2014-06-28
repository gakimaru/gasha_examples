#pragma once
#ifndef __EXAMPLE_SINGLY_LINKED_LIST_H_
#define __EXAMPLE_SINGLY_LINKED_LIST_H_

//--------------------------------------------------------------------------------
// exmaple_singly_linked_list.h
// 片方向連結リストコンテナテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include <gasha/singly_linked_list.h>//片方向連結リストコンテナ【宣言部】

#include <gasha/shared_spin_lock.h>//共有スピンロック

//--------------------------------------------------------------------------------
//片方向連結リストテスト用設定とコンパイラスイッチ
static const int TEST_DATA_NUM = 10;//大量登録テストデータの登録数
//static const int TEST_DATA_NUM = 50000;//大量登録テストデータの登録数

static const int TEST_DATA_FIND_NUM = 1000;//大量テストの線形探索回数
static const int TEST_DATA_FIND_STEP = TEST_DATA_NUM > TEST_DATA_FIND_NUM ? TEST_DATA_NUM / TEST_DATA_FIND_NUM : 1;//大量テストの線形実行ステップ

#define ENABLE_SORT_TEST//大量データテストでソートを実行する場合、このマクロを有効化する

#define PRINT_TEST_DATA_DETAIL//テストデータの詳細タを表示する場合は、このマクロを有効化する
//#define TEST_DATA_WATCH_CONSTRUCTOR//コンストラクタ／デストラクタ／代入演算子の動作を確認する場合、このマクロを有効化する

#if defined(GASHA_SINGLY_LINKED_LIST_ENABLE_BINARY_SEARCH) && !defined(GASHA_SINGLY_LINKED_LIST_ENABLE_REVERSE_ITERATOR)
#undef GASHA_SINGLY_LINKED_LIST_ENABLE_BINARY_SEARCH
#endif

//片方向連結リストテスト
void example_singly_linked_list();

#endif//__EXAMPLE_SINGLY_LINKED_LIST_H_

// End of file
