//--------------------------------------------------------------------------------
// exmaple_serialization.cpp
// シリアライズテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_serialization.h"//シリアライズテスト

#include "feature_test/feature_test.h"//機能テスト

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
}

// End of file
