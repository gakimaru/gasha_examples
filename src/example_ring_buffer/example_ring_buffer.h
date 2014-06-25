#pragma once
#ifndef __EXAMPLE_RING_BUFFER_H_
#define __EXAMPLE_RING_BUFFER_H_

//--------------------------------------------------------------------------------
// exmaple_ring_buffer.h
// リングバッファテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//リングバッファテスト用設定とコンパイラスイッチ
static const int TEST_DATA_NUM = 10;//大量登録テストデータの登録数
//static const int TEST_DATA_NUM = 10000000;//大量登録テストデータの登録数

static const int TEST_DATA_FIND_NUM = 100;//大量テストの線形探索回数
static const int TEST_DATA_FIND_STEP = TEST_DATA_NUM > TEST_DATA_FIND_NUM ? TEST_DATA_NUM / TEST_DATA_FIND_NUM : 1;//大量テストの線形実行ステップ

#define PRINT_TEST_DATA_DETAIL//テストデータの詳細タを表示する場合は、このマクロを有効化する
//#define TEST_DATA_WATCH_CONSTRUCTOR//コンストラクタ／デストラクタ／代入演算子の動作を確認する場合、このマクロを有効化する

//リングバッファテスト
void example_ring_buffer();

#endif//__EXAMPLE_RING_BUFFER_H_

// End of file
