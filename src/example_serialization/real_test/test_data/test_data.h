#pragma once
#ifndef GASHA_INCLUDED_TEST_DATA_H
#define GASHA_INCLUDED_TEST_DATA_H

//--------------------------------------------------------------------------------
// test_data.h
// テストデータ【宣言部】
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

//--------------------
//全データリセット
void resetAll();

//--------------------
//テストデータ作成
void makeTestData(const int pattern);

//--------------------
//現在のデータを表示
void printDataAll();

//--------------------
//データ操作テスト
void testDataControl();

#endif//GASHA_INCLUDED_TEST_DATA_H

// End of file
