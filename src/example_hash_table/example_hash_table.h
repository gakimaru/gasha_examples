#pragma once
#ifndef __EXAMPLE_HASH_TABLE_H_
#define __EXAMPLE_HASH_TABLE_H_

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

//--------------------------------------------------------------------------------
//ハッシュテーブルテスト用設定とコンパイラスイッチ
//static const int TEST_DATA_TABLE_SIZE = 500000;//テストデータテーブルサイズ
static const int TEST_DATA_TABLE_SIZE = 20;//テストデータテーブルサイズ

#define PRINT_TEST_DATA_DETAIL//テストデータの詳細を表示する場合は、このマクロを有効化する
//#define TEST_DATA_WATCH_CONSTRUCTOR//コンストラクタ／デストラクタ／代入演算子の動作を確認する場合、このマクロを有効化する

//#define USE_GCC//GCC版でコンパイルするときは、このマクロを有効化する


//開番地法ハッシュテーブルコンテナテスト
void example_hash_table();

#endif//__EXAMPLE_HASH_TABLE_H_

// End of file
