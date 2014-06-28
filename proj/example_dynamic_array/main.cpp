//--------------------------------------------------------------------------------
// main.cpp
// 動的配列コンテナテスト - メイン
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_dynamic_array.h"//動的配列テスト

#include <stdio.h>
#include <stdlib.h>

int main(const int argc, const char* argv[])
{
	//動的配列コンテナテスト
	example_dynamic_array();
	
	//動的配列シンプルコンテナテスト
	example_simple_dynamic_array();

	return EXIT_SUCCESS;
}

// End of file
