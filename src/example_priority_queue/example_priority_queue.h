#pragma once
#ifndef GASHA_INCLUDED_EXAMPLE_EXAMPLE_EXAMPLE_PRIORITY_QEUEUE_H
#define GASHA_INCLUDED_EXAMPLE_EXAMPLE_EXAMPLE_PRIORITY_QEUEUE_H

//--------------------------------------------------------------------------------
// exmaple_priority_queue.h
// 優先度付きキューコンテナアダプタ／二分ヒープコンテナテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include <gasha/binary_heap.h>//二分ヒープコンテナ【宣言部】
#include <gasha/priority_queue.h>//優先度付きキューコンテナアダプタ【宣言部】

#include <gasha/spin_lock.h>//スピンロック

//--------------------------------------------------------------------------------
//二分ヒープテスト用設定とコンパイラスイッチ
static const int TEST_DATA_PRIOR_MIN = 1;//テストデータの最小プライオリティ
static const int TEST_DATA_PRIOR_MAX = 5;//テストデータの最大プライオリティ
static const int TEST_DATA_MAX = 128;//テストデータの最大登録可能数
static const int TEST_DATA_REG_NUM = 20;//テストデータの登録数
//static const int TEST_DATA_MAX = 1000000;//テストデータの最大登録可能数
//static const int TEST_DATA_REG_NUM = 1000000;//テストデータの登録数

#define PRINT_TEST_DATA_DETAIL//テストデータの詳細タを表示する場合は、このマクロを有効化する
//#define TEST_DATA_WATCH_CONSTRUCTOR//コンストラクタ／デストラクタ／代入演算子の動作を確認する場合、このマクロを有効化する


//優先度付きキュー／二分ヒープテスト
void example_priority_queue();

#endif//GASHA_INCLUDED_EXAMPLE_EXAMPLE_EXAMPLE_PRIORITY_QEUEUE_H

// End of file
