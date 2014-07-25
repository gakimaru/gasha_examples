//--------------------------------------------------------------------------------
// exmaple_serialization.cpp
// シリアライズテスト
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_serialization.h"//シリアライズテスト

#include "basic_test/basic_test.h"//基本テスト
#include "feature_test/feature_test.h"//機能テスト
#include "real_test/real_test.h"//実ゲーム処理テスト

//----------------------------------------
//シリアライズテスト
void example_serialization()
{
#ifdef ENABLE_BASIC_TEST
	//基本テスト
	basic_test();
#endif//ENABLE_BASIC_TEST

#ifdef ENABLE_FEATURE_TEST
	//機能テスト
	feature_test();
#endif//ENABLE_FEATURE_TEST

#ifdef ENABLE_REAL_TEST
	//実ゲーム処理テスト
	real_test();
#endif//ENABLE_REAL_TEST
}

// End of file
