#pragma once
#ifndef GASHA_INCLUDED_EXAMPLE_SERIALIZATION_H
#define GASHA_INCLUDED_EXAMPLE_SERIALIZATION_H

//--------------------------------------------------------------------------------
// exmaple_serialization.h
// シリアライズテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//テスト用コンパイラスイッチ／定数

#define ENABLE_BASIC_TEST//シリアライズ系基本テストを有効にする場合は、このマクロを有効化する
#define ENABLE_FEATURE_TEST//シリアライズ系機能テストを有効にする場合は、このマクロを有効化する
#define ENABLE_REAL_TEST//実際のゲームを想定したテストを有効にする場合は、このマクロを有効化する

#define FEATURE_TEST_ENABLE_TO_SERIALIZE_BITSET//機能テストで std::bitset のユーザー定義シリアライズを行う場合、このマクロを有効化する（セーブデータのサイズが大きくなる）
//#define FEATURE_TEST_ENABLE_TO_OCCUR_FATAL_ERROR//機能テストで致命的エラーを発生させる場合、このマクロを有効化する
//#define FEATURE_TEST_HAS_INVALID_ITEM//機能テストで無効なデータ（重複した名前のデータ項目を登録）を扱う場合、このマクロを有効化する

//----------------------------------------
//シリアライズテスト
void example_serialization();

#endif//GASHA_INCLUDED_EXAMPLE_SERIALIZATION_H

// End of file
