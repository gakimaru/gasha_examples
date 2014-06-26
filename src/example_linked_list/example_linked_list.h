#pragma once
#ifndef __EXAMPLE_LINKED_LIST_H_
#define __EXAMPLE_LINKED_LIST_H_

//--------------------------------------------------------------------------------
// exmaple_linked_list.h
// 双方向連結リストテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//双方向連結リストテスト用設定とコンパイラスイッチ
static const int TEST_DATA_NUM = 10;//大量登録テストデータの登録数
//static const int TEST_DATA_NUM = 20000;//大量登録テストデータの登録数

static const int TEST_DATA_FIND_NUM = 1000;//大量テストの線形探索回数
static const int TEST_DATA_FIND_STEP = TEST_DATA_NUM > TEST_DATA_FIND_NUM ? TEST_DATA_NUM / TEST_DATA_FIND_NUM : 1;//大量テストの線形実行ステップ

#define ENABLE_SORT_TEST//大量データテストでソートを実行する場合、このマクロを有効化する

#define PRINT_TEST_DATA_DETAIL//テストデータの詳細タを表示する場合は、このマクロを有効化する
//#define TEST_DATA_WATCH_CONSTRUCTOR//コンストラクタ／デストラクタ／代入演算子の動作を確認する場合、このマクロを有効化する

//--------------------------------------------------------------------------------
//双方向連結リストのコンパイラスイッチ
//#define GASHA_LINKED_LIST_ENABLE_BINARY_SEARCH//二分探索を有効にする
//#define GASHA_LINKED_LIST_ENABLE_STABLE_SORT//安定ソートを有効にする
//#define GASHA_LINKED_LIST_USE_SHELL_SORT//通常ソートにシェルソートを使用する（無効化時は挿入ソートを使用）


//双方向連結リストテスト
void example_linked_list();

#endif//__EXAMPLE_LINKED_LIST_H_

// End of file
