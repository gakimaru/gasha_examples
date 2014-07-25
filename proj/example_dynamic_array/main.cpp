//--------------------------------------------------------------------------------
// main.cpp
// 動的配列コンテナテスト - メイン
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_dynamic_array.h"//動的配列テスト

#include <cstdio>
#include <cstdlib>

int main(const int argc, const char* argv[])
{
	//動的配列コンテナテスト
	example_dynamic_array();
	
	//シンプル動的配列コンテナテスト
	example_simple_dynamic_array();

	return EXIT_SUCCESS;
}

// End of file
