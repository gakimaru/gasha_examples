#pragma once
#ifndef GASHA_INCLUDED_FEATURE_TEST_H
#define GASHA_INCLUDED_FEATURE_TEST_H

//--------------------------------------------------------------------------------
// feature_test.h
// シリアライズテスト：機能テスト【宣言部】
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

//【注】ポインタ変数のシリアライズをテストするために、new /delete を多用。
//　　　（スタックアロケータによるメモリ管理などは行わず、単純に処理。）

#include "example_serialization.h"//シリアライズテスト
#include "feature_test/test_data.h"//テストデータ定義

//--------------------
//テストデータ取得
testData* getTestData();

//--------------------
//テストデータインスタンス生成
testData* createTestData();

//--------------------
//テストデータインスタンス破棄
void destroyTestData();

//--------------------
//機能テスト
void feature_test();

#endif//GASHA_INCLUDED_FEATURE_TEST_H

// End of file
